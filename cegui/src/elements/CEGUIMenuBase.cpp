/***********************************************************************
	filename: 	CEGUIMenuBase.cpp
	created:	5/4/2005
	author:		Tomas Lindquist Olsen (based on code by Paul D Turner)
	
	purpose:	Implementation of MenuBase widget base class
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2006 Paul D Turner & The CEGUI Development Team
 *
 *   Permission is hereby granted, free of charge, to any person obtaining
 *   a copy of this software and associated documentation files (the
 *   "Software"), to deal in the Software without restriction, including
 *   without limitation the rights to use, copy, modify, merge, publish,
 *   distribute, sublicense, and/or sell copies of the Software, and to
 *   permit persons to whom the Software is furnished to do so, subject to
 *   the following conditions:
 *
 *   The above copyright notice and this permission notice shall be
 *   included in all copies or substantial portions of the Software.
 *
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 *   EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 *   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 *   IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 *   OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 *   ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 *   OTHER DEALINGS IN THE SOFTWARE.
 ***************************************************************************/
#include "elements/CEGUIMenuBase.h"
#include "elements/CEGUIPopupMenu.h"
#include "elements/CEGUIMenuItem.h"

// Start of CEGUI namespace section
namespace CEGUI
{

/*************************************************************************
Definition of Properties for this class
*************************************************************************/
MenuBaseProperties::ItemSpacing			MenuBase::d_itemSpacingProperty;
MenuBaseProperties::AllowMultiplePopups	MenuBase::d_allowMultiplePopupsProperty;


/*************************************************************************
	Constants
*************************************************************************/
// event strings
const String MenuBase::EventNamespace("MenuBase");
const String MenuBase::EventPopupOpened("PopupOpened");
const String MenuBase::EventPopupClosed("PopupClosed");

/*************************************************************************
	Constructor for MenuBase base class.
*************************************************************************/
MenuBase::MenuBase(const String& type, const String& name)
	: ItemListBase(type, name),
	d_itemSpacing(0.0f),
	d_popupItem(0),
	d_allowMultiplePopups(false)
{
	// add properties for MenuBase class
	addMenuBaseProperties();
}


/*************************************************************************
	Destructor for MenuBase base class.
*************************************************************************/
MenuBase::~MenuBase(void)
{
}


/*************************************************************************
	Change the currently open MenuItem PopupMenu
*************************************************************************/
void MenuBase::changePopupMenuItem(MenuItem* item)
{
	if (!d_allowMultiplePopups&&d_popupItem==item)
		return;

	if (!d_allowMultiplePopups&&d_popupItem!=0)
	{
		d_popupItem->closePopupMenu(false);
		WindowEventArgs we(d_popupItem->getPopupMenu());
		d_popupItem = 0;
		onPopupClosed(we);
	}

	if (item)
	{
		d_popupItem = item;
		d_popupItem->openPopupMenu(false);
		WindowEventArgs we(d_popupItem->getPopupMenu());
		onPopupOpened(we);
	}

}


/*************************************************************************
	handler invoked internally when the a MenuItem attached to this
	MenuBase opens its popup.
*************************************************************************/
void MenuBase::onPopupOpened(WindowEventArgs& e)
{
	fireEvent(EventPopupOpened, e, EventNamespace);
}


/*************************************************************************
	handler invoked internally when the a MenuItem attached to this
	MenuBase closes its popup.
*************************************************************************/
void MenuBase::onPopupClosed(WindowEventArgs& e)
{
	fireEvent(EventPopupClosed, e, EventNamespace);
}


/************************************************************************
    Add properties for this widget
*************************************************************************/
void MenuBase::addMenuBaseProperties(void)
{
	addProperty(&d_itemSpacingProperty);
    addProperty(&d_allowMultiplePopupsProperty);
}


/************************************************************************
    Set if multiple child popup menus are allowed simultaneously
*************************************************************************/
void MenuBase::setAllowMultiplePopups(bool setting)
{
    if (d_allowMultiplePopups != setting)
    {
        // TODO :
        // close all popups except perhaps the last one opened!
        d_allowMultiplePopups = setting;
    }
}

//----------------------------------------------------------------------------//
void MenuBase::onChildRemoved(WindowEventArgs& e)
{
    // if the removed window was our tracked popup item, zero ptr to it.
    if (e.window == d_popupItem)
        d_popupItem = 0;
        
    // base class version
    ItemListBase::onChildRemoved(e);
}

//----------------------------------------------------------------------------//

} // End of  CEGUI namespace section
