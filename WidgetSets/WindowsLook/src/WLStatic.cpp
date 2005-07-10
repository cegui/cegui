/************************************************************************
	filename: 	WLStatic.cpp
	created:	8/8/2004
	author:		Paul D Turner
*************************************************************************/
/*************************************************************************
    Crazy Eddie's GUI System (http://www.cegui.org.uk)
    Copyright (C)2004 - 2005 Paul D Turner (paul@cegui.org.uk)

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
#include "WLStatic.h"
#include "elements/CEGUIStaticText.h"
#include "elements/CEGUIStaticImage.h"
#include "CEGUIImagesetManager.h"
#include "CEGUIImageset.h"
#include "CEGUIWindowManager.h"
#include "WLVertScrollbar.h"
#include "WLHorzScrollbar.h"


// Start of CEGUI namespace section
namespace CEGUI
{
/*************************************************************************
	Constants
*************************************************************************/
const utf8	WLStaticText::WidgetTypeName[]		= "WindowsLook/StaticText";
const utf8	WLStaticImage::WidgetTypeName[]		= "WindowsLook/StaticImage";

// component widget type names for static text
const utf8*	WLStaticText::HorzScrollbarTypeName		= WLHorzScrollbar::WidgetTypeName;
const utf8*	WLStaticText::VertScrollbarTypeName		= WLVertScrollbar::WidgetTypeName;


/*************************************************************************
	Routine to do some common initialisation of static widgets
*************************************************************************/
void initWinLookStatic(Static* s)
{
	Imageset* iset = ImagesetManager::getSingleton().getImageset((utf8*)"WindowsLook");
	
	s->setFrameImages(
		&iset->getImage((utf8*)"StaticFrameTopLeft"),
		&iset->getImage((utf8*)"StaticFrameTopRight"),
		&iset->getImage((utf8*)"StaticFrameBottomLeft"),
		&iset->getImage((utf8*)"StaticFrameBottomRight"),
		&iset->getImage((utf8*)"StaticFrameLeft"),
		&iset->getImage((utf8*)"StaticFrameTop"),
		&iset->getImage((utf8*)"StaticFrameRight"),
		&iset->getImage((utf8*)"StaticFrameBottom")
		);

	s->setBackgroundImage(&iset->getImage((utf8*)"Background"));
	s->setBackgroundColours(colour(0.875f, 0.875f, 0.875f));

	s->setFrameEnabled(true);
	s->setBackgroundEnabled(true);
}


//////////////////////////////////////////////////////////////////////////
/*************************************************************************
	
	WLStaticText methods

*************************************************************************/
//////////////////////////////////////////////////////////////////////////

/*************************************************************************
	create and return a pointer to a Scrollbar widget for use as
	vertical scroll bar	
*************************************************************************/
Scrollbar* WLStaticText::createVertScrollbar(const String& name) const
{
	Scrollbar* sbar = (Scrollbar*)WindowManager::getSingleton().createWindow(VertScrollbarTypeName, name);

	// set min/max sizes
	sbar->setMinimumSize(Size(0.0125f, 0.0f));
	sbar->setMaximumSize(Size(0.0125f, 1.0f));

	return sbar;
}


/*************************************************************************
	create and return a pointer to a Scrollbar widget for use as
	horizontal scroll bar	
*************************************************************************/
Scrollbar* WLStaticText::createHorzScrollbar(const String& name) const
{
	Scrollbar* sbar = (Scrollbar*)WindowManager::getSingleton().createWindow(HorzScrollbarTypeName, name);

	// set min/max sizes
	sbar->setMinimumSize(Size(0.0f, 0.016667f));
	sbar->setMaximumSize(Size(1.0f, 0.016667f));

	return sbar;
}


/*************************************************************************
	Initialises the Window based object ready for use.
*************************************************************************/
void WLStaticText::initialise(void)
{
	StaticText::initialise();
	initWinLookStatic(this);

	setTextColours(colour(0, 0, 0));
}


//////////////////////////////////////////////////////////////////////////
/*************************************************************************
	
	WLStaticImage methods

*************************************************************************/
//////////////////////////////////////////////////////////////////////////

/*************************************************************************
	Initialises the Window based object ready for use.
*************************************************************************/
void WLStaticImage::initialise(void)
{
	StaticImage::initialise();
	initWinLookStatic(this);
}


//////////////////////////////////////////////////////////////////////////
/*************************************************************************

	Factory Methods

*************************************************************************/
//////////////////////////////////////////////////////////////////////////
/*************************************************************************
	Create, initialise and return a StaticText for the WindowsLook Scheme
*************************************************************************/
Window* WLStaticTextFactory::createWindow(const String& name)
{
	return new WLStaticText(d_type, name);
}


/*************************************************************************
	Create, initialise and return a StaticImage for the WindowsLook Scheme
*************************************************************************/
Window* WLStaticImageFactory::createWindow(const String& name)
{
	return new WLStaticImage(d_type, name);
}

} // End of  CEGUI namespace section
