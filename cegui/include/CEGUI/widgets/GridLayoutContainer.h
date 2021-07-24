/***********************************************************************
    created:    01/8/2010
    author:     Martin Preisler

    purpose:    Interface to a vertical layout container
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
#ifndef _CEGUIGridLayoutContainer_h_
#define _CEGUIGridLayoutContainer_h_

#include "LayoutContainer.h"

namespace CEGUI
{
/*!
\brief
    A Layout Container window layouting it's children into a grid
*/
class CEGUIEXPORT GridLayoutContainer : public LayoutContainer
{
public:
    //! The unique typename of this widget
    static const String WidgetTypeName;

    //! Namespace for global events
    static const String EventNamespace;

    //! Widget name for dummies.
    static const String DummyName;

    GridLayoutContainer(const String& type, const String& name);

    /*!
    \brief
        Sets both grid dimensions at once. New cells are created empty.
        Items that don't fit into a new grid are removed from its children.
    */
    void setGridDimensions(size_t width, size_t height);

    /*!
    \brief
        Sets grid width.

    \see GridLayoutContainer::setGridDimensions(size_t width, size_t height)
    */
    void setGridWidth(size_t width);

    /*!
    \brief
        Sets grid height.

    \see GridLayoutContainer::setGridDimensions(size_t width, size_t height)
    */
    void setGridHeight(size_t height);

    /*!
    \brief
        Retrieves grid width, the amount of cells in one row
    */
    size_t getGridWidth() const { return d_gridWidth; }

    /*!
    \brief
        Retrieves grid height, the amount of rows in the grid
    */
    size_t getGridHeight() const { return d_gridHeight; }

    /*!
    \brief
        Sets majority of the grid layout (columns or rows).

    \par
        true for row-major layout, false for column-major.
    */
    void setRowMajor(bool rowMajor);

    /*!
    \brief
        Retrieves whether grid layout is row-major currently.
    */
    bool isRowMajor() const { return d_rowMajor; }

    /*!
    \brief
        Enables or disables auto-growing of the grid when its capacity is exhausted.
    */
    void setAutoGrowing(bool enabled);

    /*!
    \brief
        Retrieves whether the grid will grow automatically when its capacity is exhausted.
    */
    bool isAutoGrowing() const { return d_autoGrow; }

    /*!
    \brief
        Returns an actual layout child count not including auxiliary items (dummy placeholders)
    */
    virtual size_t getActualChildCount() const override;

    /*!
    \brief
        Returns minimal size in cells enough to keep all real children
    */
    void getMinimalSizeInCells(size_t& width, size_t& height) const;

    /*!
    \brief
        Returns the first free index in a grid or an invalid index (>= child count).
    */
    size_t getFirstFreeIndex(size_t start = 0) const;

    /*!
    \brief
        Returns the last busy index in a grid or an invalid index (>= child count).
    */
    size_t getLastBusyIndex() const;
    
    // Overridden to provide more appropriate implementation for the grid
    void addChildAtIndex(Element* element, size_t index) override;

    // Overridden to provide more appropriate implementation for the grid
    void removeChildAtIndex(size_t index) override;

    // Overridden to support auto-growing
    void moveChildToIndex(size_t indexFrom, size_t indexTo) override;

    /*!
    \brief
        Add the specified Window to specified grid position as a child of
        this Grid Layout Container. If the Window \a window already
        has parent, it is detached before being added to this Window.

    \par
        If something is already in given grid cell, it gets removed!

    \see
        Element::addChild
    */
    void addChildToCell(Window* window, size_t gridX, size_t gridY, bool replace = false);

    /*!
    \brief
        Removes the child window that is currently at given grid position

    \see
        Element::removeChild
    */
    void removeChildFromCell(size_t gridX, size_t gridY);

    /*!
    \brief
        Moves given child window to given grid position, shifting
        all children between the old and the new positions.
    */
    void moveChildToCell(Window* wnd, size_t gridX, size_t gridY);

    /*!
    \brief
        Swaps positions of 2 windows given by grid positions
    */
    void swapCells(size_t gridX1, size_t gridY1,
                   size_t gridX2, size_t gridY2);

    /*!
    \brief
        Retrieves child window that is currently at given grid position
    */
    Window* getChildAtCell(size_t gridX, size_t gridY) const;

    void onChildOrderChanged(ElementEventArgs& e) override;

    //! converts from grid cell position to idx
    size_t mapCellToIndex(size_t gridX, size_t gridY) const;
    //! converts from idx to grid cell position
    void mapIndexToCell(size_t idx, size_t& gridX, size_t& gridY) const;

protected:

    Window* createDummy();
    void validateGridCell(size_t gridX, size_t gridY);
    void growByOneLine();

    void endInitialisation() override;
    void addChild_impl(Element* element) override;
    void removeChild_impl(Element* element) override;
    Window* getChildAutoWindow(const String& name) override;
    int writeChildWindowsXML(XMLSerializer& xml_stream) const override;

    virtual void layout_impl() override;

    size_t d_gridWidth = 0;
    size_t d_gridHeight = 0;
    
    size_t d_requestedChildIdx;
    size_t d_freeSearchStart = 0; // Free cell search optimization
    
    size_t d_nextDummyIdx = 0;
    
    bool d_rowMajor = true;
    bool d_autoGrow = false;

private:

    void addGridLayoutContainerProperties();
};

}

#endif
