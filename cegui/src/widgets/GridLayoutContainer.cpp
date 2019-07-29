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
#include "CEGUI/Exceptions.h"
#include "CEGUI/CoordConverter.h"
#include "CEGUI/SharedStringStream.h"
#include <limits>

// Start of CEGUI namespace section
namespace CEGUI
{
/*************************************************************************
    Constants
*************************************************************************/
// type name for this widget
const String GridLayoutContainer::WidgetTypeName("GridLayoutContainer");
// name for dummies, a number is added to the end of that
const String GridLayoutContainer::DummyName("__auto_dummy_");

const String GridLayoutContainer::EventNamespace("GridLayoutContainer");

//----------------------------------------------------------------------------//
static size_t cellToIndex(size_t gridX, size_t gridY,
                          size_t gridWidth, size_t gridHeight)
{
    // example:
    // d_children = {1, 2, 3, 4, 5, 6}
    // grid is 3x2
    // 1 2 3
    // 4 5 6

    assert(gridX < gridWidth);
    assert(gridY < gridHeight);
    CEGUI_UNUSED(gridHeight); // silence warning in release builds

    return gridY * gridWidth + gridX;
}

//----------------------------------------------------------------------------//
static void indexToCell(size_t idx, size_t& gridX, size_t& gridY,
                        size_t gridWidth, size_t gridHeight)
{
    gridX = 0;
    gridY = 0;

    while (idx >= gridWidth)
    {
        idx -= gridWidth;
        ++gridY;
    }

    assert(gridY < gridHeight);
    CEGUI_UNUSED(gridHeight); // silence warning in release builds

    gridX = idx;
}

/*************************************************************************
    Constructor
*************************************************************************/
GridLayoutContainer::GridLayoutContainer(const String& type,
                                         const String& name) :
    LayoutContainer(type, name),

    d_gridWidth(0),
    d_gridHeight(0),

    d_autoPositioning(AutoPositioning::LeftToRight),
    d_nextAutoPositioningIdx(0),

    d_nextGridX(std::numeric_limits<size_t>::max()),
    d_nextGridY(std::numeric_limits<size_t>::max()),

    d_nextDummyIdx(0)
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
    // copy the old children list
    ChildList oldChildren = d_children;

    // remove all child windows
    while (getChildCount() != 0)
        removeChild(d_children[0]);

    const size_t oldWidth = d_gridWidth;

    const size_t oldHeight = d_gridHeight;

    const AutoPositioning oldAO = d_autoPositioning;

    d_gridWidth = width;

    d_gridHeight = height;

    // we simply fill the grid with dummies to ensure everything works smoothly
    // when something is added to the grid, it simply replaces the dummy
    for (size_t i = 0; i < width * height; ++i)
    {
        Window* dummy = createDummy();
        addChild(dummy);
    }

    // now we have to map oldChildren to new children
    for (size_t y = 0; y < height; ++y)
    {
        for (size_t x = 0; x < width; ++x)
        {
            // we have to skip if we are out of the old grid
            if (x >= oldWidth || y >= oldHeight)
                continue;

            const size_t oldIdx = cellToIndex(x, y, oldWidth, oldHeight);
            Window* previous = static_cast<Window*>(oldChildren[oldIdx]);

            if (isDummy(previous))
            {
                WindowManager::getSingleton().destroyWindow(previous);
            }
            else
            {
                addChildToCell(previous, x, y);
            }

            oldChildren[oldIdx] = nullptr;
        }
    }

    d_autoPositioning = oldAO;
    // oldAOIdx could mean something completely different now!
    // todo: perhaps convert oldAOOdx to new AOIdx?
    d_nextAutoPositioningIdx = 0;

    // we have to destroy windows that don't fit the new grid if they are set
    // to be destroyed by parent
    for (size_t i = 0; i < oldChildren.size(); ++i)
    {
        if (oldChildren[i] && static_cast<Window*>(oldChildren[i])->isDestroyedByParent())
        {
            WindowManager::getSingleton().destroyWindow(static_cast<Window*>(oldChildren[i]));
        }
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
size_t GridLayoutContainer::getGridWidth() const
{
    return d_gridWidth;
}

//----------------------------------------------------------------------------//
size_t GridLayoutContainer::getGridHeight() const
{
    return d_gridHeight;
}

//----------------------------------------------------------------------------//
void GridLayoutContainer::setAutoPositioning(AutoPositioning positioning)
{
    if (d_autoPositioning == positioning) return;

    d_autoPositioning = positioning;
    d_nextAutoPositioningIdx = 0;
}

//----------------------------------------------------------------------------//
GridLayoutContainer::AutoPositioning GridLayoutContainer::getAutoPositioning() const
{
    return d_autoPositioning;
}

//----------------------------------------------------------------------------//
void GridLayoutContainer::setNextAutoPositioningIdx(size_t idx)
{
    d_nextAutoPositioningIdx = idx;
}

//----------------------------------------------------------------------------//
size_t GridLayoutContainer::getNextAutoPositioningIdx() const
{
    return d_nextAutoPositioningIdx;
}

//----------------------------------------------------------------------------//
void GridLayoutContainer::autoPositioningSkipCells(size_t cells)
{
    setNextAutoPositioningIdx(getNextAutoPositioningIdx() + cells);
}

//----------------------------------------------------------------------------//
size_t GridLayoutContainer::getActualChildCount() const
{
    size_t count = 0;
    for (auto* child : d_children)
    {
        if (!isDummy(static_cast<Window*>(child)))
            ++count;
    }
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
                Window* child = getChildAtCell(x, y);
                if (!isDummy(child))
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
        if (isDummy(static_cast<Window*>(getChildAtIndex(i))))
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
        if (!isDummy(static_cast<Window*>(getChildAtIndex(i))))
        {
            lastIdx = i;
            found = true;
        }
    }

    return found ? lastIdx : std::numeric_limits<size_t>().max();
}

//----------------------------------------------------------------------------//
size_t GridLayoutContainer::getFirstFreeAutoPositioningIndex(size_t start) const
{
    for (size_t i = start; i < d_children.size(); ++i)
    {
        const size_t realIndex = translateAPToGridIdx(i);
        if (isDummy(static_cast<Window*>(getChildAtIndex(realIndex))))
            return i;
    }

    return std::numeric_limits<size_t>().max();
}

//----------------------------------------------------------------------------//
size_t GridLayoutContainer::getLastBusyAutoPositioningIndex() const
{
    bool found = false;
    size_t lastIdx = 0;
    for (size_t i = 0; i < d_children.size(); ++i)
    {
        const size_t realIndex = translateAPToGridIdx(i);
        if (!isDummy(static_cast<Window*>(getChildAtIndex(realIndex))))
        {
            lastIdx = i;
            found = true;
        }
    }

    return found ? lastIdx : std::numeric_limits<size_t>().max();
}

//----------------------------------------------------------------------------//
void GridLayoutContainer::addChildToCell(Window* window,
                                             size_t gridX, size_t gridY)
{
    // when user starts to add windows to specific locations, AO has to be disabled
    setAutoPositioning(AutoPositioning::Disabled);
    d_nextGridX = gridX;
    d_nextGridY = gridY;

    LayoutContainer::addChild(window);
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
    removeChild(getChildAtCell(gridX, gridY));
}

//----------------------------------------------------------------------------//
void GridLayoutContainer::swapChildCells(size_t gridX1, size_t gridY1,
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
void GridLayoutContainer::moveChildToCell(const String& wnd, size_t gridX, size_t gridY)
{
    moveChildToCell(getChild(wnd), gridX, gridY);
}

//----------------------------------------------------------------------------//
void GridLayoutContainer::layout()
{
    std::vector<UDim> colSizes(d_gridWidth, UDim(0, 0));
    std::vector<UDim> rowSizes(d_gridHeight, UDim(0, 0));

    // used to compare UDims
    const float absWidth = getChildContentArea().get().getWidth();
    const float absHeight = getChildContentArea().get().getHeight();

    // first, we need to determine rowSizes and colSizes, this is needed before
    // any layouting work takes place
    for (size_t y = 0; y < d_gridHeight; ++y)
    {
        for (size_t x = 0; x < d_gridWidth; ++x)
        {
            // x and y is the position of window in the grid
            Window* window = getChildAtCell(x, y);
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

    // OK, now in rowSizes[y] is the height of y-th row
    //         in colSizes[x] is the width of x-th column

    // second layouting phase starts now
    for (size_t y = 0; y < d_gridHeight; ++y)
    {
        for (size_t x = 0; x < d_gridWidth; ++x)
        {
            // x and y is the position of window in the grid
            Window* window = getChildAtCell(x, y);
            const UVector2 offset = getOffsetForWindow(window);
            const UVector2 gridCellOffset = getGridCellOffset(colSizes,
                                                              rowSizes,
                                                              x, y);

            window->setPosition(gridCellOffset + offset);
        }
    }

    // now we just need to determine the total width and height and set it
    setSize(getGridSize(colSizes, rowSizes));
}

//! converts from grid cell position to idx
size_t GridLayoutContainer::mapCellToIndex(size_t gridX, size_t gridY) const
{
    return cellToIndex(gridX, gridY, d_gridWidth, d_gridHeight);
}

//! converts from idx to grid cell position
void GridLayoutContainer::mapIndexToCell(size_t idx, size_t& gridX, size_t& gridY) const
{
    return indexToCell(idx, gridX, gridY, d_gridWidth, d_gridHeight);
}

//----------------------------------------------------------------------------//
UVector2 GridLayoutContainer::getGridCellOffset(
        const std::vector<UDim>& colSizes,
        const std::vector<UDim>& rowSizes,
        size_t gridX, size_t gridY) const
{
    assert(gridX < d_gridWidth);
    assert(gridY < d_gridHeight);

    UVector2 ret(UDim(0, 0), UDim(0, 0));

    for (size_t i = 0; i < gridX; ++i)
    {
        ret.d_x += colSizes[i];
    }

    for (size_t i = 0; i < gridY; ++i)
    {
        ret.d_y += rowSizes[i];
    }

    return ret;
}

//----------------------------------------------------------------------------//
USize GridLayoutContainer::getGridSize(const std::vector<UDim>& colSizes,
        const std::vector<UDim>& rowSizes) const
{
    USize ret(UDim(0, 0), UDim(0, 0));

    for (size_t i = 0; i < colSizes.size(); ++i)
    {
        ret.d_width += colSizes[i];
    }

    for (size_t i = 0; i < rowSizes.size(); ++i)
    {
        ret.d_height += rowSizes[i];
    }

    return ret;
}

//----------------------------------------------------------------------------//
size_t GridLayoutContainer::translateAPToGridIdx(size_t APIdx) const
{
    if (d_autoPositioning == AutoPositioning::Disabled ||
        d_autoPositioning == AutoPositioning::LeftToRight)
    {
        // this is the same positioning as implementation
        return APIdx;
    }
    else if (d_autoPositioning == AutoPositioning::TopToBottom)
    {
        // we want
        // 0 2 4
        // 1 3 5

        size_t x = 0;
        size_t y = 0;
        indexToCell(APIdx, y, x, d_gridHeight, d_gridWidth);
        return mapCellToIndex(x, y);
    }

    // should never happen
    assert(0);
    return APIdx;
}

//----------------------------------------------------------------------------//
Window* GridLayoutContainer::createDummy()
{
    std::stringstream& sstream = SharedStringstream::GetPreparedStream();
    sstream << d_nextDummyIdx;

    ++d_nextDummyIdx;

    Window* dummy = WindowManager::getSingleton().createWindow("DefaultWindow",
                    DummyName + sstream.str());

    dummy->setAutoWindow(true);
    dummy->setVisible(false);
    dummy->setSize(USize(UDim(0, 0), UDim(0, 0)));
    dummy->setDestroyedByParent(true);

    return dummy;
}

//----------------------------------------------------------------------------//
bool GridLayoutContainer::isDummy(Window* wnd) const
{
    // all auto windows inside grid are dummies
    return wnd && wnd->isAutoWindow();
}

//----------------------------------------------------------------------------//
void GridLayoutContainer::addChild_impl(Element* element)
{
    if (!d_gridWidth || !d_gridHeight)
    {
        throw InvalidRequestException(
            "GridLayoutContainer must have non-zero width and height to accept children.");
    }

    Window* wnd = dynamic_cast<Window*>(element);
    
    if (!wnd)
    {
        throw InvalidRequestException(
            "GridLayoutContainer can only have Elements of type Window added "
            "as children (Window path: " + getNamePath() + ").");
    }

    if (isDummy(wnd))
    {
        LayoutContainer::addChild_impl(wnd);
    }
    else
    {
        LayoutContainer::addChild_impl(wnd);

        // OK, wnd is already in d_children

        // idx is the future index of the child that's being added
        size_t idx;

        if (d_autoPositioning == AutoPositioning::Disabled)
        {
            if ((d_nextGridX == std::numeric_limits<size_t>::max()) &&
                (d_nextGridY == std::numeric_limits<size_t>::max()))
            {
                throw InvalidRequestException(
                    "Unable to add child without explicit grid position "
                    "because auto positioning is disabled.  Consider using the "
                    "GridLayoutContainer::addChildToCell function.");
            }

            idx = mapCellToIndex(d_nextGridX, d_nextGridY);

            // reset location to sentinel values.
            d_nextGridX = d_nextGridY = std::numeric_limits<size_t>::max();
        }
        else
        {
            idx = translateAPToGridIdx(d_nextAutoPositioningIdx);
            ++d_nextAutoPositioningIdx;
        }

        // we swap the dummy and the added child
        // this essentially places the added child to it's right position and
        // puts the dummy at the end of d_children it will soon get removed from
        std::swap(d_children[idx], d_children[d_children.size() - 1]);

        Window* toBeRemoved = static_cast<Window*>(d_children[d_children.size() - 1]);
        removeChild(toBeRemoved);

        if (toBeRemoved->isDestroyedByParent())
        {
            WindowManager::getSingleton().destroyWindow(toBeRemoved);
        }
    }
}

//----------------------------------------------------------------------------//
void GridLayoutContainer::removeChild_impl(Element* element)
{
    Window* wnd = static_cast<Window*>(element);
    
    if (!isDummy(wnd) && !WindowManager::getSingleton().isLocked())
    {
        // before we remove the child, we must add new dummy and place it
        // instead of the removed child
        Window* dummy = createDummy();
        addChild(dummy);

        const size_t i = getChildIndex(wnd);
        if (i < d_children.size() - 1)
            std::swap(d_children[i], d_children[d_children.size() - 1]);
    }

    LayoutContainer::removeChild_impl(wnd);
}

//----------------------------------------------------------------------------//
void GridLayoutContainer::addGridLayoutContainerProperties(void)
{
    const String& propertyOrigin = WidgetTypeName;

    CEGUI_DEFINE_PROPERTY(GridLayoutContainer, size_t,
        "GridWidth",
        "Width of the grid of this layout container. "
        "Value is an unsigned integer number.",
        &GridLayoutContainer::setGridWidth, &GridLayoutContainer::getGridWidth, 0
    );

    CEGUI_DEFINE_PROPERTY(GridLayoutContainer, size_t,
        "GridHeight",
        "Height of the grid of this layout container. "
        "Value is an unsigned integer number.",
        &GridLayoutContainer::setGridHeight, &GridLayoutContainer::getGridHeight, 0
    );

    CEGUI_DEFINE_PROPERTY(GridLayoutContainer, AutoPositioning,
        "AutoPositioning", "Sets the method used for auto positioning. "
        "Possible values: 'Disabled', 'Left to Right', 'Top to Bottom'.",
        &GridLayoutContainer::setAutoPositioning, &GridLayoutContainer::getAutoPositioning, GridLayoutContainer::AutoPositioning::LeftToRight
    );

}

//----------------------------------------------------------------------------//

} // End of  CEGUI namespace section