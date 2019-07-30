/***********************************************************************
    created:    01/8/2010
    author:     Martin Preisler
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2010 Paul D Turner & The CEGUI Development Team
 *
 *   Permission is hereby granted, free of charge, to any person obtaining
 *   a copy of this software and associated documentation files (the
 *   "Software"), to deal in the Software without restriction, including
 *   without limitation the rights to use, copy, modify, merge, publish,
 *   distribute, sublicense, and/or sell copies of the Software, and to
 *   permit persons to whom the Software is furnished to do so, subject to
 *   the following conditions:
 *
 *   The above copyright notice and this permission notice shall be
 *   included in all copies or substantial portions of the Software.
 *
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 *   EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 *   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 *   IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 *   OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 *   ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 *   OTHER DEALINGS IN THE SOFTWARE.
 ***************************************************************************/
#include "CEGUI/widgets/GridLayoutContainer.h"
#include "CEGUI/WindowManager.h"
#include "CEGUI/CoordConverter.h"

namespace CEGUI
{
const String GridLayoutContainer::WidgetTypeName("GridLayoutContainer");
const String GridLayoutContainer::EventNamespace("GridLayoutContainer");

// name for dummies, a number is added to the end of that
const String GridLayoutContainer::DummyName("__auto_dummy_");

//----------------------------------------------------------------------------//
static inline bool isDummy(const Element& child)
{
    // all auto windows inside grid are dummies
    return static_cast<const Window&>(child).isAutoWindow();
}

//----------------------------------------------------------------------------//
GridLayoutContainer::GridLayoutContainer(const String& type, const String& name) :
    LayoutContainer(type, name),
    d_gridWidth(0),
    d_gridHeight(0),
    d_nextDummyIdx(0),
    d_requestedChildIdx(std::numeric_limits<size_t>().max())
{
    // grid size is 0x0 that means 0 child windows,
    // no need to populate d_children with dummies

    addGridLayoutContainerProperties();
}

//----------------------------------------------------------------------------//
GridLayoutContainer::~GridLayoutContainer(void)
{}

//----------------------------------------------------------------------------//
void GridLayoutContainer::setGridDimensions(size_t width, size_t height)
{
    if (width == d_gridWidth && height == d_gridHeight)
        return;

    const size_t oldWidth = d_gridWidth;
    const size_t oldHeight = d_gridHeight;

    d_gridWidth = width;
    d_gridHeight = height;

    ChildList oldChildren = std::move(d_children);

    d_children.reserve(width * height);

    // To handle majority we operate not on rows & columns, but on major & minor dimensions
    const size_t oldMajorDim = d_rowMajor ? oldWidth : oldHeight;
    const size_t newMajorDim = d_rowMajor ? width : height;
    const size_t oldMinorDim = d_rowMajor ? oldHeight : oldWidth;
    const size_t newMinorDim = d_rowMajor ? height : width;
    const size_t majorTransferSize = std::min(oldMajorDim, newMajorDim);
    const size_t minorTransferSize = std::min(oldMinorDim, newMinorDim);
    const size_t majorTailSize = (newMajorDim > oldMajorDim) ? (newMajorDim - oldMajorDim) : 0;
    const size_t minorTailSize = (newMinorDim > oldMinorDim) ? (newMinorDim - oldMinorDim) : 0;

    // Copy existing data row by row or column by column
    auto itMajorStart = oldChildren.begin();
    for (size_t y = 0; y < minorTransferSize; ++y)
    {
        // Move transferrable part of the line
        if (majorTransferSize)
        {
            d_children.insert(d_children.end(), itMajorStart, itMajorStart + majorTransferSize);
            std::fill(itMajorStart, itMajorStart + majorTransferSize, nullptr);
        }

        // Fill the remaining part of the line with dummies. Set nullptr
        // for now, fill with real dummies later.
        if (majorTailSize)
            d_children.insert(d_children.end(), majorTailSize, nullptr);

        itMajorStart += oldMajorDim;
    }

    // Remove empty records from oldChildren, leaving only unused children there
    oldChildren.erase(std::remove(oldChildren.begin(), oldChildren.end(), nullptr), oldChildren.end());

    // Fill remaining cells with dummies. Set nullptr for now, fill with real
    // dummies later. That's not necessary but done this way for code unification.
    if (minorTailSize)
        d_children.insert(d_children.end(), minorTailSize * newMajorDim, nullptr);

    // Do a second pass over new children, fixing nullptrs with real dummies
    auto itUnusedChild = oldChildren.begin();
    for (auto& child : d_children)
    {
        if (child)
            continue;

        // Find a reusable dummy if there is one
        Element* dummy = nullptr;
        while (itUnusedChild != oldChildren.end())
        {
            auto& oldChild = *itUnusedChild;
            ++itUnusedChild;
            if (isDummy(*oldChild))
            {
                dummy = oldChild;
                oldChild = nullptr;
                break;
            }
        }

        child = dummy ? dummy : createDummy();
    }

    // Now remove children that are left unused and do not fit into the new grid
    for (Element* child : oldChildren)
    {
        // Skip reused dummies, we replaced them with nullptrs above
        if (!child)
            continue;

        // Grid will not repace this child with a dummy because it isn't present
        // in the actual child list. This is done intentionally and is a key point.
        removeChild(child);

        if (static_cast<Window*>(child)->isDestroyedByParent())
            WindowManager::getSingleton().destroyWindow(static_cast<Window*>(child));
    }
}

//----------------------------------------------------------------------------//
void GridLayoutContainer::setGridWidth(size_t width)
{
    setGridDimensions(width, d_gridHeight);
}

//----------------------------------------------------------------------------//
void GridLayoutContainer::setGridHeight(size_t height)
{
    setGridDimensions(d_gridWidth, height);
}

//----------------------------------------------------------------------------//
void GridLayoutContainer::setRowMajor(bool rowMajor)
{
    if (d_rowMajor == rowMajor) return;

    d_rowMajor = rowMajor;
    markNeedsLayouting();
}

//----------------------------------------------------------------------------//
void GridLayoutContainer::setAutoGrowing(bool enabled)
{
    d_autoGrow = enabled;
}

//----------------------------------------------------------------------------//
size_t GridLayoutContainer::getActualChildCount() const
{
    size_t count = 0;
    for (auto* child : d_children)
        if (!isDummy(*child))
            ++count;

    return count;
}

//----------------------------------------------------------------------------//
void GridLayoutContainer::getMinimalSizeInCells(size_t& width, size_t& height) const
{
    width = 0;
    height = 0;
    for (size_t y = 0; y < d_gridHeight; ++y)
    {
        for (size_t x = 0; x < d_gridWidth; ++x)
        {
            if (x >= width || y >= height)
            {
                Window* child = getChildAtIndex(mapCellToIndex(x, y));
                if (child && !isDummy(*child))
                {
                    if (x >= width)
                        width = x + 1;
                    if (y >= height)
                        height = y + 1;
                }
            }
        }
    }
}

//----------------------------------------------------------------------------//
size_t GridLayoutContainer::getFirstFreeIndex(size_t start) const
{
    for (size_t i = start; i < d_children.size(); ++i)
        if (isDummy(*getChildAtIndex(i)))
            return i;

    return std::numeric_limits<size_t>().max();
}

//----------------------------------------------------------------------------//
size_t GridLayoutContainer::getLastBusyIndex() const
{
    bool found = false;
    size_t lastIdx = 0;
    for (size_t i = 0; i < d_children.size(); ++i)
    {
        if (!isDummy(*getChildAtIndex(i)))
        {
            lastIdx = i;
            found = true;
        }
    }

    return found ? lastIdx : std::numeric_limits<size_t>().max();
}

//----------------------------------------------------------------------------//
void GridLayoutContainer::addChildToIndex(Element* element, size_t index)
{
    // Intended usage of this method is insertion of the new item into an
    // one-dimensional child list, like in an editor. If the target cell is
    // empty then the new child will be inserted into it. Otherwise next
    // children are shifted until an empty cell or the end of the grid reached.
    // If the end was reached, exception is thrown because of insufficient space.

    if (index >= getChildCount())
        throw InvalidRequestException("Invalid index specified.");

    // The target cell is not free
    if (!isDummy(*getChildAtIndex(index)))
    {
        // Find the first free cell from the requested position
        const size_t freeIdx = getFirstFreeIndex(index + 1);

        // No free cell, we can't insert a child so that the desired order is maintained
        if (freeIdx >= getChildCount())
            throw InvalidRequestException(
                "Unable to add a child because the grid has no appropriate free cells.");

        // Transfer the free cell to the target index, now we can insert a child there
        moveChildToIndex(freeIdx, index);
    }

    d_requestedChildIdx = index;
    addChild(element);
}

//----------------------------------------------------------------------------//
void GridLayoutContainer::addChildToCell(Window* window, size_t gridX, size_t gridY, bool replace)
{
    const auto index = mapCellToIndex(gridX, gridY);
    if (index >= getChildCount())
        throw InvalidRequestException("Invalid index specified.");
    if (!replace && !isDummy(*getChildAtIndex(index)))
        throw InvalidRequestException("Target cell is busy, set replace to true to replace existing item.");

    d_requestedChildIdx = index;
    addChild(window);
}

//----------------------------------------------------------------------------//
Window* GridLayoutContainer::getChildAtCell(size_t gridX, size_t gridY) const
{
    if (gridX >= d_gridWidth || gridY >= d_gridHeight)
        return nullptr;

    return getChildAtIndex(mapCellToIndex(gridX, gridY));
}

//----------------------------------------------------------------------------//
void GridLayoutContainer::removeChildFromCell(size_t gridX, size_t gridY)
{
    if (auto child = getChildAtCell(gridX, gridY))
        removeChild(child);
}

//----------------------------------------------------------------------------//
void GridLayoutContainer::swapCells(size_t gridX1, size_t gridY1,
                                    size_t gridX2, size_t gridY2)
{
    swapChildren(mapCellToIndex(gridX1, gridY1), mapCellToIndex(gridX2, gridY2));
}

//----------------------------------------------------------------------------//
void GridLayoutContainer::moveChildToCell(Window* wnd, size_t gridX, size_t gridY)
{
    removeChild(wnd);
    addChildToCell(wnd, gridX, gridY);
}

//----------------------------------------------------------------------------//
void GridLayoutContainer::layout()
{
    std::vector<UDim> colSizes(d_gridWidth, UDim(0, 0));
    std::vector<UDim> rowSizes(d_gridHeight, UDim(0, 0));

    // Used to compare UDims
    const Rectf& childContentArea = getChildContentArea().get();
    const float absWidth = childContentArea.getWidth();
    const float absHeight = childContentArea.getHeight();

    // First, we need to determine rowSizes and colSizes, this is
    // needed before any layouting work takes place
    for (size_t y = 0; y < d_gridHeight; ++y)
    {
        for (size_t x = 0; x < d_gridWidth; ++x)
        {
            //!!!!!!!!!!!!!!
            // FIXME: child pixel size is probably not yet calculated!
            //!!!!!!!!!!!!!!!!!!

            Window* window = getChildAtIndex(mapCellToIndex(x, y));
            const UVector2 size = getBoundingSizeForWindow(window);

            if (CoordConverter::asAbsolute(colSizes[x], absWidth) <
                CoordConverter::asAbsolute(size.d_x, absWidth))
            {
                colSizes[x] = size.d_x;
            }

            if (CoordConverter::asAbsolute(rowSizes[y], absHeight) <
                CoordConverter::asAbsolute(size.d_y, absHeight))
            {
                rowSizes[y] = size.d_y;
            }
        }
    }

    // OK, now rowSizes[y] is the height of y-th row
    //         colSizes[x] is the width of x-th column

    // Second layouting phase starts now
    UDim cellX;
    UDim cellY(0.f, 0.f);
    for (size_t y = 0; y < d_gridHeight; ++y)
    {
        cellX.d_scale = 0.f;
        cellX.d_offset = 0.f;
        
        for (size_t x = 0; x < d_gridWidth; ++x)
        {
            Window* window = getChildAtIndex(mapCellToIndex(x, y));
            const UVector2 offset = getOffsetForWindow(window);
            window->setPosition(UVector2(cellX, cellY) + offset);

            cellX += colSizes[x];
        }

        cellY += rowSizes[y];
    }

    // Now we just need to set the total width and height

    setSize(USize(cellX, cellY));
}

//----------------------------------------------------------------------------//
size_t GridLayoutContainer::mapCellToIndex(size_t gridX, size_t gridY) const
{
    return d_rowMajor ? (gridY * d_gridWidth + gridX) : (gridX * d_gridHeight + gridY);
}

//----------------------------------------------------------------------------//
void GridLayoutContainer::mapIndexToCell(size_t idx, size_t& gridX, size_t& gridY) const
{
    if (d_rowMajor)
    {
        gridY = idx / d_gridWidth;
        gridX = idx - gridY * d_gridWidth;
    }
    else
    {
        gridX = idx / d_gridHeight;
        gridY = idx - gridX * d_gridHeight;
    }
}

//----------------------------------------------------------------------------//
Window* GridLayoutContainer::createDummy()
{
    Window* dummy = WindowManager::getSingleton().createWindow("DefaultWindow",
                    DummyName + std::to_string(d_nextDummyIdx));

    ++d_nextDummyIdx;

    dummy->setAutoWindow(true);
    dummy->setVisible(false);
    dummy->setSize(USize(UDim(0, 0), UDim(0, 0)));
    dummy->setDestroyedByParent(true);

    return dummy;
}

//----------------------------------------------------------------------------//
void GridLayoutContainer::addChild_impl(Element* element)
{
    if (!d_gridWidth || !d_gridHeight)
    {
        throw InvalidRequestException(
            "GridLayoutContainer must have non-zero width and height to accept children.");
    }

    LayoutContainer::addChild_impl(element);

    if (!isDummy(*element))
    {
        // OK, wnd is already in d_children, now move it to the right place
        // If requested index is unspecified (invalid), get the first free index
        if (d_requestedChildIdx >= getChildCount())
        {
            d_requestedChildIdx = getFirstFreeIndex();

            // The grid is full, we can't insert a new child
            if (d_requestedChildIdx >= getChildCount())
            {
                LayoutContainer::removeChild_impl(element);
                throw InvalidRequestException(
                    "Unable to add a child because the grid is full. Consider using the "
                    "GridLayoutContainer::addChildToCell function with replace=true or "
                    "clearing some cells manually with GridLayoutContainer::removeChild[FromCell]");
            }
        }

        // We swap the element in the target cell with the added child. This essentially
        // places the added child to its right position and puts the replaced element at
        // the end of d_children it will soon get removed from. Note that replaced element
        // may be a dummy or a regular child. The d_requestedChildIdx pointing to it means
        // that the user confirmed replacement earlier.
        std::swap(d_children[d_requestedChildIdx], d_children[d_children.size() - 1]);

        // Clear the requested index so that it doesn't influence the next call
        d_requestedChildIdx = std::numeric_limits<size_t>().max();

        Window* toBeRemoved = static_cast<Window*>(d_children[d_children.size() - 1]);

        removeChild(toBeRemoved);

        if (toBeRemoved->isDestroyedByParent())
            WindowManager::getSingleton().destroyWindow(toBeRemoved);
    }
}

//----------------------------------------------------------------------------//
void GridLayoutContainer::removeChild_impl(Element* element)
{
    // Before we remove the child, we must add a new dummy and place it
    // instead of the removed child. There are exceptions though:
    // 1. If the grid is being destroyed, we don't want to maintain its structure anymore.
    // 2. If removing a dummy. It is always an intentional internal action.
    // 3. If the window manager is locked, at which time we can't create a new dummy.
    // 4. If the child is not in our list, which happens internally when grid is resized.
    if (!d_destructionStarted &&
        !isDummy(*element) &&
        !WindowManager::getSingleton().isLocked())
    {
        auto it = std::find(d_children.cbegin(), d_children.cend(), element);
        if (it != d_children.cend())
        {
            const auto idx = std::distance(d_children.cbegin(), it);
            addChild(createDummy());
            std::swap(d_children[idx], d_children[d_children.size() - 1]);
        }
    }

    LayoutContainer::removeChild_impl(element);
}

//----------------------------------------------------------------------------//
void GridLayoutContainer::addGridLayoutContainerProperties(void)
{
    const String& propertyOrigin = WidgetTypeName;

    CEGUI_DEFINE_PROPERTY(GridLayoutContainer, size_t,
        "GridWidth", "Width of the grid of this layout container. "
        "Value is an unsigned integer number.",
        &GridLayoutContainer::setGridWidth, &GridLayoutContainer::getGridWidth, 0
    );

    CEGUI_DEFINE_PROPERTY(GridLayoutContainer, size_t,
        "GridHeight", "Height of the grid of this layout container. "
        "Value is an unsigned integer number.",
        &GridLayoutContainer::setGridHeight, &GridLayoutContainer::getGridHeight, 0
    );

    CEGUI_DEFINE_PROPERTY(GridLayoutContainer, bool,
        "RowMajor", "Boolean flag - whether grid is laid out row-major (true) or column-major (false)",
        &GridLayoutContainer::setRowMajor, &GridLayoutContainer::isRowMajor, true
    );

    CEGUI_DEFINE_PROPERTY(GridLayoutContainer, bool,
        "AutoGrow", "Allow the grid to expand when its capacity is insufficient to add a new item",
        &GridLayoutContainer::setAutoGrowing, &GridLayoutContainer::isAutoGrowing, false
    );
}

//----------------------------------------------------------------------------//

} // End of  CEGUI namespace section