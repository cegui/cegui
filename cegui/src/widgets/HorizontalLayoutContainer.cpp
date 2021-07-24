/***********************************************************************
    created:    29/7/2010
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
#include "CEGUI/widgets/HorizontalLayoutContainer.h"
#include "CEGUI/CoordConverter.h"

namespace CEGUI
{
// type name for this widget
const String HorizontalLayoutContainer::WidgetTypeName("HorizontalLayoutContainer");

//----------------------------------------------------------------------------//
HorizontalLayoutContainer::HorizontalLayoutContainer(const String& type,
                                                     const String& name) :
    LayoutContainer(type, name)
{}

//----------------------------------------------------------------------------//
void HorizontalLayoutContainer::layout_impl()
{
    // used to compare UDims
    const float absHeight = getChildContentArea().get().getHeight();

    // this is where we store the left offset
    // we continually increase this number as we go through the windows
    UDim leftOffset(0, 0);
    UDim layoutHeight(0, 0);

    for (auto child : d_children)
    {
        Window* window = static_cast<Window*>(child);

        const UVector2 offset = getOffsetForWindow(window);
        window->setPosition(offset + UVector2(leftOffset, UDim(0, 0)));
        
        // setPosition triggered child pixel size recalculation, now we can get bounds
        const UVector2 boundingSize = getBoundingSizeForWindow(window);

        // full child window height, including margins
        const UDim& childHeight = boundingSize.d_y;

        if (CoordConverter::asAbsolute(layoutHeight, absHeight) <
            CoordConverter::asAbsolute(childHeight, absHeight))
        {
            layoutHeight = childHeight;
        }

        leftOffset += boundingSize.d_x;
    }

    setSize(USize(leftOffset, layoutHeight));
}

//----------------------------------------------------------------------------//

}
