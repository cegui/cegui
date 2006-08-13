/***********************************************************************
    filename:   CEGUIFalComponentBase.cpp
    created:    Mon Jul 18 2005
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
#include "falagard/CEGUIFalComponentBase.h"
#include "CEGUIExceptions.h"
#include "CEGUIPropertyHelper.h"
#include <iostream>

// Start of CEGUI namespace section
namespace CEGUI
{
    FalagardComponentBase::FalagardComponentBase() :
        d_colours(0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF),
        d_colourProperyIsRect(false)
    {}

    FalagardComponentBase::~ FalagardComponentBase()
    {}

    void FalagardComponentBase::render(Window& srcWindow, float base_z, const CEGUI::ColourRect* modColours, const Rect* clipper, bool clipToDisplay) const
    {
        Rect destRect(d_area.getPixelRect(srcWindow));
        render_impl(srcWindow, destRect, base_z, modColours, clipper, clipToDisplay);
    }

    void FalagardComponentBase::render(Window& srcWindow, const Rect& baseRect, float base_z, const CEGUI::ColourRect* modColours, const Rect* clipper, bool clipToDisplay) const
    {
        Rect destRect(d_area.getPixelRect(srcWindow, baseRect));
        render_impl(srcWindow, destRect, base_z, modColours, clipper, clipToDisplay);
    }

    const ComponentArea& FalagardComponentBase::getComponentArea() const
    {
        return d_area;
    }

    void FalagardComponentBase::setComponentArea(const ComponentArea& area)
    {
        d_area = area;
    }

    const ColourRect& FalagardComponentBase::getColours() const
    {
        return d_colours;
    }

    void FalagardComponentBase::setColours(const ColourRect& cols)
    {
        d_colours = cols;
    }

    void FalagardComponentBase::setColoursPropertySource(const String& property)
    {
        d_colourPropertyName = property;
    }

    void FalagardComponentBase::setColoursPropertyIsColourRect(bool setting)
    {
        d_colourProperyIsRect = setting;
    }

    void FalagardComponentBase::initColoursRect(const Window& wnd, const ColourRect* modCols, ColourRect& cr) const
    {
        // if colours come via a colour property
        if (!d_colourPropertyName.empty())
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
        // use explicit ColourRect.
        else
        {
            cr = d_colours;
        }

        if (modCols)
        {
            cr *= *modCols;
        }
    }


    void FalagardComponentBase::setVertFormattingPropertySource(const String& property)
    {
        d_vertFormatPropertyName = property;
    }

    void FalagardComponentBase::setHorzFormattingPropertySource(const String& property)
    {
        d_horzFormatPropertyName = property;
    }

    bool FalagardComponentBase::writeColoursXML(XMLSerializer& xml_stream) const
    {

        if (!d_colourPropertyName.empty())
        {
            if (d_colourProperyIsRect)
                xml_stream.openTag("ColourRectProperty");
            else
                xml_stream.openTag("ColourProperty");

            xml_stream.attribute("name", d_colourPropertyName)
                .closeTag();
        }
        else if (!d_colours.isMonochromatic() || d_colours.d_top_left != colour(1,1,1,1))
        {
            xml_stream.openTag("Colours")
                .attribute("topLeft", PropertyHelper::colourToString(d_colours.d_top_left))
                .attribute("topRight", PropertyHelper::colourToString(d_colours.d_top_right))
                .attribute("bottomLeft", PropertyHelper::colourToString(d_colours.d_bottom_left))
                .attribute("bottomRight", PropertyHelper::colourToString(d_colours.d_bottom_right))
                .closeTag();
        }
        else
        {
            return false;
        }
        return true;
    }

    bool FalagardComponentBase::writeVertFormatXML(XMLSerializer& xml_stream) const
    {
        if (!d_vertFormatPropertyName.empty())
        {
            xml_stream.openTag("VertFormatProperty")
                .attribute("name", d_vertFormatPropertyName)
                .closeTag();
            return true;
        }

        return false;
    }

    bool FalagardComponentBase::writeHorzFormatXML(XMLSerializer& xml_stream) const
    {
        if (!d_horzFormatPropertyName.empty())
        {
            xml_stream.openTag("HorzFormatProperty")
                .attribute("name", d_horzFormatPropertyName)
                .closeTag();
            return true;
        }

        return false;
    }

} // End of  CEGUI namespace section
