/***********************************************************************
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
#include "CEGUI/falagard/LayerSpecification.h"
#include "CEGUI/falagard/XMLHandler.h"
#include "CEGUI/XMLSerializer.h"

namespace CEGUI
{

LayerSpecification::LayerSpecification(unsigned int priority) :
    d_layerPriority(priority)
{}

void LayerSpecification::render(Window& srcWindow, const ColourRect* modcols, const Rectf* clipper) const
{
    // render all sections in this layer
    for (const auto& curr : d_sections)
        curr.render(srcWindow, modcols, clipper);
}

void LayerSpecification::render(Window& srcWindow, const Rectf& baseRect, const ColourRect* modcols, const Rectf* clipper) const
{
    // render all sections in this layer
    for (const auto& curr : d_sections)
        curr.render(srcWindow, baseRect, modcols, clipper);
}

void LayerSpecification::addSectionSpecification(const SectionSpecification& section)
{
    d_sections.push_back(section);
}

void LayerSpecification::clearSectionSpecifications()
{
    d_sections.clear();
}

unsigned int LayerSpecification::getLayerPriority() const
{
    return d_layerPriority;
}

void LayerSpecification::setLayerPriority(unsigned int priority)
{
    d_layerPriority = priority;
}

void LayerSpecification::writeXMLToStream(XMLSerializer& xml_stream) const
{
    xml_stream.openTag(Falagard_xmlHandler::LayerElement);

    if (d_layerPriority != 0)
        xml_stream.attribute(Falagard_xmlHandler::PriorityAttribute, PropertyHelper<std::uint32_t>::toString(d_layerPriority));

    // ouput all sections in this layer
    for(SectionSpecificationList::const_iterator curr = d_sections.begin(); curr != d_sections.end(); ++curr)
    {
        (*curr).writeXMLToStream(xml_stream);
    }

    xml_stream.closeTag();
}

LayerSpecification::SectionSpecificationPointerList LayerSpecification::getSectionSpecificationPointers()
{
    LayerSpecification::SectionSpecificationPointerList pointerList;

    SectionSpecificationList::iterator sectionSpecificationIter = d_sections.begin();
    SectionSpecificationList::iterator sectionSpecificationEnd = d_sections.end();
    while( sectionSpecificationIter != sectionSpecificationEnd )
    {
        pointerList.push_back(&(*sectionSpecificationIter));
        ++sectionSpecificationIter;
    }

    return pointerList;
}

const LayerSpecification::SectionSpecificationList& LayerSpecification::getSectionSpecifications() const
{
    return d_sections;
}

bool LayerSpecification::operator< (const LayerSpecification& otherLayerSpec) const
{
    return d_layerPriority < otherLayerSpec.getLayerPriority();
}

} // End of  CEGUI namespace section
