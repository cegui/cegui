/************************************************************************
	filename: 	CEGUIMenuItem.cpp
	created:	2/4/2005
	author:		Tomas Lindquist Olsen (based on code by Paul D Turner)
	
	purpose:	Implementation of MenuItem widget base class
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
#include "elements/CEGUIMenuItem.h"
#include "elements/CEGUIMenubar.h"
#include "elements/CEGUIPopupMenu.h"

#include "CEGUILogger.h"
#include "CEGUIWindowManager.h"

// Start of CEGUI namespace section
namespace CEGUI
{

/*************************************************************************
	Constants
*************************************************************************/
// event strings
const String MenuItem::WidgetTypeName("CEGUI/MenuItem");
const String MenuItem::EventNamespace("MenuItem");
const String MenuItem::EventClicked("Clicked");


/*************************************************************************
	Constructor for MenuItem base class.
*************************************************************************/
MenuItem::MenuItem(const String& type, const String& name)
	: ItemEntry(type, name),
	d_pushed(false),
	d_hovering(false),
	d_opened(false),
	d_popup(0)
{
	// menuitems dont want multi-click events
	setWantsMultiClickEvents(false);
}


/*************************************************************************
	Destructor for MenuItem base class.
*************************************************************************/
MenuItem::~MenuItem(void)
{
}


/*************************************************************************
	Update the internal state of the Widget
*************************************************************************/
void MenuItem::updateInternalState(const Point& mouse_pos)
{
	bool oldstate = d_hovering;

	// assume not hovering 
	d_hovering = false;

	// if input is captured, but not by 'this', then we never hover highlight
	const Window* capture_wnd = getCaptureWindow();

	if ((capture_wnd == 0) || (capture_wnd == this))
	{
		Window* sheet = System::getSingleton().getGUISheet();

		if (sheet)
		{
			// check if hovering highlight is required, which is basically ("mouse over widget" XOR "widget pushed").
			if ((this == sheet->getChildAtPosition(mouse_pos)) != d_pushed)
			{
				d_hovering = true;

				// are we attached to a menu ?
				if (getParent()->testClassName("MenuBase"))
				{
					MenuBase* menu = (MenuBase*)getParent();

					// is item really in list ?
					if (menu->isItemInList(this))
					{
						// does this menubar only allow one popup open? and is there a popup open?
						if ( !menu->isMultiplePopupsAllowed() && menu->getPopupMenuItem()!=0 )
						{
							// open this popup instead
							openPopupMenu();
						}

					}

				}

			}

		}

	}

	// if state has changed, trigger a re-draw
	if (oldstate != d_hovering)
	{
		requestRedraw();
	}

}


/*************************************************************************
	Set the popup menu for this item.
*************************************************************************/
void MenuItem::setPopupMenu(PopupMenu* popup)
{
    setPopupMenu_impl(popup);
}


/*************************************************************************
	Set the popup menu for this item.
*************************************************************************/
void MenuItem::setPopupMenu_impl(PopupMenu* popup, bool add_as_child)
{
    // is it the one we have already ?
    if (popup==d_popup)
    {
        // then do nothing;
        return;
    }

    // keep the old one around
    PopupMenu* old_popup = d_popup;
    // update the internal state pointer
	d_popup = popup;
	d_opened = false;

    // is there already a popup ?
    if (old_popup)
    {
        removeChildWindow(old_popup);
        // should we destroy it as well?
        if (old_popup->isDestroyedByParent())
        {
            // then do so
            WindowManager::getSingletonPtr()->destroyWindow(old_popup);
        }
    }

    // we are setting a new popup and not just clearing. and we are told to add the child
    if (popup!=0 && add_as_child)
    {
        addChildWindow(popup);
	}

    requestRedraw();
}

/*************************************************************************
	Open the PopupMenu attached to this item.
*************************************************************************/
void MenuItem::openPopupMenu(bool notify)
{
	// no popup? or already open...
	if ( d_popup == 0 || d_opened )
		return;

    // should we notify ?
	// if so, and we are attached to a menu bar or popup menu, we let it handle the "activation"
	Window* p = getParent();
	if (notify && p)
	{
	    if (p->testClassName("Menubar"))
	    {
		    // align the popup to the bottom-left of the menuitem
		    UVector2 pos(cegui_absdim(0), cegui_absdim(d_pixelSize.d_height));
		    d_popup->setWindowPosition(pos);

		    static_cast<Menubar*>(p)->changePopupMenuItem(this);
		    return; // the rest is handled when the menu bar eventually calls us itself
	    }
	    // or maybe a popup menu?
	    else if (p->testClassName("PopupMenu"))
	    {
		    // align the popup to the top-right of the menuitem
            UVector2 pos(cegui_absdim(d_pixelSize.d_width), cegui_absdim(0));
            d_popup->setWindowPosition(pos);

		    static_cast<PopupMenu*>(p)->changePopupMenuItem(this);
		    return; // the rest is handled when the popup menu eventually calls us itself
	    }
	}

	// by now we must handle it ourselves
	// match up with Menubar::changePopupMenu
    d_popup->openPopupMenu(false);

	d_opened = true;
	requestRedraw();
}


/*************************************************************************
	Close the PopupMenu attached to this item.
*************************************************************************/
void MenuItem::closePopupMenu(bool notify)
{
	// no popup? or not open...
	if ( d_popup == 0 || !d_opened )
		return;

	// should we notify the parent menu base?
	// if we are attached to a menu base, we let it handle the "deactivation"
	Window* p = getParent();
	if (notify && p->testClassName("MenuBase"))
	{
	    MenuBase* menu = static_cast<MenuBase*>(p);
	    // only if the menu base does not allow multiple popups
	    if (!menu->isMultiplePopupsAllowed())
	    {
	        menu->changePopupMenuItem(0);
		    return; // the rest is handled when the menu base eventually call us again itself
	    }
	}
	// otherwise we do ourselves
	else
	{
		// match up with Menubar::changePopupMenu
		//d_popup->hide();
		d_popup->closePopupMenu(false);
	}

	d_opened = false;
	requestRedraw();
}


/*************************************************************************
	Toggles the PopupMenu.
*************************************************************************/
bool MenuItem::togglePopupMenu(void)
{
	if (d_opened)
	{
		closePopupMenu();
		return false;
	}

	openPopupMenu();
	return true;
}


/*************************************************************************
	Recursive function that closes all popups down the hierarchy starting
	with this one.
*************************************************************************/
void MenuItem::closeAllMenuItemPopups()
{
	// are we attached to a PopupMenu?
	Window* p = getParent();
	if (p && p->testClassName("PopupMenu"))
	{
	    PopupMenu* pop = static_cast<PopupMenu*>(p);
		// is this parent popup attached to a menu item?
		p = pop->getParent();
		if (p && p->testClassName("MenuItem"))
		{
		    MenuItem* mi = static_cast<MenuItem*>(p);
			// close popup
			mi->closePopupMenu();
			// recurse
			mi->closeAllMenuItemPopups();
		}
		// otherwise we just hide the parent popup
		else
		{
			pop->closePopupMenu(false);
		}
	}
}


/*************************************************************************
	handler invoked internally when the menuitem is clicked.	
*************************************************************************/
void MenuItem::onClicked(WindowEventArgs& e)
{
	// close the popup if we did'nt spawn a child
	if (!d_opened && !d_popupWasClosed)
	{
		closeAllMenuItemPopups();
	}
	d_popupWasClosed=false;
	fireEvent(EventClicked, e, EventNamespace);
}


/*************************************************************************
	Handler for when the mouse moves
*************************************************************************/
void MenuItem::onMouseMove(MouseEventArgs& e)
{
	// this is needed to discover whether mouse is in the widget area or not.
	// The same thing used to be done each frame in the rendering method,
	// but in this version the rendering method may not be called every frame
	// so we must discover the internal widget state here - which is actually
	// more efficient anyway.

	// base class processing
	ItemEntry::onMouseMove(e);

	updateInternalState(e.position);
	e.handled = true;
}


/*************************************************************************
	Handler for mouse button pressed events
*************************************************************************/
void MenuItem::onMouseButtonDown(MouseEventArgs& e)
{
	// default processing
	ItemEntry::onMouseButtonDown(e);

	if (e.button == LeftButton)
	{
		d_popupWasClosed = false;
		if (captureInput())
		{
			d_pushed = true;
			updateInternalState(e.position);
			d_popupWasClosed = !togglePopupMenu();
			requestRedraw();
		}

		// event was handled by us.
		e.handled = true;
	}

}


/*************************************************************************
	Handler for mouse button release events
*************************************************************************/
void MenuItem::onMouseButtonUp(MouseEventArgs& e)
{
	// default processing
	ItemEntry::onMouseButtonUp(e);

	if (e.button == LeftButton)
	{
		releaseInput();
		
		// was the button released over this window?
		if ( !d_popupWasClosed && System::getSingleton().getGUISheet()->getChildAtPosition(e.position) == this )
		{
			WindowEventArgs we(this);
			onClicked(we);
		}

		// event was handled by us.
		e.handled = true;
	}

}

/*************************************************************************
	Handler for when mouse capture is lost
*************************************************************************/
void MenuItem::onCaptureLost(WindowEventArgs& e)
{
	// Default processing
	ItemEntry::onCaptureLost(e);

	d_pushed = false;
	updateInternalState(MouseCursor::getSingleton().getPosition());
	requestRedraw();

	// event was handled by us.
	e.handled = true;
}


/*************************************************************************
	Handler for when mouse leaves the widget
*************************************************************************/
void MenuItem::onMouseLeaves(MouseEventArgs& e)
{
	// deafult processing
	ItemEntry::onMouseLeaves(e);

	d_hovering = false;
	requestRedraw();

	e.handled = true;
}


/*************************************************************************
	Handler called when text is changed.
*************************************************************************/
void MenuItem::onTextChanged(WindowEventArgs& e)
{
	ItemEntry::onTextChanged(e);
	
	// if we are attached to a ItemListBase, we make it update as necessary
	Window* parent = getParent();
	if (parent && parent->testClassName("ItemListBase"))
	{
		static_cast<ItemListBase*>(parent)->handleUpdatedItemData();
	}

	e.handled = true;
}


/*************************************************************************
	Internal version of adding a child window.
*************************************************************************/
void MenuItem::addChild_impl(Window* wnd)
{
	ItemEntry::addChild_impl(wnd);

	// if this is a PopupMenu we add it like one
	if (wnd->testClassName("PopupMenu"))
	{
		setPopupMenu_impl((PopupMenu*)wnd,false);
	}

}

} // End of  CEGUI namespace section
