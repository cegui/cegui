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


// Start of CEGUI namespace section
namespace CEGUI
{
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

	Factory Methods

*************************************************************************/
//////////////////////////////////////////////////////////////////////////
/*************************************************************************
	Create, initialise and return a StaticText for the Taharez Scheme
*************************************************************************/
Window* TLStaticTextFactory::createWindow(const String& name)
{
	StaticText* wnd = new StaticText(d_type, name);
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
