/************************************************************************
	filename: 	WLCloseButton.cpp
	created:	9/8/2004
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
#include "WLCloseButton.h"

// Start of CEGUI namespace section
namespace CEGUI
{
/*************************************************************************
	return a Rect object describing the appropriately clipped Window
	area in screen space.
*************************************************************************/
Rect WLCloseButton::getPixelRect(void) const
{
	// clip to screen if we have no grand-parent
	if ((d_parent == NULL) || (d_parent->getParent() == NULL))
	{
		return System::getSingleton().getRenderer()->getRect().getIntersection(getUnclippedPixelRect());
	}
	// else clip to grand-parent
	else 
	{
		return d_parent->getParent()->getInnerRect().getIntersection(getUnclippedPixelRect());
	}

}


//////////////////////////////////////////////////////////////////////////
/*************************************************************************

	Factory Methods

*************************************************************************/
//////////////////////////////////////////////////////////////////////////
/*************************************************************************
	Create, initialise and return a WLCloseButton
*************************************************************************/
Window* WLCloseButtonFactory::createWindow(const String& name)
{
	WLCloseButton* wnd = new WLCloseButton(d_type, name);
	wnd->initialise();

	return wnd;
}

} // End of  CEGUI namespace section
