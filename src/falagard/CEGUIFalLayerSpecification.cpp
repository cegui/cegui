/***********************************************************************
    filename:   CEGUIFalLayerSpecification.cpp
    created:    Mon Jun 13 2005
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
#include "falagard/CEGUIFalLayerSpecification.h"
#include <iostream>
#include "CEGUIPropertyHelper.h"
// Start of CEGUI namespace section
namespace CEGUI
{
    LayerSpecification::LayerSpecification(uint priority) :
        d_layerPriority(priority)
    {}

    void LayerSpecification::render(Window& srcWindow, float base_z, const ColourRect* modcols, const Rect* clipper, bool clipToDisplay) const
    {
        // render all sections in this layer
        for(SectionList::const_iterator curr = d_sections.begin(); curr != d_sections.end(); ++curr)
        {
            (*curr).render(srcWindow, base_z, modcols, clipper, clipToDisplay);
        }
    }

    void LayerSpecification::render(Window& srcWindow, const Rect& baseRect, float base_z, const ColourRect* modcols, const Rect* clipper, bool clipToDisplay) const
    {
        // render all sections in this layer
        for(SectionList::const_iterator curr = d_sections.begin(); curr != d_sections.end(); ++curr)
        {
            (*curr).render(srcWindow, baseRect, base_z, modcols, clipper, clipToDisplay);
        }
    }

    void LayerSpecification::addSectionSpecification(const SectionSpecification& section)
    {
        d_sections.push_back(section);
    }

    void LayerSpecification::clearSectionSpecifications()
    {
        d_sections.clear();
    }

    uint LayerSpecification::getLayerPriority() const
    {
        return d_layerPriority;
    }

    bool LayerSpecification::operator<(const LayerSpecification& other) const
    {
        return d_layerPriority < other.d_layerPriority;
    }

    void LayerSpecification::writeXMLToStream(XMLSerializer& xml_stream) const
    {
        xml_stream.openTag("Layer");

        if (d_layerPriority != 0)
            xml_stream.attribute("priority", PropertyHelper::uintToString(d_layerPriority));

        // ouput all sections in this layer
        for(SectionList::const_iterator curr = d_sections.begin(); curr != d_sections.end(); ++curr)
        {
            (*curr).writeXMLToStream(xml_stream);
        }

        xml_stream.closeTag();
    }

} // End of  CEGUI namespace section
