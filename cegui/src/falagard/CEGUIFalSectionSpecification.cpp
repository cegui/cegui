/***********************************************************************
    filename:   CEGUIFalSectionSpecification.cpp
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
#include "falagard/CEGUIFalSectionSpecification.h"
#include "falagard/CEGUIFalImagerySection.h"
#include "falagard/CEGUIFalWidgetLookFeel.h"
#include "falagard/CEGUIFalWidgetLookManager.h"
#include "CEGUIExceptions.h"
#include "CEGUIPropertyHelper.h"
#include <iostream>

// Start of CEGUI namespace section
namespace CEGUI
{
    SectionSpecification::SectionSpecification(const String& owner, const String& sectionName, const String& controlPropertySource) :
        d_owner(owner),
        d_sectionName(sectionName),
        d_usingColourOverride(false),
        d_colourProperyIsRect(false),
        d_renderControlProperty(controlPropertySource)
    {}

    SectionSpecification::SectionSpecification(const String& owner, const String& sectionName, const String& controlPropertySource, const ColourRect& cols) :
        d_owner(owner),
        d_sectionName(sectionName),
        d_coloursOverride(cols),
        d_usingColourOverride(true),
        d_colourProperyIsRect(false),
        d_renderControlProperty(controlPropertySource)
    {}

    void SectionSpecification::render(Window& srcWindow, float base_z, const ColourRect* modcols, const Rect* clipper, bool clipToDisplay) const
    {
        // see if we need to bother rendering
        if (d_renderControlProperty.empty() ||
            PropertyHelper::stringToBool(srcWindow.getProperty(d_renderControlProperty)))
        {
            try
            {
                // get the imagery section object with the name we're set up to use
                const ImagerySection* sect =
                    &WidgetLookManager::getSingleton().getWidgetLook(d_owner).getImagerySection(d_sectionName);

                // decide what colours are to be used
                ColourRect finalColours;
                initColourRectForOverride(srcWindow, finalColours);
                finalColours.modulateAlpha(srcWindow.getEffectiveAlpha());

                if (modcols)
                    finalColours *= *modcols;

                // render the imagery section
                sect->render(srcWindow, base_z, &finalColours, clipper, clipToDisplay);
            }
            // do nothing here, errors are non-faltal and are logged for debugging purposes.
            catch (Exception&)
            {}
        }
    }

    void SectionSpecification::render(Window& srcWindow, const Rect& baseRect, float base_z, const ColourRect* modcols, const Rect* clipper, bool clipToDisplay) const
    {
        try
        {
            // get the imagery section object with the name we're set up to use
            const ImagerySection* sect =
                &WidgetLookManager::getSingleton().getWidgetLook(d_owner).getImagerySection(d_sectionName);

            // decide what colours are to be used
            ColourRect finalColours;
            initColourRectForOverride(srcWindow, finalColours);
            finalColours.modulateAlpha(srcWindow.getEffectiveAlpha());

            if (modcols)
                finalColours *= *modcols;

            // render the imagery section
            sect->render(srcWindow, baseRect, base_z, &finalColours, clipper, clipToDisplay);
        }
        // do nothing here, errors are non-faltal and are logged for debugging purposes.
        catch (Exception&)
        {}
    }

    const String& SectionSpecification::getOwnerWidgetLookFeel() const
    {
        return d_owner;
    }

    const String& SectionSpecification::getSectionName() const
    {
        return d_sectionName;
    }

    const ColourRect& SectionSpecification::getOverrideColours() const
    {
        return d_coloursOverride;
    }

    void SectionSpecification::setOverrideColours(const ColourRect& cols)
    {
        d_coloursOverride = cols;
    }

    bool SectionSpecification::isUsingOverrideColours() const
    {
        return d_usingColourOverride;
    }

    void SectionSpecification::setUsingOverrideColours(bool setting)
    {
        d_usingColourOverride = setting;
    }

    void SectionSpecification::setOverrideColoursPropertySource(const String& property)
    {
        d_colourPropertyName = property;
    }

    void SectionSpecification::initColourRectForOverride(const Window& wnd, ColourRect& cr) const
    {
        // if no override set
        if (!d_usingColourOverride)
        {
            colour val(1,1,1,1);
            cr.d_top_left     = val;
            cr.d_top_right    = val;
            cr.d_bottom_left  = val;
            cr.d_bottom_right = val;
        }
        // if override comes via a colour property
        else if (!d_colourPropertyName.empty())
        {
            // if property accesses a ColourRect
            if (d_colourProperyIsRect)
            {
                cr = PropertyHelper::stringToColourRect(wnd.getProperty(d_colourPropertyName));
            }
            // property accesses a colour
            else
            {
                colour val(PropertyHelper::stringToColour(wnd.getProperty(d_colourPropertyName)));
                cr.d_top_left     = val;
                cr.d_top_right    = val;
                cr.d_bottom_left  = val;
                cr.d_bottom_right = val;
            }
        }
        // override is an explicitly defined ColourRect.
        else
        {
            cr = d_coloursOverride;
        }
    }

    void SectionSpecification::setOverrideColoursPropertyIsColourRect(bool setting)
    {
        d_colourProperyIsRect = setting;
    }

    void SectionSpecification::setRenderControlPropertySource(const String& property)
    {
        d_renderControlProperty = property;
    }

    void SectionSpecification::writeXMLToStream(XMLSerializer& xml_stream) const
    {
        xml_stream.openTag("Section");

        if (!d_owner.empty())
            xml_stream.attribute("look", d_owner);

        xml_stream.attribute("section", d_sectionName);

        // render controlling property name if needed
        if (!d_renderControlProperty.empty())
            xml_stream.attribute("controlProperty", d_renderControlProperty);

        if (d_usingColourOverride)
        {
            // output modulative colours for this section
            if (!d_colourPropertyName.empty())
            {
                if (d_colourProperyIsRect)
                    xml_stream.openTag("ColourRectProperty");
                else
                    xml_stream.openTag("ColourProperty");

                xml_stream.attribute("name", d_colourPropertyName)
                    .closeTag();
            }
            else if (!d_coloursOverride.isMonochromatic() || d_coloursOverride.d_top_left != colour(1,1,1,1))
            {
                xml_stream.openTag("Colours")
                    .attribute("topLeft", PropertyHelper::colourToString(d_coloursOverride.d_top_left))
                    .attribute("topRight", PropertyHelper::colourToString(d_coloursOverride.d_top_right))
                    .attribute("bottomLeft", PropertyHelper::colourToString(d_coloursOverride.d_bottom_left))
                    .attribute("bottomRight", PropertyHelper::colourToString(d_coloursOverride.d_bottom_right))
                    .closeTag();
            }

        }
        // close section element 
        xml_stream.closeTag();
    }

} // End of  CEGUI namespace section
