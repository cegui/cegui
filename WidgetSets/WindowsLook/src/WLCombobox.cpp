/************************************************************************
	filename: 	WLCombobox.cpp
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
#include "WLCombobox.h"
#include "WLButton.h"
#include "CEGUIWindowManager.h"
#include "CEGUIImagesetManager.h"
#include "CEGUIImageset.h"
#include "CEGUIFont.h"
#include "WLEditbox.h"
#include "WLComboDropList.h"


// Start of CEGUI namespace section
namespace CEGUI
{
/*************************************************************************
	Constants
*************************************************************************/
const utf8	WLCombobox::WidgetTypeName[]				= "WindowsLook/Combobox";

// image / imageset related
const utf8	WLCombobox::ImagesetName[]					= "WindowsLook";
const utf8	WLCombobox::ButtonNormalImageName[]			= "LargeDownArrow";
const utf8	WLCombobox::ButtonHighlightedImageName[]	= "LargeDownArrow";

// component widget type names
const utf8*	WLCombobox::EditboxTypeName		= WLEditbox::WidgetTypeName;
const utf8*	WLCombobox::DropListTypeName	= WLComboDropList::WidgetTypeName;
const utf8*	WLCombobox::ButtonTypeName		= WLButton::WidgetTypeName;


/*************************************************************************
	Constructor for WindowsLook Combobox
*************************************************************************/
WLCombobox::WLCombobox(const String& type, const String& name) :
	Combobox(type, name)
{
}


/*************************************************************************
	Destructor for WindowsLook Combobox
*************************************************************************/
WLCombobox::~WLCombobox(void)
{
}


/*************************************************************************
	Perform rendering for this widget
*************************************************************************/
void WLCombobox::drawSelf(float z)
{
	// no need to do anything here, since we are just a bunch of child
	// widgets.
}


/*************************************************************************
	Setup size and position for the component widgets attached to this
	Combobox.	
*************************************************************************/
void WLCombobox::layoutComponentWidgets()
{
	Point	pos;
	Size	sz;

	float ebheight = getFont()->getLineSpacing() * 1.5f;

	// set the button size
	sz.d_height = sz.d_width = ebheight;
	d_button->setSize(sz);

	// set-up edit box
	pos.d_x = pos.d_y = 0;
	d_editbox->setPosition(pos);

	sz.d_width = getAbsoluteWidth() - ebheight;
	d_editbox->setSize(sz);

	// set button position
	pos.d_x = sz.d_width;
	d_button->setPosition(pos);

	// set list position and size (relative)
	pos.d_x = 0;
	pos.d_y = (getAbsoluteHeight() == 0.0f) ? 0.0f : (ebheight / getAbsoluteHeight());
	d_droplist->setPosition(pos);

	sz.d_width	= 1.0f;
	sz.d_height	= 1.0f - pos.d_y;
	d_droplist->setSize(sz);
}


/*************************************************************************
	Create, initialise, and return a pointer to an Editbox widget to be
	used as part of this Combobox.
*************************************************************************/
Editbox* WLCombobox::createEditbox(void) const
{
	Editbox* eb = (Editbox*)WindowManager::getSingleton().createWindow(EditboxTypeName, getName() + "__auto_editbox__");
	eb->setMetricsMode(Absolute);

	return eb;
}


/*************************************************************************
	Create, initialise, and return a pointer to a PushButton widget to
	be used as part of this Combobox.
*************************************************************************/
PushButton* WLCombobox::createPushButton(void) const
{
	WLButton* btn = (WLButton*)WindowManager::getSingleton().createWindow(ButtonTypeName, getName() + "__auto_button__");
	btn->setMetricsMode(Absolute);

	// Set up imagery
	btn->setStandardImageryEnabled(true);
	btn->setCustomImageryAutoSized(true);
	btn->setAlwaysOnTop(true);

	RenderableImage img;
	img.setHorzFormatting(RenderableImage::HorzCentred);
	img.setVertFormatting(RenderableImage::VertCentred);
	img.setImage(&ImagesetManager::getSingleton().getImageset(ImagesetName)->getImage(ButtonNormalImageName));
	btn->setNormalImage(&img);
	btn->setDisabledImage(&img);

	img.setImage(&ImagesetManager::getSingleton().getImageset(ImagesetName)->getImage(ButtonHighlightedImageName));
	btn->setHoverImage(&img);
	btn->setPushedImage(&img);

	return btn;
}


/*************************************************************************
	Create, initialise, and return a pointer to a ComboDropList widget
	to be used as part of this Combobox.
*************************************************************************/
ComboDropList* WLCombobox::createDropList(void) const
{
	return (ComboDropList*)WindowManager::getSingleton().createWindow(DropListTypeName, getName() + "__auto_droplist__");
}


//////////////////////////////////////////////////////////////////////////
/*************************************************************************

	Factory Methods

*************************************************************************/
//////////////////////////////////////////////////////////////////////////
/*************************************************************************
	Create, initialise and return a WLCombobox
*************************************************************************/
Window* WLComboboxFactory::createWindow(const String& name)
{
	WLCombobox* wnd = new WLCombobox(d_type, name);
	wnd->initialise();

	return wnd;
}

} // End of  CEGUI namespace section
