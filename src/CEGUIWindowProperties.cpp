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
#include <cstdlib>
#include <cstdio>

// Start of CEGUI namespace section
namespace CEGUI
{
// Start of WindowProperties namespace section
namespace WindowProperties
{
String ID::get(const PropertyReceiver* receiver) const
{
	return PropertyHelper::uintToString(static_cast<const Window*>(receiver)->getID());
}


void ID::set(PropertyReceiver* receiver, const String& value)
{
	static_cast<Window*>(receiver)->setID(PropertyHelper::stringToUint(value));
}


String Alpha::get(const PropertyReceiver* receiver) const
{
	return PropertyHelper::floatToString(static_cast<const Window*>(receiver)->getAlpha());
}


void Alpha::set(PropertyReceiver* receiver, const String& value)
{
	static_cast<Window*>(receiver)->setAlpha(PropertyHelper::stringToFloat(value));
}


String Font::get(const PropertyReceiver* receiver) const
{
	const CEGUI::Font* fnt = static_cast<const Window*>(receiver)->getFont();

	if (fnt)
	{
		return fnt->getProperty ("Name");
	}
	else
	{
		return String();
	}

}


void Font::set(PropertyReceiver* receiver, const String& value)
{
	try
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
	catch (UnknownObjectException&)
	{ }
}

bool Font::isDefault(const PropertyReceiver* receiver) const
{
    return static_cast<const Window*>(receiver)->getFont(false) == 0;
}


String Text::get(const PropertyReceiver* receiver) const
{
	return static_cast<const Window*>(receiver)->getText();
}


void Text::set(PropertyReceiver* receiver, const String& value)
{
	static_cast<Window*>(receiver)->setText(value);
}


String MouseCursorImage::get(const PropertyReceiver* receiver) const
{
	const Image* img = static_cast<const Window*>(receiver)->getMouseCursor();

	if (img)
	{
		return PropertyHelper::imageToString(img);
	}
	else
	{
		return String();
	}

}

void MouseCursorImage::set(PropertyReceiver* receiver, const String& value)
{
	if (!value.empty())
	{
		static_cast<Window*>(receiver)->setMouseCursor(PropertyHelper::stringToImage(value));
	}
}

bool MouseCursorImage::isDefault(const PropertyReceiver* receiver) const
{
    return static_cast<const Window*>(receiver)->getMouseCursor(false) == 0;
}


String ClippedByParent::get(const PropertyReceiver* receiver) const
{
	return PropertyHelper::boolToString(static_cast<const Window*>(receiver)->isClippedByParent());
}


void ClippedByParent::set(PropertyReceiver* receiver, const String& value)
{
	static_cast<Window*>(receiver)->setClippedByParent(PropertyHelper::stringToBool(value));
}


String InheritsAlpha::get(const PropertyReceiver* receiver) const
{
	return PropertyHelper::boolToString(static_cast<const Window*>(receiver)->inheritsAlpha());
}


void InheritsAlpha::set(PropertyReceiver* receiver, const String& value)
{
	static_cast<Window*>(receiver)->setInheritsAlpha(PropertyHelper::stringToBool(value));
}


String AlwaysOnTop::get(const PropertyReceiver* receiver) const
{
	return PropertyHelper::boolToString(static_cast<const Window*>(receiver)->isAlwaysOnTop());
}


void AlwaysOnTop::set(PropertyReceiver* receiver, const String& value)
{
	static_cast<Window*>(receiver)->setAlwaysOnTop(PropertyHelper::stringToBool(value));
}


String Disabled::get(const PropertyReceiver* receiver) const
{
	return PropertyHelper::boolToString(static_cast<const Window*>(receiver)->isDisabled());
}


void Disabled::set(PropertyReceiver* receiver, const String& value)
{
	static_cast<Window*>(receiver)->setEnabled(!PropertyHelper::stringToBool(value));
}

bool Disabled::isDefault(const PropertyReceiver* receiver) const
{
    return !static_cast<const Window*>(receiver)->isDisabled(true);
}


String Visible::get(const PropertyReceiver* receiver) const
{
	return PropertyHelper::boolToString(static_cast<const Window*>(receiver)->isVisible());
}


void Visible::set(PropertyReceiver* receiver, const String& value)
{
	static_cast<Window*>(receiver)->setVisible(PropertyHelper::stringToBool(value));
}

bool Visible::isDefault(const PropertyReceiver* receiver) const
{
    return static_cast<const Window*>(receiver)->isVisible(true);
}


String RestoreOldCapture::get(const PropertyReceiver* receiver) const
{
	return	PropertyHelper::boolToString(static_cast<const Window*>(receiver)->restoresOldCapture());
}


void RestoreOldCapture::set(PropertyReceiver* receiver, const String& value)
{
	static_cast<Window*>(receiver)->setRestoreCapture(PropertyHelper::stringToBool(value));
}


String DestroyedByParent::get(const PropertyReceiver* receiver) const
{
	return PropertyHelper::boolToString(static_cast<const Window*>(receiver)->isDestroyedByParent());
}


void DestroyedByParent::set(PropertyReceiver* receiver, const String& value)
{
	static_cast<Window*>(receiver)->setDestroyedByParent(PropertyHelper::stringToBool(value));
}


String ZOrderChangeEnabled::get(const PropertyReceiver* receiver) const
{
	return PropertyHelper::boolToString(static_cast<const Window*>(receiver)->isZOrderingEnabled());
}


void ZOrderChangeEnabled::set(PropertyReceiver* receiver, const String& value)
{
	static_cast<Window*>(receiver)->setZOrderingEnabled(PropertyHelper::stringToBool(value));
}


String WantsMultiClickEvents::get(const PropertyReceiver* receiver) const
{
    return PropertyHelper::boolToString(static_cast<const Window*>(receiver)->wantsMultiClickEvents());
}


void WantsMultiClickEvents::set(PropertyReceiver* receiver, const String& value)
{
    static_cast<Window*>(receiver)->setWantsMultiClickEvents(PropertyHelper::stringToBool(value));
}


String MouseButtonDownAutoRepeat::get(const PropertyReceiver* receiver) const
{
    return PropertyHelper::boolToString(static_cast<const Window*>(receiver)->isMouseAutoRepeatEnabled());
}


void MouseButtonDownAutoRepeat::set(PropertyReceiver* receiver, const String& value)
{
    static_cast<Window*>(receiver)->setMouseAutoRepeatEnabled(PropertyHelper::stringToBool(value));
}


String AutoRepeatDelay::get(const PropertyReceiver* receiver) const
{
    return PropertyHelper::floatToString(static_cast<const Window*>(receiver)->getAutoRepeatDelay());
}


void AutoRepeatDelay::set(PropertyReceiver* receiver, const String& value)
{
    static_cast<Window*>(receiver)->setAutoRepeatDelay(PropertyHelper::stringToFloat(value));
}


String AutoRepeatRate::get(const PropertyReceiver* receiver) const
{
    return PropertyHelper::floatToString(static_cast<const Window*>(receiver)->getAutoRepeatRate());
}


void AutoRepeatRate::set(PropertyReceiver* receiver, const String& value)
{
    static_cast<Window*>(receiver)->setAutoRepeatRate(PropertyHelper::stringToFloat(value));
}


String DistributeCapturedInputs::get(const PropertyReceiver* receiver) const
{
    return PropertyHelper::boolToString(static_cast<const Window*>(receiver)->distributesCapturedInputs());
}


void DistributeCapturedInputs::set(PropertyReceiver* receiver, const String& value)
{
    static_cast<Window*>(receiver)->setDistributesCapturedInputs(PropertyHelper::stringToBool(value));
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
    return PropertyHelper::boolToString(static_cast<const Window*>(receiver)->inheritsTooltipText());
}


void InheritsTooltipText::set(PropertyReceiver* receiver, const String& value)
{
    static_cast<Window*>(receiver)->setInheritsTooltipText(PropertyHelper::stringToBool(value));
}


String RiseOnClick::get(const PropertyReceiver* receiver) const
{
    return PropertyHelper::boolToString(static_cast<const Window*>(receiver)->isRiseOnClickEnabled());
}


void RiseOnClick::set(PropertyReceiver* receiver, const String& value)
{
    static_cast<Window*>(receiver)->setRiseOnClickEnabled(PropertyHelper::stringToBool(value));
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
    return PropertyHelper::urectToString(static_cast<const Window*>(receiver)->getArea());
}

void UnifiedAreaRect::set(PropertyReceiver* receiver, const String& value)
{
    static_cast<Window*>(receiver)->setArea(PropertyHelper::stringToURect(value));
}


String UnifiedPosition::get(const PropertyReceiver* receiver) const
{
    return PropertyHelper::uvector2ToString(static_cast<const Window*>(receiver)->getPosition());
}

void UnifiedPosition::set(PropertyReceiver* receiver, const String& value)
{
    static_cast<Window*>(receiver)->setPosition(PropertyHelper::stringToUVector2(value));
}


String UnifiedXPosition::get(const PropertyReceiver* receiver) const
{
    return PropertyHelper::udimToString(static_cast<const Window*>(receiver)->getXPosition());
}

void UnifiedXPosition::set(PropertyReceiver* receiver, const String& value)
{
    static_cast<Window*>(receiver)->setXPosition(PropertyHelper::stringToUDim(value));
}


String UnifiedYPosition::get(const PropertyReceiver* receiver) const
{
    return PropertyHelper::udimToString(static_cast<const Window*>(receiver)->getYPosition());
}

void UnifiedYPosition::set(PropertyReceiver* receiver, const String& value)
{
    static_cast<Window*>(receiver)->setYPosition(PropertyHelper::stringToUDim(value));
}


String UnifiedSize::get(const PropertyReceiver* receiver) const
{
    return PropertyHelper::uvector2ToString(static_cast<const Window*>(receiver)->getSize());
}

void UnifiedSize::set(PropertyReceiver* receiver, const String& value)
{
    static_cast<Window*>(receiver)->setSize(PropertyHelper::stringToUVector2(value));
}


String UnifiedWidth::get(const PropertyReceiver* receiver) const
{
    return PropertyHelper::udimToString(static_cast<const Window*>(receiver)->getWidth());
}

void UnifiedWidth::set(PropertyReceiver* receiver, const String& value)
{
    static_cast<Window*>(receiver)->setWidth(PropertyHelper::stringToUDim(value));
}


String UnifiedHeight::get(const PropertyReceiver* receiver) const
{
    return PropertyHelper::udimToString(static_cast<const Window*>(receiver)->getHeight());
}

void UnifiedHeight::set(PropertyReceiver* receiver, const String& value)
{
    static_cast<Window*>(receiver)->setHeight(PropertyHelper::stringToUDim(value));
}


String UnifiedMinSize::get(const PropertyReceiver* receiver) const
{
    return PropertyHelper::uvector2ToString(static_cast<const Window*>(receiver)->getMinSize());
}

void UnifiedMinSize::set(PropertyReceiver* receiver, const String& value)
{
    static_cast<Window*>(receiver)->setMinSize(PropertyHelper::stringToUVector2(value));
}


String UnifiedMaxSize::get(const PropertyReceiver* receiver) const
{
    return PropertyHelper::uvector2ToString(static_cast<const Window*>(receiver)->getMaxSize());
}

void UnifiedMaxSize::set(PropertyReceiver* receiver, const String& value)
{
    static_cast<Window*>(receiver)->setMaxSize(PropertyHelper::stringToUVector2(value));
}


String MousePassThroughEnabled::get(const PropertyReceiver* receiver) const
{
    return PropertyHelper::boolToString(static_cast<const Window*>(receiver)->isMousePassThroughEnabled());
}

void MousePassThroughEnabled::set(PropertyReceiver* receiver, const String& value)
{
    static_cast<Window*>(receiver)->setMousePassThroughEnabled(PropertyHelper::stringToBool(value));
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
    return PropertyHelper::boolToString(static_cast<const Window*>(receiver)->isDragDropTarget());
}


void DragDropTarget::set(PropertyReceiver* receiver, const String& value)
{
    static_cast<Window*>(receiver)->setDragDropTarget(PropertyHelper::stringToBool(value));
}


} // End of  WindowProperties namespace section

} // End of  CEGUI namespace section
