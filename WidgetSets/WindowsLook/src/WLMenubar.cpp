/************************************************************************
    filename:   WLMenubar.cpp
    created:    31/3/2005
    author:     Tomas Lindquist Olsen (based on code by Paul D Turner)
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
#include "WLMenubar.h"
#include "CEGUIImagesetManager.h"
#include "CEGUIImageset.h"


// Start of CEGUI namespace section
namespace CEGUI
{

const utf8 WLMenubar::WidgetTypeName[] = "WindowsLook/Menubar";

const utf8 WLMenubar::ImagesetName[] = "WindowsLook";
const utf8 WLMenubar::BackgroundImageName[] = "Background";


/*************************************************************************
	Constructor for WLMenubar widgets.	
*************************************************************************/
WLMenubar::WLMenubar(const String& type, const String& name) :
	Menubar(type, name)
{
	d_backgroundColours.d_top_left		= 0xFFDFEFF5;
	d_backgroundColours.d_top_right		= 0xFFDFDFF5;
	d_backgroundColours.d_bottom_left	= 0xFFF4F3F5;
	d_backgroundColours.d_bottom_right	= 0xFFF4F3F5;
}


/*************************************************************************
	Destructor for WLMenubar widgets.
*************************************************************************/
WLMenubar::~WLMenubar(void)
{
}


/*************************************************************************
	Resizes the menu bar to exactly fit the items attached to it.
*************************************************************************/
void WLMenubar::sizeToContent_impl(void)
{
	// no special decoration.
	setSize(Absolute,getContentSize());
}


/*************************************************************************
	Return the un-clipped pixel Rect available for rendering menu items
*************************************************************************/
Rect WLMenubar::getItemRenderArea(void) const
{
	Rect r = getUnclippedPixelRect();
	return Rect(0,0,r.getWidth(),r.getHeight());
}


/*************************************************************************
	Perform the actual rendering of this WLMenubar
*************************************************************************/
void WLMenubar::populateRenderCache()
{
	const Image& white = ImagesetManager::getSingleton().getImageset(ImagesetName)->getImage(BackgroundImageName);

	ColourRect cols = d_backgroundColours;
	cols.modulateAlpha(getEffectiveAlpha());

    d_renderCache.cacheImage(white, getAbsoluteRect(), 0, cols);
}


//////////////////////////////////////////////////////////////////////////
/*************************************************************************
	Factory Methods
*************************************************************************/
//////////////////////////////////////////////////////////////////////////

Window* WLMenubarFactory::createWindow(const String& name)
{
    return new WLMenubar(d_type, name);
}


void WLMenubarFactory::destroyWindow(Window* window)
{
    if (window->getType() == d_type)
    {
		delete window;
	}
}

} // End of  CEGUI namespace section
