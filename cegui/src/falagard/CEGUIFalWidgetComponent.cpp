/***********************************************************************
    filename:   CEGUIFalWidgetComponent.cpp
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
#include "falagard/CEGUIFalWidgetComponent.h"
#include "falagard/CEGUIFalXMLEnumHelper.h"
#include "CEGUIWindowManager.h"
#include "CEGUIExceptions.h"
#include <iostream>

// Start of CEGUI namespace section
namespace CEGUI
{
    WidgetComponent::WidgetComponent(const String& type, const String& look, const String& suffix, const String& renderer) :
        d_baseType(type),
        d_imageryName(look),
        d_nameSuffix(suffix),
        d_rendererType(renderer),
        d_vertAlign(VA_TOP),
        d_horzAlign(HA_LEFT)
    {}

    void WidgetComponent::create(Window& parent) const
    {
        // build final name and create widget.
        String widgetName = parent.getName() + d_nameSuffix;
        Window* widget = WindowManager::getSingleton().createWindow(d_baseType, widgetName);

        // set the window renderer
        if (!d_rendererType.empty())
            widget->setWindowRenderer(d_rendererType);

        // set the widget look
        if (!d_imageryName.empty())
            widget->setLookNFeel(d_imageryName);

        // add the new widget to its parent
        parent.addChildWindow(widget);

        // set alignment options
        widget->setVerticalAlignment(d_vertAlign);
        widget->setHorizontalAlignment(d_horzAlign);

        // TODO: We still need code to specify position and size.  Due to the advanced
        // TODO: possibilities, this is better handled via a 'layout' method instead of
        // TODO: setting this once and forgetting about it.

        // initialise properties.  This is done last so that these properties can
        // override properties specified in the look assigned to the created widget.
        for(PropertiesList::const_iterator curr = d_properties.begin(); curr != d_properties.end(); ++curr)
        {
            (*curr).apply(*widget);
        }
    }

    const ComponentArea& WidgetComponent::getComponentArea() const
    {
        return d_area;
    }

    void WidgetComponent::setComponentArea(const ComponentArea& area)
    {
        d_area = area;
    }

    const String& WidgetComponent::getBaseWidgetType() const
    {
        return d_baseType;
    }

    void WidgetComponent::setBaseWidgetType(const String& type)
    {
        d_baseType = type;
    }

    const String& WidgetComponent::getWidgetLookName() const
    {
        return d_imageryName;
    }

    void WidgetComponent::setWidgetLookName(const String& look)
    {
        d_imageryName = look;
    }

    const String& WidgetComponent::getWidgetNameSuffix() const
    {
        return d_nameSuffix;
    }

    void WidgetComponent::setWidgetNameSuffix(const String& suffix)
    {
        d_nameSuffix = suffix;
    }

    const String& WidgetComponent::getWindowRendererType() const
    {
        return d_rendererType;
    }

    void WidgetComponent::setWindowRendererType(const String& type)
    {
        d_rendererType = type;
    }

    VerticalAlignment WidgetComponent::getVerticalWidgetAlignment() const
    {
        return d_vertAlign;
    }

    void WidgetComponent::setVerticalWidgetAlignment(VerticalAlignment alignment)
    {
        d_vertAlign = alignment;
    }

    HorizontalAlignment WidgetComponent::getHorizontalWidgetAlignment() const
    {
        return d_horzAlign;
    }

    void WidgetComponent::setHorizontalWidgetAlignment(HorizontalAlignment alignment)
    {
        d_horzAlign = alignment;
    }

    void WidgetComponent::addPropertyInitialiser(const PropertyInitialiser& initialiser)
    {
        d_properties.push_back(initialiser);
    }

    void WidgetComponent::clearPropertyInitialisers()
    {
        d_properties.clear();
    }

    void WidgetComponent::layout(const Window& owner) const
    {
        try
        {
            Rect pixelArea(d_area.getPixelRect(owner));
            URect window_area(cegui_absdim(pixelArea.d_left),
                              cegui_absdim(pixelArea.d_top),
                              cegui_absdim(pixelArea.d_right),
                              cegui_absdim(pixelArea.d_bottom));

            Window* wnd = WindowManager::getSingleton().getWindow(owner.getName() + d_nameSuffix);
            wnd->setArea(window_area);
            wnd->notifyScreenAreaChanged();
        }
        catch (UnknownObjectException&)
        {}
    }

    void WidgetComponent::writeXMLToStream(XMLSerializer& xml_stream) const
    {
        // output opening tag
        xml_stream.openTag("Child")
            .attribute("type", d_baseType)
            .attribute("nameSuffix", d_nameSuffix);

        if (!d_imageryName.empty())
            xml_stream.attribute("look", d_imageryName);

        if (!d_rendererType.empty())
            xml_stream.attribute("renderer", d_rendererType);


        // output target area
        d_area.writeXMLToStream(xml_stream);

        // output vertical alignment
        xml_stream.openTag("VertAlignment")
            .attribute("type", FalagardXMLHelper::vertAlignmentToString(d_vertAlign))
            .closeTag();

        // output horizontal alignment
        xml_stream.openTag("HorzAlignment")
            .attribute("type", FalagardXMLHelper::horzAlignmentToString(d_horzAlign))
            .closeTag();

        //output property initialisers
        for (PropertiesList::const_iterator prop = d_properties.begin(); prop != d_properties.end(); ++prop)
        {
            (*prop).writeXMLToStream(xml_stream);
        }

        // output closing tag
        xml_stream.closeTag();
    }

    const PropertyInitialiser* WidgetComponent::findPropertyInitialiser(const String& propertyName) const
    {
        PropertiesList::const_reverse_iterator i = d_properties.rbegin();
        while (i != d_properties.rend())
        {
            if ((*i).getTargetPropertyName() == propertyName)
                return &(*i);
            ++i;
        }
        return 0;
    }

} // End of  CEGUI namespace section
