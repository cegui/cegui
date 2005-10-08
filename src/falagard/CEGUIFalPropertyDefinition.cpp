/************************************************************************
    filename:   CEGUIFalPropertyDefinition.cpp
    created:    Sun Jun 26 2005
    author:     Paul D Turner <paul@cegui.org.uk>
*************************************************************************/
/*************************************************************************
    Crazy Eddie's GUI System (http://www.cegui.org.uk)
    Copyright (C)2004 - 2005 Paul D Turner (paul@cegui.org.uk)
 
    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.
 
    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.
 
    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*************************************************************************/
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

    void PropertyDefinition::writeXMLElementType(OutStream& out_stream) const
    {
        out_stream << "PropertyDefinition";
    }

} // End of  CEGUI namespace section
