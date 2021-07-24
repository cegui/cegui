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
#include "CEGUI/widgets/VerticalLayoutContainer.h"
#include "CEGUI/CoordConverter.h"

namespace CEGUI
{
// type name for this widget
const String VerticalLayoutContainer::WidgetTypeName("VerticalLayoutContainer");

//----------------------------------------------------------------------------//
VerticalLayoutContainer::VerticalLayoutContainer(const String& type,
                                                 const String& name) :
    LayoutContainer(type, name)
{}

//----------------------------------------------------------------------------//
void VerticalLayoutContainer::layout_impl()
{
    // used to compare UDims
    const float absWidth = getChildContentArea().get().getWidth();

    // this is where we store the top offset
    // we continually increase this number as we go through the windows
    UDim topOffset(0, 0);
    UDim layoutWidth(0, 0);

    for (auto child : d_children)
    {
        Window* window = static_cast<Window*>(child);

        const UVector2 offset = getOffsetForWindow(window);
        window->setPosition(offset + UVector2(UDim(0, 0), topOffset));

        // setPosition triggered child pixel size recalculation, now we can get bounds
        const UVector2 boundingSize = getBoundingSizeForWindow(window);

        // full child window width, including margins
        const UDim& childWidth = boundingSize.d_x;

        if (CoordConverter::asAbsolute(layoutWidth, absWidth) <
            CoordConverter::asAbsolute(childWidth, absWidth))
        {
            layoutWidth = childWidth;
        }

        topOffset += boundingSize.d_y;
    }

    setSize(USize(layoutWidth, topOffset));
}
//----------------------------------------------------------------------------//

}
