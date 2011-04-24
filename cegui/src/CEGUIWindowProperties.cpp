/***********************************************************************
	filename: 	CEGUIWindowProperties.cpp
	created:	5/7/2004
	author:		Paul D Turner
	
	purpose:	Implementation of available window base class properties
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
#include "CEGUIWindowProperties.h"
#include "CEGUIWindow.h"
#include "CEGUIFont.h"
#include "CEGUIPropertyHelper.h"
#include "CEGUIExceptions.h"
#include "CEGUIWindowFactoryManager.h"
#include "CEGUIImageManager.h"
#include "CEGUIImage.h"
#include <cstdlib>
#include <cstdio>

// Start of CEGUI namespace section
namespace CEGUI
{
// Start of WindowProperties namespace section
namespace WindowProperties
{

String Font::get(const PropertyReceiver* receiver) const
{
	const CEGUI::Font* fnt = static_cast<const Window*>(receiver)->getFont();

	if (fnt)
	{
		return fnt->getName();
	}
	else
	{
		return String();
	}

}


void Font::set(PropertyReceiver* receiver, const String& value)
{
	CEGUI_TRY
	{
		if (value.empty())
		{
			static_cast<Window*>(receiver)->setFont(0);
		}
		else
		{
			static_cast<Window*>(receiver)->setFont(value);
		}
	}
	CEGUI_CATCH (UnknownObjectException&)
	{ }
}

bool Font::isDefault(const PropertyReceiver* receiver) const
{
    return static_cast<const Window*>(receiver)->getFont(false) == 0;
}


String MouseCursorImage::get(const PropertyReceiver* receiver) const
{
	const Image* img = static_cast<const Window*>(receiver)->getMouseCursor();

    return img ? img->getName() : String();
}

void MouseCursorImage::set(PropertyReceiver* receiver, const String& value)
{
	if (!value.empty())
	{
		static_cast<Window*>(receiver)->setMouseCursor(
            &ImageManager::getSingleton().get(value));
	}
}

bool MouseCursorImage::isDefault(const PropertyReceiver* receiver) const
{
    return static_cast<const Window*>(receiver)->getMouseCursor(false) == 0;
}

String DestroyedByParent::get(const PropertyReceiver* receiver) const
{
	return PropertyHelper<bool>::toString(static_cast<const Window*>(receiver)->isDestroyedByParent());
}


void DestroyedByParent::set(PropertyReceiver* receiver, const String& value)
{
	static_cast<Window*>(receiver)->setDestroyedByParent(PropertyHelper<bool>::fromString(value));
}

String WantsMultiClickEvents::get(const PropertyReceiver* receiver) const
{
    return PropertyHelper<bool>::toString(static_cast<const Window*>(receiver)->wantsMultiClickEvents());
}


void WantsMultiClickEvents::set(PropertyReceiver* receiver, const String& value)
{
    static_cast<Window*>(receiver)->setWantsMultiClickEvents(PropertyHelper<bool>::fromString(value));
}


String MouseButtonDownAutoRepeat::get(const PropertyReceiver* receiver) const
{
    return PropertyHelper<bool>::toString(static_cast<const Window*>(receiver)->isMouseAutoRepeatEnabled());
}


void MouseButtonDownAutoRepeat::set(PropertyReceiver* receiver, const String& value)
{
    static_cast<Window*>(receiver)->setMouseAutoRepeatEnabled(PropertyHelper<bool>::fromString(value));
}


String AutoRepeatDelay::get(const PropertyReceiver* receiver) const
{
    return PropertyHelper<float>::toString(static_cast<const Window*>(receiver)->getAutoRepeatDelay());
}


void AutoRepeatDelay::set(PropertyReceiver* receiver, const String& value)
{
    static_cast<Window*>(receiver)->setAutoRepeatDelay(PropertyHelper<float>::fromString(value));
}


String AutoRepeatRate::get(const PropertyReceiver* receiver) const
{
    return PropertyHelper<float>::toString(static_cast<const Window*>(receiver)->getAutoRepeatRate());
}


void AutoRepeatRate::set(PropertyReceiver* receiver, const String& value)
{
    static_cast<Window*>(receiver)->setAutoRepeatRate(PropertyHelper<float>::fromString(value));
}


String DistributeCapturedInputs::get(const PropertyReceiver* receiver) const
{
    return PropertyHelper<bool>::toString(static_cast<const Window*>(receiver)->distributesCapturedInputs());
}


void DistributeCapturedInputs::set(PropertyReceiver* receiver, const String& value)
{
    static_cast<Window*>(receiver)->setDistributesCapturedInputs(PropertyHelper<bool>::fromString(value));
}


String CustomTooltipType::get(const PropertyReceiver* receiver) const
{
    return static_cast<const Window*>(receiver)->getTooltipType();
}


void CustomTooltipType::set(PropertyReceiver* receiver, const String& value)
{
    static_cast<Window*>(receiver)->setTooltipType(value);
}

String Tooltip::get(const PropertyReceiver* receiver) const
{
    const Window* wnd = static_cast<const Window*>(receiver);

    if (!wnd->getParent() || !wnd->inheritsTooltipText() || (wnd->getTooltipText() != wnd->getParent()->getTooltipText()))
    {
        return wnd->getTooltipText();
    }
    else
    {
        return String("");
    }
}


void Tooltip::set(PropertyReceiver* receiver, const String& value)
{
    static_cast<Window*>(receiver)->setTooltipText(value);
}


String InheritsTooltipText::get(const PropertyReceiver* receiver) const
{
    return PropertyHelper<bool>::toString(static_cast<const Window*>(receiver)->inheritsTooltipText());
}


void InheritsTooltipText::set(PropertyReceiver* receiver, const String& value)
{
    static_cast<Window*>(receiver)->setInheritsTooltipText(PropertyHelper<bool>::fromString(value));
}


String RiseOnClick::get(const PropertyReceiver* receiver) const
{
    return PropertyHelper<bool>::toString(static_cast<const Window*>(receiver)->isRiseOnClickEnabled());
}


void RiseOnClick::set(PropertyReceiver* receiver, const String& value)
{
    static_cast<Window*>(receiver)->setRiseOnClickEnabled(PropertyHelper<bool>::fromString(value));
}


String VerticalAlignment::get(const PropertyReceiver* receiver) const
{
    switch(static_cast<const Window*>(receiver)->getVerticalAlignment())
    {
        case VA_CENTRE:
            return String("Centre");
            break;

        case VA_BOTTOM:
            return String("Bottom");
            break;

        default:
            return String("Top");
    }
}

void VerticalAlignment::set(PropertyReceiver* receiver, const String& value)
{
    CEGUI::VerticalAlignment align;

    if (value == "Centre")
    {
        align = VA_CENTRE;
    }
    else if (value == "Bottom")
    {
        align = VA_BOTTOM;
    }
    else
    {
        align = VA_TOP;
    }
    
    static_cast<Window*>(receiver)->setVerticalAlignment(align);
}


String HorizontalAlignment::get(const PropertyReceiver* receiver) const
{
    switch(static_cast<const Window*>(receiver)->getHorizontalAlignment())
    {
        case HA_CENTRE:
            return String("Centre");
            break;

        case HA_RIGHT:
            return String("Right");
            break;

        default:
            return String("Left");
    }
}

void HorizontalAlignment::set(PropertyReceiver* receiver, const String& value)
{
    CEGUI::HorizontalAlignment align;

    if (value == "Centre")
    {
        align = HA_CENTRE;
    }
    else if (value == "Right")
    {
        align = HA_RIGHT;
    }
    else
    {
        align = HA_LEFT;
    }
    
    static_cast<Window*>(receiver)->setHorizontalAlignment(align);
}


String UnifiedAreaRect::get(const PropertyReceiver* receiver) const
{
    return PropertyHelper<URect>::toString(static_cast<const Window*>(receiver)->getArea());
}

void UnifiedAreaRect::set(PropertyReceiver* receiver, const String& value)
{
    static_cast<Window*>(receiver)->setArea(PropertyHelper<URect>::fromString(value));
}


String UnifiedPosition::get(const PropertyReceiver* receiver) const
{
    return PropertyHelper<UVector2>::toString(static_cast<const Window*>(receiver)->getPosition());
}

void UnifiedPosition::set(PropertyReceiver* receiver, const String& value)
{
    static_cast<Window*>(receiver)->setPosition(PropertyHelper<UVector2>::fromString(value));
}


String UnifiedXPosition::get(const PropertyReceiver* receiver) const
{
    return PropertyHelper<UDim>::toString(static_cast<const Window*>(receiver)->getXPosition());
}

void UnifiedXPosition::set(PropertyReceiver* receiver, const String& value)
{
    static_cast<Window*>(receiver)->setXPosition(PropertyHelper<UDim>::fromString(value));
}


String UnifiedYPosition::get(const PropertyReceiver* receiver) const
{
    return PropertyHelper<UDim>::toString(static_cast<const Window*>(receiver)->getYPosition());
}

void UnifiedYPosition::set(PropertyReceiver* receiver, const String& value)
{
    static_cast<Window*>(receiver)->setYPosition(PropertyHelper<UDim>::fromString(value));
}


String UnifiedSize::get(const PropertyReceiver* receiver) const
{
    return PropertyHelper<UVector2>::toString(static_cast<const Window*>(receiver)->getSize());
}

void UnifiedSize::set(PropertyReceiver* receiver, const String& value)
{
    static_cast<Window*>(receiver)->setSize(PropertyHelper<UVector2>::fromString(value));
}


String UnifiedWidth::get(const PropertyReceiver* receiver) const
{
    return PropertyHelper<UDim>::toString(static_cast<const Window*>(receiver)->getWidth());
}

void UnifiedWidth::set(PropertyReceiver* receiver, const String& value)
{
    static_cast<Window*>(receiver)->setWidth(PropertyHelper<UDim>::fromString(value));
}


String UnifiedHeight::get(const PropertyReceiver* receiver) const
{
    return PropertyHelper<UDim>::toString(static_cast<const Window*>(receiver)->getHeight());
}

void UnifiedHeight::set(PropertyReceiver* receiver, const String& value)
{
    static_cast<Window*>(receiver)->setHeight(PropertyHelper<UDim>::fromString(value));
}


String UnifiedMinSize::get(const PropertyReceiver* receiver) const
{
    return PropertyHelper<UVector2>::toString(static_cast<const Window*>(receiver)->getMinSize());
}

void UnifiedMinSize::set(PropertyReceiver* receiver, const String& value)
{
    static_cast<Window*>(receiver)->setMinSize(PropertyHelper<UVector2>::fromString(value));
}


String UnifiedMaxSize::get(const PropertyReceiver* receiver) const
{
    return PropertyHelper<UVector2>::toString(static_cast<const Window*>(receiver)->getMaxSize());
}

void UnifiedMaxSize::set(PropertyReceiver* receiver, const String& value)
{
    static_cast<Window*>(receiver)->setMaxSize(PropertyHelper<UVector2>::fromString(value));
}


String MousePassThroughEnabled::get(const PropertyReceiver* receiver) const
{
    return PropertyHelper<bool>::toString(static_cast<const Window*>(receiver)->isMousePassThroughEnabled());
}

void MousePassThroughEnabled::set(PropertyReceiver* receiver, const String& value)
{
    static_cast<Window*>(receiver)->setMousePassThroughEnabled(PropertyHelper<bool>::fromString(value));
}


String WindowRenderer::get(const PropertyReceiver* receiver) const
{
    CEGUI::WindowRenderer* wr = static_cast<const Window*>(receiver)->getWindowRenderer();
    return wr ? wr->getName() : "";
}

void WindowRenderer::set(PropertyReceiver* receiver, const String& value)
{
    static_cast<Window*>(receiver)->setWindowRenderer(value);
}

void WindowRenderer::writeXMLToStream(const PropertyReceiver* receiver, XMLSerializer& xml_stream) const
{
    const Window* wnd = static_cast<const Window*>(receiver);
    if (!WindowFactoryManager::getSingleton().isFalagardMappedType(wnd->getType()))
    {
        Property::writeXMLToStream(receiver, xml_stream);
    }
}


String LookNFeel::get(const PropertyReceiver* receiver) const
{
    return static_cast<const Window*>(receiver)->getLookNFeel();
}

void LookNFeel::set(PropertyReceiver* receiver, const String& value)
{
    static_cast<Window*>(receiver)->setLookNFeel(value);
}

void LookNFeel::writeXMLToStream(const PropertyReceiver* receiver, XMLSerializer& xml_stream) const
{
    const Window* wnd = static_cast<const Window*>(receiver);
    if (!WindowFactoryManager::getSingleton().isFalagardMappedType(wnd->getType()))
    {
        Property::writeXMLToStream(receiver, xml_stream);
    }
}


String DragDropTarget::get(const PropertyReceiver* receiver) const
{
    return PropertyHelper<bool>::toString(static_cast<const Window*>(receiver)->isDragDropTarget());
}


void DragDropTarget::set(PropertyReceiver* receiver, const String& value)
{
    static_cast<Window*>(receiver)->setDragDropTarget(PropertyHelper<bool>::fromString(value));
}

//----------------------------------------------------------------------------//
String AutoRenderingSurface::get(const PropertyReceiver* receiver) const
{
    return PropertyHelper<bool>::toString(
        static_cast<const Window*>(receiver)->isUsingAutoRenderingSurface());
}

//----------------------------------------------------------------------------//
void AutoRenderingSurface::set(PropertyReceiver* receiver, const String& value)
{
    static_cast<Window*>(receiver)->setUsingAutoRenderingSurface(
        PropertyHelper<bool>::fromString(value));
}

//----------------------------------------------------------------------------//
String Rotation::get(const PropertyReceiver* receiver) const
{
    return PropertyHelper<Quaternion>::toString(
        static_cast<const Window*>(receiver)->getRotation());
}

//----------------------------------------------------------------------------//
void Rotation::set(PropertyReceiver* receiver, const String& value)
{
    static_cast<Window*>(receiver)->setRotation(
        PropertyHelper<Quaternion>::fromString(value));
}

//----------------------------------------------------------------------------//
String NonClient::get(const PropertyReceiver* receiver) const
{
    return PropertyHelper<bool>::toString(
        static_cast<const Window*>(receiver)->isNonClientWindow());
}

//----------------------------------------------------------------------------//
void NonClient::set(PropertyReceiver* receiver, const String& value)
{
    static_cast<Window*>(receiver)->
        setNonClientWindow(PropertyHelper<bool>::fromString(value));
}

//----------------------------------------------------------------------------//
String TextParsingEnabled::get(const PropertyReceiver* receiver) const
{
    return PropertyHelper<bool>::toString(
        static_cast<const Window*>(receiver)->isTextParsingEnabled());
}

//----------------------------------------------------------------------------//
void TextParsingEnabled::set(PropertyReceiver* receiver, const String& value)
{
    static_cast<Window*>(receiver)->
        setTextParsingEnabled(PropertyHelper<bool>::fromString(value));
}

//----------------------------------------------------------------------------//
String Margin::get(const PropertyReceiver* receiver) const
{
	return PropertyHelper<UBox>::toString(
        static_cast<const Window*>(receiver)->getMargin());
}

//----------------------------------------------------------------------------//
void Margin::set(PropertyReceiver* receiver, const String& value)
{
	static_cast<Window*>(receiver)->
        setMargin(PropertyHelper<UBox>::fromString(value));
}

//----------------------------------------------------------------------------//
String UpdateMode::get(const PropertyReceiver* receiver) const
{
    switch(static_cast<const Window*>(receiver)->getUpdateMode())
    {
        case WUM_ALWAYS:
            return String("Always");

        case WUM_NEVER:
            return String("Never");

        default:
            return String("Visible");
    }
}

//----------------------------------------------------------------------------//
void UpdateMode::set(PropertyReceiver* receiver, const String& value)
{
    CEGUI::WindowUpdateMode m;

    if (value == "Always")
        m = WUM_ALWAYS;
    else if (value == "Never")
        m = WUM_NEVER;
    else
        m = WUM_VISIBLE;
    
    static_cast<Window*>(receiver)->setUpdateMode(m);
}

//----------------------------------------------------------------------------//
String MouseInputPropagationEnabled::get(const PropertyReceiver* receiver) const
{
    return PropertyHelper<bool>::toString(static_cast<const Window*>(receiver)->
        isMouseInputPropagationEnabled());
}

//----------------------------------------------------------------------------//
void MouseInputPropagationEnabled::set(PropertyReceiver* receiver,
                                       const String& value)
{
    static_cast<Window*>(receiver)->
        setMouseInputPropagationEnabled(PropertyHelper<bool>::fromString(value));
}

//----------------------------------------------------------------------------//

} // End of  WindowProperties namespace section

} // End of  CEGUI namespace section
