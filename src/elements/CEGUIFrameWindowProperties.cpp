/************************************************************************
	filename: 	CEGUIFrameWindowProperties.cpp
	created:	10/7/2004
	author:		Paul D Turner
	
	purpose:	Implements properties for FrameWindow class
*************************************************************************/
/*************************************************************************
    Crazy Eddie's GUI System (http://crayzedsgui.sourceforge.net)
    Copyright (C)2004 Paul D Turner (crayzed@users.sourceforge.net)

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*************************************************************************/
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
String	SizingEnabled::get(const void* receiver) const
{
	return PropertyHelper::boolToString(static_cast<const FrameWindow*>(receiver)->isSizingEnabled());
}


void	SizingEnabled::set(void* receiver, const String& value)
{
	static_cast<FrameWindow*>(receiver)->setSizingEnabled(PropertyHelper::stringToBool(value));
}


String	FrameEnabled::get(const void* receiver) const
{
	return PropertyHelper::boolToString(static_cast<const FrameWindow*>(receiver)->isFrameEnabled());
}


void	FrameEnabled::set(void* receiver, const String& value)
{
	static_cast<FrameWindow*>(receiver)->setFrameEnabled(PropertyHelper::stringToBool(value));
}


String	TitlebarEnabled::get(const void* receiver) const
{
	return PropertyHelper::boolToString(static_cast<const FrameWindow*>(receiver)->isTitleBarEnabled());
}


void	TitlebarEnabled::set(void* receiver, const String& value)
{
	static_cast<FrameWindow*>(receiver)->setTitleBarEnabled(PropertyHelper::stringToBool(value));
}


String	CloseButtonEnabled::get(const void* receiver) const
{
	return PropertyHelper::boolToString(static_cast<const FrameWindow*>(receiver)->isCloseButtonEnabled());
}


void	CloseButtonEnabled::set(void* receiver, const String& value)
{
	static_cast<FrameWindow*>(receiver)->setCloseButtonEnabled(PropertyHelper::stringToBool(value));
}


String	DragMovingEnabled::get(const void* receiver) const
{
	return PropertyHelper::boolToString(static_cast<const FrameWindow*>(receiver)->isDragMovingEnabled());
}


void	DragMovingEnabled::set(void* receiver, const String& value)
{
	static_cast<FrameWindow*>(receiver)->setDragMovingEnabled(PropertyHelper::stringToBool(value));
}


String	RollUpEnabled::get(const void* receiver) const
{
	return PropertyHelper::boolToString(static_cast<const FrameWindow*>(receiver)->isRollupEnabled());
}


void	RollUpEnabled::set(void* receiver, const String& value)
{
	static_cast<FrameWindow*>(receiver)->setRollupEnabled(PropertyHelper::stringToBool(value));
}


String	RollUpState::get(const void* receiver) const
{
	return PropertyHelper::boolToString(static_cast<const FrameWindow*>(receiver)->isRolledup());
}


void	RollUpState::set(void* receiver, const String& value)
{
	bool reqState = PropertyHelper::stringToBool(value);

	if (reqState != static_cast<FrameWindow*>(receiver)->isRolledup())
	{
		static_cast<FrameWindow*>(receiver)->toggleRollup();
	}

}


String	SizingBorderThickness::get(const void* receiver) const
{
	return PropertyHelper::floatToString(static_cast<const FrameWindow*>(receiver)->getSizingBorderThickness());
}


void	SizingBorderThickness::set(void* receiver, const String& value)
{
	static_cast<FrameWindow*>(receiver)->setSizingBorderThickness(PropertyHelper::stringToFloat(value));
}


String	TitlebarFont::get(const void* receiver) const
{
	return static_cast<const FrameWindow*>(receiver)->getTitlebarFont()->getName();	
}


void	TitlebarFont::set(void* receiver, const String& value)
{
	static_cast<FrameWindow*>(receiver)->setTitlebarFont(value);
}

String CaptionColour::get(const void* receiver) const
{
	return PropertyHelper::colourToString(static_cast<const FrameWindow*>(receiver)->getCaptionColour());
}

void CaptionColour::set(void* receiver, const String& value)
{
	static_cast<FrameWindow*>(receiver)->setCaptionColour(PropertyHelper::stringToColour(value));
}

} // End of  FrameWindowProperties namespace section

} // End of  CEGUI namespace section
