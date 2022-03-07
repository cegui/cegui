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
#include "CEGUI/XMLSerializer.h"
#include <algorithm>

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
    d_requestedChildIdx(std::numeric_limits<size_t>().max())
{
    // grid size is 0x0 that means 0 child windows,
    // no need to populate d_children with dummies

    addGridLayoutContainerProperties();
}

//----------------------------------------------------------------------------//
void GridLayoutContainer::setGridDimensions(size_t width, size_t height)
{
    if (width == d_gridWidth && height == d_gridHeight)
        return;

    // Too many things may change, so drop the cache
    d_freeSearchStart = 0;

    const size_t oldWidth = d_gridWidth;
    const size_t oldHeight = d_gridHeight;

    d_gridWidth = width;
    d_gridHeight = height;

    // Empty cells will be filled with dummies after loading
    if (d_initialising)
        return;

    std::vector<Element*> oldChildren;
    std::swap(oldChildren, d_children);

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
        while (itUnusedChild != oldChildren.end())
        {
            auto& unusedChild = *itUnusedChild;
            ++itUnusedChild;
            if (isDummy(*unusedChild))
            {
                child = unusedChild;
                unusedChild = nullptr;
                break;
            }
        }

        // If there are no reusable dummies left, the new one is created but not
        // added yet, because nullptrs in a d_children may confuse Window code
        if (!child)
            child = createDummy();
    }

    // Register new dummies as our childs. This will not insert a second copy
    // of the child into d_children, but will do all necessary setup.
    for (Element* child : d_children)
        if (!child->getParentElement())
            addChild(child);

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
    for (const Element* child : d_children)
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
                const Window* child = getChildAtIndex(mapCellToIndex(x, y));
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
void GridLayoutContainer::addChildAtIndex(Element* element, size_t index)
{
    // Intended usage of this method is insertion of the new item into an
    // one-dimensional child list, like in an editor. If the target cell is
    // empty then the new child will be inserted into it. Otherwise next
    // children are shifted until an empty cell or the end of the grid reached.
    // If the end was reached, exception is thrown because of insufficient space.

    size_t x, y;
    mapIndexToCell(index, x, y);
    validateGridCell(x, y);

    // The target cell is not free
    if (!isDummy(*getChildAtIndex(index)))
    {
        // Find the first free cell from the requested position
        size_t freeIdx = getFirstFreeIndex(index + 1);

        if (freeIdx >= getChildCount())
        {
            // The grid is full
            if (d_autoGrow)
            {
                freeIdx = getChildCount();
                growByOneLine();
            }
            else
            {
                // No free cell, we can't insert a child so that the desired order is maintained
                throw InvalidRequestException(
                    "Unable to add a child because the grid has no appropriate free cells.");
            }
        }

        // Transfer the free cell to the target index, now we can insert a child there
        moveChildToIndex(freeIdx, index);
    }

    d_requestedChildIdx = index;
    addChild(element);
}

//----------------------------------------------------------------------------//
void GridLayoutContainer::removeChildAtIndex(size_t index)
{
    auto child = getChildAtIndex(index);
    if (child && !isDummy(*child))
        removeChild(child);
}

//----------------------------------------------------------------------------//
void GridLayoutContainer::moveChildToIndex(size_t indexFrom, size_t indexTo)
{
    if (indexFrom >= getChildCount())
        return;

    size_t x, y;
    mapIndexToCell(indexTo, x, y);
    validateGridCell(x, y);

    LayoutContainer::moveChildToIndex(indexFrom, indexTo);
}

//----------------------------------------------------------------------------//
void GridLayoutContainer::addChildToCell(Window* window, size_t gridX, size_t gridY, bool replace)
{
    validateGridCell(gridX, gridY);

    const auto index = mapCellToIndex(gridX, gridY);
    if (!replace && !isDummy(*getChildAtIndex(index)))
        throw InvalidRequestException("Target cell is busy, set replace to true to replace existing item.");

    d_requestedChildIdx = index;
    addChild(window);
}

//----------------------------------------------------------------------------//
void GridLayoutContainer::removeChildFromCell(size_t gridX, size_t gridY)
{
    removeChildAtIndex(mapCellToIndex(gridX, gridY));
}

//----------------------------------------------------------------------------//
void GridLayoutContainer::moveChildToCell(Window* wnd, size_t gridX, size_t gridY)
{
    moveChildToIndex(getChildIndex(wnd), mapCellToIndex(gridX, gridY));
}

//----------------------------------------------------------------------------//
void GridLayoutContainer::swapCells(size_t gridX1, size_t gridY1,
                                    size_t gridX2, size_t gridY2)
{
    swapChildren(mapCellToIndex(gridX1, gridY1), mapCellToIndex(gridX2, gridY2));
}

//----------------------------------------------------------------------------//
Window* GridLayoutContainer::getChildAtCell(size_t gridX, size_t gridY) const
{
    if (gridX >= d_gridWidth || gridY >= d_gridHeight)
        return nullptr;

    return getChildAtIndex(mapCellToIndex(gridX, gridY));
}

//----------------------------------------------------------------------------//
void GridLayoutContainer::layout_impl()
{
    // Used to compare UDims
    const Rectf& childContentArea = getChildContentArea().get();
    const float absWidth = childContentArea.getWidth();
    const float absHeight = childContentArea.getHeight();

    // First, we need to determine rowSizes and colSizes, this is
    // needed before any layouting work takes place
    std::vector<UDim> colSizes(d_gridWidth, UDim(0, 0));
    std::vector<UDim> rowSizes(d_gridHeight, UDim(0, 0));
    for (size_t y = 0; y < d_gridHeight; ++y)
    {
        for (size_t x = 0; x < d_gridWidth; ++x)
        {
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
void GridLayoutContainer::onChildOrderChanged(ElementEventArgs& e)
{
    // TODO: could optimize if e would contain the first affected index
    d_freeSearchStart = 0;
    LayoutContainer::onChildOrderChanged(e);
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
void GridLayoutContainer::validateGridCell(size_t gridX, size_t gridY)
{
    if (gridX < d_gridWidth && gridY < d_gridHeight)
        return;

    if (d_autoGrow)
        setGridDimensions(std::max(d_gridWidth, gridX + 1), std::max(d_gridHeight, gridY + 1));
    else
        throw InvalidRequestException("The cell specified is out of the grid bounds.");
}

//----------------------------------------------------------------------------//
void GridLayoutContainer::growByOneLine()
{
    const size_t newWidth = std::max(size_t{1}, d_gridWidth + (d_rowMajor ? 0 : 1));
    const size_t newHeight = std::max(size_t{1}, d_gridHeight + (d_rowMajor ? 1 : 0));
    setGridDimensions(newWidth, newHeight);
}

//----------------------------------------------------------------------------//
void GridLayoutContainer::endInitialisation()
{
    // After we loaded children from XML, fill remaining free cells with dummies
    const size_t capacity = d_gridWidth * d_gridHeight;
    for (size_t i = d_children.size(); i < capacity; ++i)
        addChild(createDummy());

    LayoutContainer::endInitialisation();
}

//----------------------------------------------------------------------------//
void GridLayoutContainer::addChild_impl(Element* element)
{
    // Custom logic for dummies. Allow to refresh children already in the list.
    // It is necessary for rearrangement optimization when resizing the grid.
    // Skips LayoutContainer's subscriptions on child resizing and draw list
    // maintaining because dummies have no size and are invisible.
    // Skips already existing child name check because we guarantee uniqueness.
    if (isDummy(*element))
    {
        Element::addChild_impl(element);
        return;
    }

    // Regular logic for the loading time
    if (d_initialising)
    {
        LayoutContainer::addChild_impl(element);
        return;
    }

    // If requested index is unspecified (invalid), get the first free index
    if (d_requestedChildIdx >= getChildCount())
    {
        d_requestedChildIdx = getFirstFreeIndex(d_freeSearchStart);

        if (d_requestedChildIdx >= getChildCount())
        {
            // The grid is full
            if (d_autoGrow)
            {
                d_requestedChildIdx = getChildCount();
                growByOneLine();
            }
            else
            {
                throw InvalidRequestException(
                    "Unable to add a child because the grid is full. Consider using the "
                    "GridLayoutContainer::addChildToCell function with replace=true, "
                    "clearing some cells manually with GridLayoutContainer::removeChild[FromCell] "
                    "or using an auto-growing feature with GridLayoutContainer::setAutoGrowing.");
            }
        }

        // We now know an index of the first free cell. Caching it allows us not to scan the
        // whole child list each time we insert a child without specifying a location. This
        // cache is invalidated in some situations. Be careful when changing related logic.
        d_freeSearchStart = d_requestedChildIdx + 1;
    }

    // The new child is added to the end of d_children
    LayoutContainer::addChild_impl(element);

    // We swap the element in the target cell with the added child. This essentially
    // places the added child to its right position and puts the replaced element at
    // the end of d_children it will soon get removed from. Note that replaced element
    // may be a dummy or a regular child. The d_requestedChildIdx pointing to it means
    // that the user confirmed replacement earlier.
    std::swap(d_children[d_requestedChildIdx], d_children[d_children.size() - 1]);

    // Clear the requested index so that it doesn't influence the next call
    d_requestedChildIdx = std::numeric_limits<size_t>().max();

    // Finally remove the child that was in the target cell before
    Window* toBeRemoved = static_cast<Window*>(d_children[d_children.size() - 1]);
    removeChild(toBeRemoved);
    if (toBeRemoved->isDestroyedByParent())
        WindowManager::getSingleton().destroyWindow(toBeRemoved);
}

//----------------------------------------------------------------------------//
void GridLayoutContainer::removeChild_impl(Element* element)
{
    // Before we remove the child, we must add a new dummy and place it
    // instead of the removed child. There are exceptions though:
    // 1. If the grid is being destroyed, we don't want to maintain its structure anymore.
    // 2. If the grid is being initialized, structure is not established yet.
    // 3. If removing a dummy. It is always an intentional internal action.
    // 4. If the window manager is locked, at which time we can't create a new dummy.
    // 5. If the child is not in our list, which happens internally when grid is resized.

    if (!d_destructionStarted &&
        !d_initialising &&
        !isDummy(*element) &&
        !WindowManager::getSingleton().isLocked())
    {
        auto it = std::find(d_children.cbegin(), d_children.cend(), element);
        if (it != d_children.cend())
        {
            const size_t idx = static_cast<size_t>(std::distance(d_children.cbegin(), it));
            addChild(createDummy());
            std::swap(d_children[idx], d_children[d_children.size() - 1]);

            if (d_freeSearchStart > idx)
                d_freeSearchStart = idx;
        }
    }
    else if (WindowManager::getSingleton().isLocked())
    {
        // Free cells may shift towards the beginning of the child list
        if (d_freeSearchStart > 0)
            --d_freeSearchStart;
    }

    LayoutContainer::removeChild_impl(element);
}

//----------------------------------------------------------------------------//
Window* GridLayoutContainer::getChildAutoWindow(const String& name)
{
    // Since dummies are on-demand and their names may change from one run to
    // another, we create them right at the loading time and ignore exact names.
    if (d_initialising && name == DummyName)
    {
        Window* dummy = createDummy();
        addChild(dummy);
        return dummy;
    }

    return LayoutContainer::getChildAutoWindow(name);
}

//----------------------------------------------------------------------------//
int GridLayoutContainer::writeChildWindowsXML(XMLSerializer& xml_stream) const
{
    size_t windowsWritten = 0;
    size_t dummiesSkipped = 0;
    for (const Element* child : d_children)
    {
        if (isDummy(*child))
        {
            // Trailing dummies aren't saved
            ++dummiesSkipped;
        }
        else
        {
            windowsWritten += dummiesSkipped + 1;

            while (dummiesSkipped)
            {
                // All dummies are written with the same name. It serves
                // as a type for on-demand creation when loading.
                xml_stream.openTag(AutoWindowXMLElementName);
                xml_stream.attribute(AutoWindowNamePathXMLAttributeName, DummyName);
                xml_stream.closeTag();
                --dummiesSkipped;
            }

            static_cast<const Window*>(child)->writeXMLToStream(xml_stream);
        }
    }

    return static_cast<int>(windowsWritten);
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

}
