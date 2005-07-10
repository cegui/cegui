/************************************************************************
	filename: 	WLTabControl.cpp
	created:	03/12/2004
	author:		Paul D Turner
	
	purpose:	Implementation of Windows Look static widgets & factories.
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
#include "WLTabControl.h"
#include "CEGUIWindowManager.h"
#include "WLTabButton.h"
#include "WLTabPane.h"
#include "CEGUIImageset.h"


// Start of CEGUI namespace section
namespace CEGUI
{
/*************************************************************************
	Constants
*************************************************************************/
// type names for this widget
const utf8	WLTabControl::WidgetTypeName[]		= "WindowsLook/TabControl";
const utf8	WLTabControl::ImagesetName[]		= "WindowsLook";
const utf8	WLTabControl::FillerImageName[]		= "TabControlButtonPaneFiller";

// window type stuff
const utf8*	WLTabControl::TabContentPaneType	= WLTabPane::WidgetTypeName;
const utf8*	WLTabControl::TabButtonType		    = WLTabButton::WidgetTypeName;


/*************************************************************************
	Constructor
*************************************************************************/
WLTabControl::WLTabControl(const String& type, const String& name)
    :TabControl(type, name)
{
    Imageset* iset = ImagesetManager::getSingleton().getImageset(ImagesetName);

    // setup cache of image pointers
    d_fillerImage		    = &iset->getImage(FillerImageName);
}


/*************************************************************************
	Destructor
*************************************************************************/
WLTabControl::~WLTabControl(void)
{
}


/*************************************************************************
	Create a content pane
*************************************************************************/
TabPane* WLTabControl::createTabContentPane(const String& name) const
{
    return static_cast<TabPane*>(
        WindowManager::getSingleton().createWindow(
            TabContentPaneType, 
            name)
            );

}


/*************************************************************************
	Create a tab button
*************************************************************************/
TabButton* WLTabControl::createTabButton(const String& name) const
{
    return static_cast<TabButton*>(
        WindowManager::getSingleton().createWindow(
            TabButtonType, name ) );

}


/*************************************************************************
	Layout components
*************************************************************************/
void WLTabControl::layoutComponentWidgets()
{
    TabControl::layoutComponentWidgets();

    // Create the 'filler'
    if (d_tabButtonPane)
    {
        // Calculate the positions and sizes of the tab buttons
		d_fillerSize.d_width = d_tabButtonPane->getWidth(Absolute);
		d_fillerSize.d_height = d_fillerImage->getHeight();
        d_fillerPos.d_x = d_tabButtonPane->getAbsoluteXPosition();
        d_fillerPos.d_y = d_tabButtonPane->getAbsoluteYPosition() + d_tabButtonPane->getAbsoluteHeight() - d_fillerSize.d_height;

		if (getTabCount() > 0)
		{
			TabButtonIndexMap::iterator iter = d_tabButtonIndexMap.end();
			std::advance(iter, -1);
			Window* btn = iter->second;

			d_fillerPos.d_x = d_tabButtonPane->getAbsoluteXPosition() + 
				btn->getAbsoluteXPosition() + btn->getWidth(Absolute);

			d_fillerSize.d_width -= d_fillerPos.d_x;
		}

    }

}


/*************************************************************************
	Draw
*************************************************************************/
void WLTabControl::drawSelf(float z)
{
    TabControl::drawSelf(z);

    Rect clipper(getPixelRect());

    // do nothing if the widget is totally clipped.
    if (clipper.getWidth() == 0)
    {
        return;
    }

    // get the destination screen rect for this window
    Rect absrect(getUnclippedPixelRect());

	// calculate colours to use.
	ColourRect colours(colour(1, 1, 1, getEffectiveAlpha()));

    // Do filler section
    Vector3 pos = d_fillerPos;
    pos.d_x += absrect.d_left;
    pos.d_y += absrect.d_top;
    pos.d_z = z;
    d_fillerImage->draw(pos, d_fillerSize, clipper, colours);


}


//////////////////////////////////////////////////////////////////////////
/*************************************************************************

	Factory Methods

*************************************************************************/
//////////////////////////////////////////////////////////////////////////
/*************************************************************************
	Create, initialise and return a TabControl for the Windows Scheme
*************************************************************************/
Window* WLTabControlFactory::createWindow(const String& name)
{
	return new WLTabControl(d_type, name);
}

} // End of  CEGUI namespace section
