/************************************************************************
	filename: 	TLCloseButton.cpp
	created:	20/5/2004
	author:		Paul D Turner
	
	purpose:	Implementation of 'close button' widget.  This is needed
				due to the fact that Taharez frame windows standard
				controls have custom clipping requirements.
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
#include "TLCloseButton.h"

// Start of CEGUI namespace section
namespace CEGUI
{
/*************************************************************************
	Constants 
*************************************************************************/
// type name for this widget
const utf8	TLCloseButton::WidgetTypeName[]	= "TaharezLook/CloseButton";


/*************************************************************************
	return a Rect object describing the appropriately clipped Window
	area in screen space.
*************************************************************************/
Rect TLCloseButton::getPixelRect(void) const
{
    // clip to grand-parent as needed
    if (d_parent && d_parent->getParent() && isClippedByParent())
    {
        return d_parent->getParent()->getInnerRect().getIntersection(getUnclippedPixelRect());
    }
    // clip to screen if no grand-parent, or if clipping has been disabled for us.
    else 
    {
        return System::getSingleton().getRenderer()->getRect().getIntersection(getUnclippedPixelRect());
    }
}


//////////////////////////////////////////////////////////////////////////
/*************************************************************************

	Factory Methods

*************************************************************************/
//////////////////////////////////////////////////////////////////////////
/*************************************************************************
	Create, initialise and return a TLCloseButton
*************************************************************************/
Window* TLCloseButtonFactory::createWindow(const String& name)
{
	TLCloseButton* wnd = new TLCloseButton(d_type, name);
	wnd->initialise();

	return wnd;
}

} // End of  CEGUI namespace section
