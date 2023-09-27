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
#include "CEGUI/falagard/WidgetComponent.h"
#include "CEGUI/falagard/XMLEnumHelper.h"
#include "CEGUI/falagard/XMLHandler.h"
#include "CEGUI/Window.h"
#include "CEGUI/WindowManager.h"
#include "CEGUI/XMLSerializer.h"
#include <algorithm>

namespace CEGUI
{

    //! Default values
    const HorizontalAlignment WidgetComponent::HorizontalAlignmentDefault(HorizontalAlignment::Left);
    const VerticalAlignment WidgetComponent::VerticalAlignmentDefault(VerticalAlignment::Top);


    WidgetComponent::WidgetComponent(const String& targetType,
                                        const String& suffix,
                                        const String& renderer,
                                        bool autoWindow) :
        d_targetType(targetType),
        d_name(suffix),
        d_rendererType(renderer),
        d_autoWindow(autoWindow),
        d_vertAlign(VerticalAlignmentDefault),
        d_horzAlign(HorizontalAlignmentDefault)
    {}

    void WidgetComponent::create(Window& parent) const
    {
        Window* widget = WindowManager::getSingleton().createWindow(d_targetType, d_name);
        widget->setAutoWindow(d_autoWindow);

        // set the window renderer
        if (!d_rendererType.empty())
            widget->setWindowRenderer(d_rendererType);

        // add the new widget to its parent
        parent.addChild(widget);

        // set alignment options
        widget->setVerticalAlignment(d_vertAlign);
        widget->setHorizontalAlignment(d_horzAlign);

        // TODO: We still need code to specify position and size.  Due to the advanced
        // TODO: possibilities, this is better handled via a 'layout' method instead of
        // TODO: setting this once and forgetting about it.

        // initialise properties.  This is done last so that these properties can
        // override properties specified in the look assigned to the created widget.
        for(PropertyInitialiserList::const_iterator curr = d_propertyInitialisers.begin(); curr != d_propertyInitialisers.end(); ++curr)
        {
            (*curr).apply(*widget);
        }

        // link up the event actions
        for (EventActionList::const_iterator i = d_eventActions.begin();
                i != d_eventActions.end();
                ++i)
        {
            (*i).initialiseWidget(*widget);
        }
    }

    void WidgetComponent::cleanup(Window& parent) const
    {
        Window* widget = parent.findChild(getWidgetName());
        if (!widget)
            return;

        // clean up up the event actions
        for (EventActionList::const_iterator i = d_eventActions.begin();
                i != d_eventActions.end();
                ++i)
        {
            (*i).cleanupWidget(*widget);
        }

        parent.destroyChild(widget);
    }

    const ComponentArea& WidgetComponent::getComponentArea() const
    {
        return d_area;
    }

    void WidgetComponent::setComponentArea(const ComponentArea& area)
    {
        d_area = area;
    }

    const String& WidgetComponent::getTargetType() const
    {
        return d_targetType;
    }

    void WidgetComponent::setTargetType(const String& type)
    {
        d_targetType = type;
    }

    const String& WidgetComponent::getWidgetName() const
    {
        return d_name;
    }

    void WidgetComponent::setWidgetName(const String& name)
    {
        d_name= name;
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
        d_propertyInitialisers.push_back(initialiser);
    }

    void WidgetComponent::removePropertyInitialiser(const String& name)
    {
        PropertyInitialiserList::iterator f = d_propertyInitialisers.begin();
        PropertyInitialiserList::iterator const l = d_propertyInitialisers.end();
        // look for any removal candidate
        for(; f != l; ++f)
        {
            if(f->getTargetPropertyName() == name)
            {
                break;
            }
        }
        if(f == l)
        {
            // nothing to remove, so done
            return;
        }
        // start moving over any remaining items to keep
        for(PropertyInitialiserList::iterator i = f; ++i != l;)
        {
            if(i->getTargetPropertyName() != name)
            {
                std::iter_swap(f, i);
                ++f;
            }
        }
        d_propertyInitialisers.erase(f, l);
    }

    void WidgetComponent::clearPropertyInitialisers()
    {
        d_propertyInitialisers.clear();
    }

    void WidgetComponent::setAutoWindow(bool is_auto)
    {
        d_autoWindow = is_auto;
    }

    bool WidgetComponent::isAutoWindow() const
    {
        return d_autoWindow;
    }
        
    void WidgetComponent::addEventAction(const EventAction& event_action)
    {
        d_eventActions.push_back(event_action);
    }

    void WidgetComponent::clearEventActions()
    {
        d_eventActions.clear();
    }

    bool WidgetComponent::layout(const Window& owner) const
    {
        try
        {
            Window* child = owner.getChild(d_name);

            const Rectf pixelArea = d_area.getPixelRect(owner);

            // TODO: check equality inside Element::setArea?
            const URect& currArea = child->getArea();
            if (currArea.d_min.d_x.d_scale == 0.f &&
                currArea.d_min.d_y.d_scale == 0.f &&
                currArea.d_max.d_x.d_scale == 0.f &&
                currArea.d_max.d_y.d_scale == 0.f &&
                currArea.d_min.d_x.d_offset == pixelArea.left() &&
                currArea.d_min.d_y.d_offset == pixelArea.top() &&
                currArea.d_max.d_x.d_offset == pixelArea.right() &&
                currArea.d_max.d_y.d_offset == pixelArea.bottom())
            {
                return false;
            }

            child->setArea(
                cegui_absdim(pixelArea.left()),
                cegui_absdim(pixelArea.top()),
                cegui_absdim(pixelArea.getWidth()),
                cegui_absdim(pixelArea.getHeight()));
        }
        catch (UnknownObjectException&)
        {
            return false;
        }

        return true;
    }

    void WidgetComponent::writeXMLToStream(XMLSerializer& xml_stream) const
    {
        // output opening tag
        xml_stream.openTag(Falagard_xmlHandler::ChildElement)
            .attribute(Falagard_xmlHandler::NameSuffixAttribute, d_name)
            .attribute(Falagard_xmlHandler::TypeAttribute, d_targetType);

        if (!d_rendererType.empty())
            xml_stream.attribute(Falagard_xmlHandler::RendererAttribute, d_rendererType);

        if (!d_autoWindow)
            xml_stream.attribute(Falagard_xmlHandler::AutoWindowAttribute, PropertyHelper<bool>::ValueFalse);

        // Output <EventAction> elements
        for (EventActionList::const_iterator i = d_eventActions.begin();
                i != d_eventActions.end();
                ++i)
        {
            (*i).writeXMLToStream(xml_stream);
        }

        // output target area
        d_area.writeXMLToStream(xml_stream);

        // output vertical alignment if not-default
        if(d_vertAlign != VerticalAlignmentDefault)
        {
            xml_stream.openTag(Falagard_xmlHandler::VertAlignmentElement);
            xml_stream.attribute(Falagard_xmlHandler::TypeAttribute, FalagardXMLHelper<VerticalAlignment>::toString(d_vertAlign));
            xml_stream.closeTag();
        }

        // output horizontal alignment if not-default
        if(d_horzAlign != HorizontalAlignmentDefault)
        {
            xml_stream.openTag(Falagard_xmlHandler::HorzAlignmentElement);
            xml_stream.attribute(Falagard_xmlHandler::TypeAttribute, FalagardXMLHelper<HorizontalAlignment>::toString(d_horzAlign));
            xml_stream.closeTag();
        }

        //output property initialisers
        for (PropertyInitialiserList::const_iterator prop = d_propertyInitialisers.begin(); prop != d_propertyInitialisers.end(); ++prop)
        {
            (*prop).writeXMLToStream(xml_stream);
        }

        // output closing tag
        xml_stream.closeTag();
    }

    const PropertyInitialiser* WidgetComponent::findPropertyInitialiser(const String& propertyName) const
    {
        PropertyInitialiserList::const_reverse_iterator i = d_propertyInitialisers.rbegin();
        while (i != d_propertyInitialisers.rend())
        {
            if ((*i).getTargetPropertyName() == propertyName)
                return &(*i);
            ++i;
        }
        return nullptr;
    }

    bool WidgetComponent::handleFontRenderSizeChange(Window& window,
                                                        const Font* font) const
    {
        if (d_area.handleFontRenderSizeChange(window, font))
        {
            window.performChildLayout(false, false);
            return true;
        }

        return false;
    }

const WidgetComponent::PropertyInitialiserList& WidgetComponent::getPropertyInitialisers() const
{
    return d_propertyInitialisers;
}

const WidgetComponent::EventActionList& WidgetComponent::getEventActions() const
{
    return d_eventActions;
}

}
