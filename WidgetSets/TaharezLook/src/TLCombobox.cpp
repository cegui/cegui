/************************************************************************
	filename: 	TLCombobox.cpp
	created:	12/6/2004
	author:		Paul D Turner
	
	purpose:	Implementation of Taharez look Combo box class
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
#include "TLCombobox.h"
#include "TLButton.h"
#include "CEGUIWindowManager.h"
#include "CEGUIImagesetManager.h"
#include "CEGUIImageset.h"
#include "CEGUIFont.h"
#include "elements/CEGUIEditbox.h"
#include "elements/CEGUIComboDropList.h"


// Start of CEGUI namespace section
namespace CEGUI
{
/*************************************************************************
	Constants
*************************************************************************/
// image / imageset related
const utf8	TLCombobox::ImagesetName[]					= "TaharezImagery";
const utf8	TLCombobox::ButtonNormalImageName[]			= "ComboboxListButtonNormal";
const utf8	TLCombobox::ButtonHighlightedImageName[]	= "ComboboxListButtonHover";

// component widget type names
const utf8	TLCombobox::EditboxTypeName[]	= "Taharez ComboEditbox";
const utf8	TLCombobox::DropListTypeName[]	= "Taharez ComboDropList";
const utf8	TLCombobox::ButtonTypeName[]	= "Taharez Button";


/*************************************************************************
	Constructor for Taharez Combobox
*************************************************************************/
TLCombobox::TLCombobox(const String& type, const String& name) :
	Combobox(type, name)
{
}


/*************************************************************************
	Destructor for Taharez Combobox
*************************************************************************/
TLCombobox::~TLCombobox(void)
{
}


/*************************************************************************
	Perform rendering for this widget
*************************************************************************/
void TLCombobox::drawSelf(float z)
{
	// no need to do anything here, since we are just a bunch of child
	// widgets.
}


/*************************************************************************
	Setup size and position for the component widgets attached to this
	Combobox.	
*************************************************************************/
void TLCombobox::layoutComponentWidgets()
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
Editbox* TLCombobox::createEditbox(void) const
{
	Editbox* eb = (Editbox*)WindowManager::getSingleton().createWindow(EditboxTypeName, getName() + "__auto_editbox__");
	eb->setMetricsMode(Absolute);

	return eb;
}


/*************************************************************************
	Create, initialise, and return a pointer to a PushButton widget to
	be used as part of this Combobox.
*************************************************************************/
PushButton* TLCombobox::createPushButton(void) const
{
	TLButton* btn = (TLButton*)WindowManager::getSingleton().createWindow(ButtonTypeName, getName() + "__auto_button__");
	btn->setMetricsMode(Absolute);

	// Set up imagery
	btn->setStandardImageryEnabled(false);
	btn->setCustomImageryAutoSized(true);

	RenderableImage img;
	img.setHorzFormatting(RenderableImage::HorzStretched);
	img.setVertFormatting(RenderableImage::VertStretched);

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
ComboDropList* TLCombobox::createDropList(void) const
{
	return (ComboDropList*)WindowManager::getSingleton().createWindow(DropListTypeName, getName() + "__auto_droplist__");
}


//////////////////////////////////////////////////////////////////////////
/*************************************************************************

	Factory Methods

*************************************************************************/
//////////////////////////////////////////////////////////////////////////
/*************************************************************************
	Create, initialise and return a TLCombobox
*************************************************************************/
Window* TLComboboxFactory::createWindow(const String& name)
{
	TLCombobox* wnd = new TLCombobox(d_type, name);
	wnd->initialise();

	return wnd;
}

} // End of  CEGUI namespace section
