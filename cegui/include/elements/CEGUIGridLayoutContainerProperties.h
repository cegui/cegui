/***********************************************************************
    filename:   CEGUIGridLayoutContainerProperties.h
    created:    5/8/2010
    author:     Martin Preisler

    purpose:    Interface to GridLayoutContainer property classes
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
#ifndef _CEGUIGridLayoutContainerProperties_h_
#define _CEGUIGridLayoutContainerProperties_h_

#include "../CEGUIProperty.h"


// Start of CEGUI namespace section
namespace CEGUI
{

// Start of GridLayoutContainerProperties namespace section
/*!
\brief
    Namespace containing all classes that make up the properties interface for
    the GridLayoutContainer class
*/
namespace GridLayoutContainerProperties
{

class GridSize : public Property
{
public:
    GridSize() : Property(
                "GridSize",
                "Size of the grid of this layout container. "
                "Value uses the 'w:# h:#' format and will be rounded up because "
                "only integer values are valid as grid size.")
    {}

    String get(const PropertyReceiver* receiver) const;

    void set(PropertyReceiver* receiver, const String& value);
};

class AutoPositioning : public Property
{
public:
    AutoPositioning() : Property(
                "AutoPositioning",
                "Sets the method used for auto positioning. "
                "Possible values: 'Disabled', 'Left to Right', 'Top to Bottom'.")
    {}

    String get(const PropertyReceiver* receiver) const;

    void set(PropertyReceiver* receiver, const String& value);
};

} // End of  GridLayoutContainerProperties namespace section

} // End of  CEGUI namespace section

#endif  // end of guard _CEGUIGridLayoutContainerProperties_h_

