/************************************************************************
	filename: 	CEGUIStaticProperties.cpp
	created:	10/7/2004
	author:		Paul D Turner
	
	purpose:	Implements property classes for the Static widget.
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
#include "elements/CEGUIStaticProperties.h"
#include "elements/CEGUIStatic.h"
#include "CEGUIPropertyHelper.h"


// Start of CEGUI namespace section
namespace CEGUI
{

// Start of StaticProperties namespace section
namespace StaticProperties
{
String	FrameEnabled::get(const void* receiver) const
{
	return PropertyHelper::boolToString(static_cast<const Static*>(receiver)->isFrameEnabled());
}


void	FrameEnabled::set(void* receiver, const String& value)
{
	static_cast<Static*>(receiver)->setFrameEnabled(PropertyHelper::stringToBool(value));
}


String	BackgroundEnabled::get(const void* receiver) const
{
	return PropertyHelper::boolToString(static_cast<const Static*>(receiver)->isBackgroundEnabled());
}


void	BackgroundEnabled::set(void* receiver, const String& value)
{
	static_cast<Static*>(receiver)->setBackgroundEnabled(PropertyHelper::stringToBool(value));
}


String	FrameColours::get(const void* receiver) const
{
	return PropertyHelper::colourRectToString(static_cast<const Static*>(receiver)->getFrameColours());
}


void	FrameColours::set(void* receiver, const String& value)
{
	static_cast<Static*>(receiver)->setFrameColours(PropertyHelper::stringToColourRect(value));
}


String	BackgroundColours::get(const void* receiver) const
{
	return PropertyHelper::colourRectToString(static_cast<const Static*>(receiver)->getBackgroundColours());
}


void	BackgroundColours::set(void* receiver, const String& value)
{
	static_cast<Static*>(receiver)->setBackgroundColours(PropertyHelper::stringToColourRect(value));
}


String	BackgroundImage::get(const void* receiver) const
{
	return PropertyHelper::imageToString(static_cast<const Static*>(receiver)->getBackgroundImage());
}


void	BackgroundImage::set(void* receiver, const String& value)
{
	static_cast<Static*>(receiver)->setBackgroundImage(PropertyHelper::stringToImage(value));
}


String	TopLeftFrameImage::get(const void* receiver) const
{
	return PropertyHelper::imageToString(static_cast<const Static*>(receiver)->getImageForFrameLocation(TopLeftCorner));
}


void	TopLeftFrameImage::set(void* receiver, const String& value)
{
	static_cast<Static*>(receiver)->setImageForFrameLocation(TopLeftCorner, PropertyHelper::stringToImage(value));
}


String	TopRightFrameImage::get(const void* receiver) const
{
	return PropertyHelper::imageToString(static_cast<const Static*>(receiver)->getImageForFrameLocation(TopRightCorner));
}


void	TopRightFrameImage::set(void* receiver, const String& value)
{
	static_cast<Static*>(receiver)->setImageForFrameLocation(TopRightCorner, PropertyHelper::stringToImage(value));
}


String	BottomLeftFrameImage::get(const void* receiver) const
{
	return PropertyHelper::imageToString(static_cast<const Static*>(receiver)->getImageForFrameLocation(BottomLeftCorner));
}


void	BottomLeftFrameImage::set(void* receiver, const String& value)
{
	static_cast<Static*>(receiver)->setImageForFrameLocation(BottomLeftCorner, PropertyHelper::stringToImage(value));
}


String	BottomRightFrameImage::get(const void* receiver) const
{
	return PropertyHelper::imageToString(static_cast<const Static*>(receiver)->getImageForFrameLocation(BottomRightCorner));
}


void	BottomRightFrameImage::set(void* receiver, const String& value)
{
	static_cast<Static*>(receiver)->setImageForFrameLocation(BottomRightCorner, PropertyHelper::stringToImage(value));
}


String	LeftFrameImage::get(const void* receiver) const
{
	return PropertyHelper::imageToString(static_cast<const Static*>(receiver)->getImageForFrameLocation(LeftEdge));
}


void	LeftFrameImage::set(void* receiver, const String& value)
{
	static_cast<Static*>(receiver)->setImageForFrameLocation(LeftEdge, PropertyHelper::stringToImage(value));
}


String	RightFrameImage::get(const void* receiver) const
{
	return PropertyHelper::imageToString(static_cast<const Static*>(receiver)->getImageForFrameLocation(RightEdge));
}


void	RightFrameImage::set(void* receiver, const String& value)
{
	static_cast<Static*>(receiver)->setImageForFrameLocation(RightEdge, PropertyHelper::stringToImage(value));
}


String	TopFrameImage::get(const void* receiver) const
{
	return PropertyHelper::imageToString(static_cast<const Static*>(receiver)->getImageForFrameLocation(TopEdge));
}


void	TopFrameImage::set(void* receiver, const String& value)
{
	static_cast<Static*>(receiver)->setImageForFrameLocation(TopEdge, PropertyHelper::stringToImage(value));
}


String	BottomFrameImage::get(const void* receiver) const
{
	return PropertyHelper::imageToString(static_cast<const Static*>(receiver)->getImageForFrameLocation(BottomEdge));
}


void	BottomFrameImage::set(void* receiver, const String& value)
{
	static_cast<Static*>(receiver)->setImageForFrameLocation(BottomEdge, PropertyHelper::stringToImage(value));
}

} // End of  StaticProperties namespace section

} // End of  CEGUI namespace section
