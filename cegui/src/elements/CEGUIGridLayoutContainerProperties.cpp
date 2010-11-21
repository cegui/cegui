/***********************************************************************
    filename:   CEGUIGridLayoutContainerProperties.cpp
    created:    5/8/2010
    author:     Martin Preisler

    purpose:    Implements GridLayoutContainer Property classes
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2006 Paul D Turner & The CEGUI Development Team
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
#include "elements/CEGUIGridLayoutContainerProperties.h"
#include "elements/CEGUIGridLayoutContainer.h"
#include "CEGUIPropertyHelper.h"
#include <cmath>

// Start of CEGUI namespace section
namespace CEGUI
{

// Start of GridLayoutContainerProperties namespace section
namespace GridLayoutContainerProperties
{
//----------------------------------------------------------------------------//
String GridSize::get(const PropertyReceiver* receiver) const
{
    const GridLayoutContainer* grid =
        static_cast<const GridLayoutContainer*>(receiver);

    const Size size(static_cast<float>(grid->getGridWidth()),
                    static_cast<float>(grid->getGridHeight()));

    return PropertyHelper<Size>::toString(size);
}

//----------------------------------------------------------------------------//
void GridSize::set(PropertyReceiver* receiver, const String& value)
{
    GridLayoutContainer* grid = static_cast<GridLayoutContainer*>(receiver);
    Size size = PropertyHelper<Size>::fromString(value);
    size.d_width = std::max(0.0f, size.d_width);
    size.d_height = std::max(0.0f, size.d_height);

    grid->setGridDimensions(static_cast<size_t>(ceil(size.d_width)),
                            static_cast<size_t>(ceil(size.d_height)));
}

//----------------------------------------------------------------------------//
String AutoPositioning::get(const PropertyReceiver* receiver) const
{
    const GridLayoutContainer* grid =
        static_cast<const GridLayoutContainer*>(receiver);

    if (grid->getAutoPositioning() == GridLayoutContainer::AP_Disabled)
    {
        return "Disabled";
    }
    else if (grid->getAutoPositioning() == GridLayoutContainer::AP_LeftToRight)
    {
        return "Left to Right";
    }
    else if (grid->getAutoPositioning() == GridLayoutContainer::AP_TopToBottom)
    {
        return "Top to Bottom";
    }
    else
    {
        assert(0);
        return "";
    }
}

//----------------------------------------------------------------------------//
void AutoPositioning::set(PropertyReceiver* receiver, const String& value)
{
    GridLayoutContainer* grid = static_cast<GridLayoutContainer*>(receiver);

    if (value == "Disabled")
    {
        grid->setAutoPositioning(GridLayoutContainer::AP_Disabled);
    }
    else if (value == "Left to Right")
    {
        grid->setAutoPositioning(GridLayoutContainer::AP_LeftToRight);
    }
    else if (value == "Top to Bottom")
    {
        grid->setAutoPositioning(GridLayoutContainer::AP_TopToBottom);
    }
    else
    {
        // sensible default
        grid->setAutoPositioning(GridLayoutContainer::AP_LeftToRight);
    }
}

//----------------------------------------------------------------------------//

} // End of  GridLayoutContainerProperties namespace section

} // End of  CEGUI namespace section

