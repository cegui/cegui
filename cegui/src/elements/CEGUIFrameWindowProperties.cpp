/***********************************************************************
	filename: 	CEGUIFrameWindowProperties.cpp
	created:	10/7/2004
	author:		Paul D Turner
	
	purpose:	Implements properties for FrameWindow class
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
#include "elements/CEGUIFrameWindowProperties.h"
#include "elements/CEGUIFrameWindow.h"
#include "CEGUIPropertyHelper.h"
#include "CEGUIExceptions.h"
#include "CEGUIFont.h"


// Start of CEGUI namespace section
namespace CEGUI
{

// Start of FrameWindowProperties namespace section
namespace FrameWindowProperties
{
String	SizingEnabled::get(const PropertyReceiver* receiver) const
{
	return PropertyHelper<bool>::toString(static_cast<const FrameWindow*>(receiver)->isSizingEnabled());
}


void	SizingEnabled::set(PropertyReceiver* receiver, const String& value)
{
	static_cast<FrameWindow*>(receiver)->setSizingEnabled(PropertyHelper<bool>::fromString(value));
}


String	FrameEnabled::get(const PropertyReceiver* receiver) const
{
	return PropertyHelper<bool>::toString(static_cast<const FrameWindow*>(receiver)->isFrameEnabled());
}


void	FrameEnabled::set(PropertyReceiver* receiver, const String& value)
{
	static_cast<FrameWindow*>(receiver)->setFrameEnabled(PropertyHelper<bool>::fromString(value));
}


String	TitlebarEnabled::get(const PropertyReceiver* receiver) const
{
	return PropertyHelper<bool>::toString(static_cast<const FrameWindow*>(receiver)->isTitleBarEnabled());
}


void	TitlebarEnabled::set(PropertyReceiver* receiver, const String& value)
{
	static_cast<FrameWindow*>(receiver)->setTitleBarEnabled(PropertyHelper<bool>::fromString(value));
}


String	CloseButtonEnabled::get(const PropertyReceiver* receiver) const
{
	return PropertyHelper<bool>::toString(static_cast<const FrameWindow*>(receiver)->isCloseButtonEnabled());
}


void	CloseButtonEnabled::set(PropertyReceiver* receiver, const String& value)
{
	static_cast<FrameWindow*>(receiver)->setCloseButtonEnabled(PropertyHelper<bool>::fromString(value));
}


String	DragMovingEnabled::get(const PropertyReceiver* receiver) const
{
	return PropertyHelper<bool>::toString(static_cast<const FrameWindow*>(receiver)->isDragMovingEnabled());
}


void	DragMovingEnabled::set(PropertyReceiver* receiver, const String& value)
{
	static_cast<FrameWindow*>(receiver)->setDragMovingEnabled(PropertyHelper<bool>::fromString(value));
}


String	RollUpEnabled::get(const PropertyReceiver* receiver) const
{
	return PropertyHelper<bool>::toString(static_cast<const FrameWindow*>(receiver)->isRollupEnabled());
}


void	RollUpEnabled::set(PropertyReceiver* receiver, const String& value)
{
	static_cast<FrameWindow*>(receiver)->setRollupEnabled(PropertyHelper<bool>::fromString(value));
}


String	RollUpState::get(const PropertyReceiver* receiver) const
{
	return PropertyHelper<bool>::toString(static_cast<const FrameWindow*>(receiver)->isRolledup());
}


void	RollUpState::set(PropertyReceiver* receiver, const String& value)
{
	bool reqState = PropertyHelper<bool>::fromString(value);

	if (reqState != static_cast<FrameWindow*>(receiver)->isRolledup())
	{
		static_cast<FrameWindow*>(receiver)->toggleRollup();
	}

}


String	SizingBorderThickness::get(const PropertyReceiver* receiver) const
{
	return PropertyHelper<float>::toString(static_cast<const FrameWindow*>(receiver)->getSizingBorderThickness());
}


void	SizingBorderThickness::set(PropertyReceiver* receiver, const String& value)
{
	static_cast<FrameWindow*>(receiver)->setSizingBorderThickness(PropertyHelper<float>::fromString(value));
}


String NSSizingCursorImage::get(const PropertyReceiver* receiver) const
{
    const Image* img =
        static_cast<const FrameWindow*>(receiver)->getNSSizingCursorImage();
    return PropertyHelper<Image*>::toString(img);
}

void NSSizingCursorImage::set(PropertyReceiver* receiver, const String &value)
{
    static_cast<FrameWindow*>(receiver)->
        setNSSizingCursorImage(PropertyHelper<Image*>::fromString(value));
}

String EWSizingCursorImage::get(const PropertyReceiver* receiver) const
{
    const Image* img =
        static_cast<const FrameWindow*>(receiver)->getEWSizingCursorImage();
    return PropertyHelper<Image*>::toString(img);
}

void EWSizingCursorImage::set(PropertyReceiver* receiver, const String &value)
{
    static_cast<FrameWindow*>(receiver)->
        setEWSizingCursorImage(PropertyHelper<Image*>::fromString(value));
}

String NWSESizingCursorImage::get(const PropertyReceiver* receiver) const
{
    const Image* img =
        static_cast<const FrameWindow*>(receiver)->getNWSESizingCursorImage();
    return PropertyHelper<Image*>::toString(img);
}

void NWSESizingCursorImage::set(PropertyReceiver* receiver, const String &value)
{
    static_cast<FrameWindow*>(receiver)->
        setNWSESizingCursorImage(PropertyHelper<Image*>::fromString(value));
}

String NESWSizingCursorImage::get(const PropertyReceiver* receiver) const
{
    const Image* img =
        static_cast<const FrameWindow*>(receiver)->getNESWSizingCursorImage();
    return PropertyHelper<Image*>::toString(img);
}

void NESWSizingCursorImage::set(PropertyReceiver* receiver, const String &value)
{
    static_cast<FrameWindow*>(receiver)->
        setNESWSizingCursorImage(PropertyHelper<Image*>::fromString(value));
}


} // End of  FrameWindowProperties namespace section

} // End of  CEGUI namespace section
