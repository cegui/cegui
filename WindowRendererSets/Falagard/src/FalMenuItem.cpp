/************************************************************************
    filename:   FalMenuItem.cpp
    created:    Fri Jul 8 2005
    author:     Paul D Turner <paul@cegui.org.uk>
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
#include "FalMenuItem.h"
#include "falagard/CEGUIFalWidgetLookManager.h"
#include "falagard/CEGUIFalWidgetLookFeel.h"
#include "elements/CEGUIMenuItem.h"

// Start of CEGUI namespace section
namespace CEGUI
{
    const utf8 FalagardMenuItem::TypeName[] = "Falagard/MenuItem";

    FalagardMenuItem::FalagardMenuItem(const String& type) :
        ItemEntryWindowRenderer(type)
    {
    }

    Size FalagardMenuItem::getItemPixelSize() const
    {
        MenuItem* w = (MenuItem*)d_window;
        Window* parent = w->getParent();
		bool not_menubar = (!parent) ? true : !parent->testClassName("Menubar");
		const WidgetLookFeel& wlf = getLookNFeel();
		const NamedArea* area;
		if (w->getPopupMenu() && not_menubar && wlf.isNamedAreaDefined("HasPopupContentSize"))
		{
		    area = &wlf.getNamedArea("HasPopupContentSize");
		}
		else
		{
		    area = &wlf.getNamedArea("ContentSize");
		}
        return area->getArea().getPixelRect(*w).getSize();
    }

    void FalagardMenuItem::render()
    {
        MenuItem* w = (MenuItem*)d_window;
        // build name of state we're in
        String stateName(w->isDisabled() ? "Disabled" : "Enabled");

        if (w->isOpened())
            stateName += "PopupOpen";
        else if (w->isPushed())
            stateName += "Pushed";
        else if (w->isHovering())
            stateName += "Hover";
        else
            stateName += "Normal";

        const StateImagery* imagery;
        // get WidgetLookFeel for the assigned look.
        const WidgetLookFeel& wlf = getLookNFeel();
        // try and get imagery for our current state
        imagery = &wlf.getStateImagery(stateName);
        // peform the rendering operation.
        imagery->render(*w);

		// only draw popup-open/closed-icon if we have a popup menu, and parent is not a menubar
		Window* parent_window = w->getParent();
		bool not_menubar = (!parent_window) ? true : !parent_window->testClassName("Menubar");
        if (w->getPopupMenu() && not_menubar)
        {
            // get imagery for popup open/closed state
            imagery = &wlf.getStateImagery(w->isOpened() ? "PopupOpenIcon" : "PopupClosedIcon");
            // peform the rendering operation.
            imagery->render(*w);
        }
    }

}
