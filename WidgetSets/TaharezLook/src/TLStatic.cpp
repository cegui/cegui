/************************************************************************
	filename: 	TLStatic.cpp
	created:	5/6/2004
	author:		Paul D Turner
	
	purpose:	Implementation of Taharez Look static widgets & factories.
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
#include "TLStatic.h"
#include "elements/CEGUIStaticText.h"
#include "elements/CEGUIStaticImage.h"
#include "CEGUIImagesetManager.h"
#include "CEGUIImageset.h"
#include "CEGUIWindowManager.h"
#include "elements/CEGUIScrollbar.h"


// Start of CEGUI namespace section
namespace CEGUI
{
/*************************************************************************
	Constants
*************************************************************************/
// component widget type names
const utf8	TLStaticText::HorzScrollbarTypeName[]		= "Taharez MiniHorzScrollbar";
const utf8	TLStaticText::VertScrollbarTypeName[]		= "Taharez MiniVertScrollbar";


/*************************************************************************
	Routine to do some common initialisation of static widgets
*************************************************************************/
void initTaharezStatic(Static* s)
{
	Imageset* iset = ImagesetManager::getSingleton().getImageset((utf8*)"TaharezImagery");
	
	s->setFrameImages(
		&iset->getImage((utf8*)"StaticTopLeft"),
		&iset->getImage((utf8*)"StaticTopRight"),
		&iset->getImage((utf8*)"StaticBottomLeft"),
		&iset->getImage((utf8*)"StaticBottomRight"),
		&iset->getImage((utf8*)"StaticLeft"),
		&iset->getImage((utf8*)"StaticTop"),
		&iset->getImage((utf8*)"StaticRight"),
		&iset->getImage((utf8*)"StaticBottom")
		);

	s->setBackgroundImage(&iset->getImage((utf8*)"StaticBackdrop"));

	s->setFrameEnabled(true);
	s->setBackgroundEnabled(true);
}


//////////////////////////////////////////////////////////////////////////
/*************************************************************************
	
	TLStaticText methods

*************************************************************************/
//////////////////////////////////////////////////////////////////////////

/*************************************************************************
	create and return a pointer to a Scrollbar widget for use as
	vertical scroll bar	
*************************************************************************/
Scrollbar* TLStaticText::createVertScrollbar(void) const
{
	Scrollbar* sbar = (Scrollbar*)WindowManager::getSingleton().createWindow(VertScrollbarTypeName, getName() + "__auto_vscrollbar__");

	// set min/max sizes
	sbar->setMinimumSize(Size(0.0125f, 0.0f));
	sbar->setMaximumSize(Size(0.0125f, 1.0f));

	return sbar;
}


/*************************************************************************
	create and return a pointer to a Scrollbar widget for use as
	horizontal scroll bar	
*************************************************************************/
Scrollbar* TLStaticText::createHorzScrollbar(void) const
{
	Scrollbar* sbar = (Scrollbar*)WindowManager::getSingleton().createWindow(HorzScrollbarTypeName, getName() + "__auto_hscrollbar__");

	// set min/max sizes
	sbar->setMinimumSize(Size(0.0f, 0.016667f));
	sbar->setMaximumSize(Size(1.0f, 0.016667f));

	return sbar;
}


//////////////////////////////////////////////////////////////////////////
/*************************************************************************

	Factory Methods

*************************************************************************/
//////////////////////////////////////////////////////////////////////////
/*************************************************************************
	Create, initialise and return a StaticText for the Taharez Scheme
*************************************************************************/
Window* TLStaticTextFactory::createWindow(const String& name)
{
	TLStaticText* wnd = new TLStaticText(d_type, name);
	wnd->initialise();

	initTaharezStatic(wnd);

	return wnd;
}


/*************************************************************************
	Create, initialise and return a StaticImage for the Taharez Scheme
*************************************************************************/
Window* TLStaticImageFactory::createWindow(const String& name)
{
	StaticImage* wnd = new StaticImage(d_type, name);
	wnd->initialise();

	initTaharezStatic(wnd);

	return wnd;
}

} // End of  CEGUI namespace section
