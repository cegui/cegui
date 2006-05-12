/***********************************************************************
    filename:   CEGUIFalPropertyDefinitionBase.cpp
    created:    Sat Oct 8 2005
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
