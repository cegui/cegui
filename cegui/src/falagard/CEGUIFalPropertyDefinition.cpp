/***********************************************************************
    filename:   CEGUIFalPropertyDefinition.cpp
    created:    Sun Jun 26 2005
    author:     Paul D Turner <paul@cegui.org.uk>
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
#include "falagard/CEGUIFalPropertyDefinition.h"
#include "CEGUIWindow.h"
#include <iostream>

// Start of CEGUI namespace section
namespace CEGUI
{
    PropertyDefinition::PropertyDefinition(const String& name,
                                           const String& initialValue,
                                           bool redrawOnWrite,
                                           bool layoutOnWrite)
      : PropertyDefinitionBase(name,
                               "Falagard custom property definition - gets/sets a named user string.",
                               initialValue, redrawOnWrite,
                               layoutOnWrite),
        d_userStringName(name + "_fal_auto_prop__")
    {
    }

    // abstract members from Property
    String PropertyDefinition::get(const PropertyReceiver* receiver) const
    {
        return static_cast<const Window*>(receiver)->getUserString(d_userStringName);
    }

    void PropertyDefinition::set(PropertyReceiver* receiver, const String& value)
    {
        static_cast<Window*>(receiver)->setUserString(d_userStringName, value);
        PropertyDefinitionBase::set(receiver, value);
    }

    void PropertyDefinition::writeXMLElementType(XMLSerializer& xml_stream) const
    {
        xml_stream.openTag("PropertyDefinition");
    }

} // End of  CEGUI namespace section
