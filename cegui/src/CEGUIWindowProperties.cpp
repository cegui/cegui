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
