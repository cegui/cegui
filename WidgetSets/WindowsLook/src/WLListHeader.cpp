/************************************************************************
	filename: 	WLListHeader.cpp
	created:	16/8/2004
	author:		Paul D Turner
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
#include "WLListHeader.h"
#include "CEGUIWindowManager.h"
#include "WLListHeaderSegment.h"

// Start of CEGUI namespace section
namespace CEGUI
{
/*************************************************************************
	Constants
*************************************************************************/
const utf8	WLListHeader::WidgetTypeName[]		= "WindowsLook/ListHeader";

// image / imageset related
const utf8 WLListHeader::ImagesetName[]			= "WindowsLook";
const utf8 WLListHeader::MouseCursorImageName[]	= "MouseArrow";

	
// type names for the component widgets
const utf8* WLListHeader::SegmentWidgetType		= WLListHeaderSegment::WidgetTypeName;


/*************************************************************************
	Constructor
*************************************************************************/
WLListHeader::WLListHeader(const String& type, const String& name) :
	ListHeader(type, name)
{
	setMouseCursor(ImagesetName, MouseCursorImageName);
}


/*************************************************************************
	Destructor
*************************************************************************/
WLListHeader::~WLListHeader(void)
{
}

	
/*************************************************************************
	Render the widget.
*************************************************************************/
void WLListHeader::drawSelf(float z)
{
}


/*************************************************************************
	Create and return a pointer to a new ListHeaderSegment based object.
*************************************************************************/
ListHeaderSegment* WLListHeader::createNewSegment(const String& name) const
{
	return (ListHeaderSegment*)WindowManager::getSingleton().createWindow(SegmentWidgetType, name);
}


/*************************************************************************
	Cleanup and destroy the given ListHeaderSegment that was created
	via the createNewSegment method.
*************************************************************************/
void WLListHeader::destroyListSegment(ListHeaderSegment* segment) const
{
	WindowManager::getSingleton().destroyWindow(segment);
}


//////////////////////////////////////////////////////////////////////////
/*************************************************************************

	Factory Methods

*************************************************************************/
//////////////////////////////////////////////////////////////////////////
/*************************************************************************
	Create, initialise and return a WLListHeader
*************************************************************************/
Window* WLListHeaderFactory::createWindow(const String& name)
{
	WLListHeader* wnd = new WLListHeader(d_type, name);
	wnd->initialise();

	return wnd;
}

} // End of  CEGUI namespace section
