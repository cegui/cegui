/************************************************************************
	filename: 	CEGUIMenuBase.cpp
	created:	5/4/2005
	author:		Tomas Lindquist Olsen (based on code by Paul D Turner)
	
	purpose:	Implementation of MenuBase widget base class
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
	// add new events specific to MenuBase.
	addMenuBaseEvents();
	
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
	Add MenuBase specific events	
*************************************************************************/
void MenuBase::addMenuBaseEvents(void)
{
	addEvent(EventPopupOpened);
	addEvent(EventPopupClosed);
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

} // End of  CEGUI namespace section
