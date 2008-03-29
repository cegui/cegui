/***********************************************************************
    filename:   CEGUIFalStateImagery.cpp
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
#include "falagard/CEGUIFalStateImagery.h"
#include "CEGUISystem.h"
#include "CEGUIRenderer.h"
#include <iostream>

// Start of CEGUI namespace section
namespace CEGUI
{
    StateImagery::StateImagery(const String& name) :
        d_stateName(name),
        d_clipToDisplay(false)
    {}

    void StateImagery::render(Window& srcWindow, const ColourRect* modcols, const Rect* clipper) const
    {
        float base_z;

        // render all layers defined for this state
        for(LayersList::const_iterator curr = d_layers.begin(); curr != d_layers.end(); ++curr)
        {
            // TODO: Magic number removal
            base_z = -0.0000001f * static_cast<float>((*curr).getLayerPriority());
            (*curr).render(srcWindow, base_z, modcols, clipper, d_clipToDisplay);
        }
    }

    void StateImagery::render(Window& srcWindow, const Rect& baseRect, const ColourRect* modcols, const Rect* clipper) const
    {
        float base_z;

        // render all layers defined for this state
        for(LayersList::const_iterator curr = d_layers.begin(); curr != d_layers.end(); ++curr)
        {
            // TODO: Magic number removal
            base_z = -0.0000001f * static_cast<float>((*curr).getLayerPriority());
            (*curr).render(srcWindow, baseRect, base_z, modcols, clipper, d_clipToDisplay);
        }
    }

    void StateImagery::addLayer(const LayerSpecification& layer)
    {
        d_layers.insert(layer);
    }

    void StateImagery::clearLayers()
    {
        d_layers.clear();
    }

    const String& StateImagery::getName() const
    {
        return d_stateName;
    }

    bool StateImagery::isClippedToDisplay() const
    {
        return d_clipToDisplay;
    }

    void StateImagery::setClippedToDisplay(bool setting)
    {
        d_clipToDisplay = setting;
    }

    void StateImagery::writeXMLToStream(XMLSerializer& xml_stream) const
    {
        xml_stream.openTag("StateImagery")
            .attribute("name", d_stateName);

        if (d_clipToDisplay)
            xml_stream.attribute("clipped", "false");

        // output all layers defined for this state
        for(LayersList::const_iterator curr = d_layers.begin(); curr != d_layers.end(); ++curr)
            (*curr).writeXMLToStream(xml_stream);
        // write closing </StateImagery> tag
        xml_stream.closeTag();
    }

} // End of  CEGUI namespace section
