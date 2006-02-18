/************************************************************************
    filename:   CEGUIFalPropertyDefinitionBase.cpp
    created:    Sat Oct 8 2005
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
#include "falagard/CEGUIFalPropertyDefinitionBase.h"
#include "CEGUIWindow.h"
#include <iostream>

// Start of CEGUI namespace section
namespace CEGUI
{
    PropertyDefinitionBase::PropertyDefinitionBase(const String& name,
                                                   const String& help,
                                                   const String& initialValue,
                                                   bool redrawOnWrite,
                                                   bool layoutOnWrite)

      : Property(name, help, initialValue),
        d_writeCausesRedraw(redrawOnWrite),
        d_writeCausesLayout(layoutOnWrite)
    {
    }

    void PropertyDefinitionBase::set(PropertyReceiver* receiver, const String& value)
    {
        if (d_writeCausesLayout)
            static_cast<Window*>(receiver)->performChildWindowLayout();

        if (d_writeCausesRedraw)
            static_cast<Window*>(receiver)->requestRedraw();
    }

    void PropertyDefinitionBase::writeXMLToStream(OutStream& out_stream, uint indentLevel) const
    {
        String indent(indentLevel, '\t');
        // open tag
        out_stream <<indent << "<";
        // write out the element type
        writeXMLElementType(out_stream);
        // ensure there a space between tag name and first attribute
        out_stream << " ";
        // write attributes
        writeXMLAttributes(out_stream);
        // close tag
        out_stream << "/>" << std::endl;
    }

    void PropertyDefinitionBase::writeXMLAttributes(OutStream& out_stream) const
    {
        // write the name of the property
        out_stream << "name=\"" << d_name << "\" ";
        // write initial value, if any
        if (!d_default.empty())
            out_stream << "initialValue=\"" << d_default << "\" ";
        // write option to redraw when property is written
        if (d_writeCausesRedraw)
            out_stream << "redrawOnWrite=\"true\" ";
        // write option to loayout children when property is written
        if (d_writeCausesLayout)
            out_stream << "layoutOnWrite=\"true\" ";
    }
} // End of  CEGUI namespace section
