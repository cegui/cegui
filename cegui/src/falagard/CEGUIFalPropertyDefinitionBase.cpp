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

    void PropertyDefinitionBase::writeXMLToStream(XMLSerializer& xml_stream) const
    {
        // write out the element type
        writeXMLElementType(xml_stream);
        // write attributes
        writeXMLAttributes(xml_stream);
        // close tag
        xml_stream.closeTag();
    }

    void PropertyDefinitionBase::writeXMLAttributes(XMLSerializer& xml_stream) const
    {
        // write the name of the property
        xml_stream.attribute("name", d_name);
        // write initial value, if any
        if (!d_default.empty())
            xml_stream.attribute("initialValue", d_default);
        // write option to redraw when property is written
        if (d_writeCausesRedraw)
            xml_stream.attribute("redrawOnWrite", "true");
        // write option to loayout children when property is written
        if (d_writeCausesLayout)
            xml_stream.attribute("layoutOnWrite", "true");
    }
} // End of  CEGUI namespace section
