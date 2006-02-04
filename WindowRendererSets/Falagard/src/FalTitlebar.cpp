/************************************************************************
    filename:   FalTitlebar.cpp
    created:    Sat Jul 2 2005
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
#include "FalTitlebar.h"
#include "falagard/CEGUIFalWidgetLookManager.h"
#include "falagard/CEGUIFalWidgetLookFeel.h"
#include "elements/CEGUITitlebar.h"

// Start of CEGUI namespace section
namespace CEGUI
{
    const utf8 FalagardTitlebar::TypeName[] = "Falagard/Titlebar";

    FalagardTitlebar::FalagardTitlebar(const String& type) :
        WindowRenderer(type, "Titlebar")
    {
    }

    void FalagardTitlebar::render()
    {
        const StateImagery* imagery;

        try
        {
            // get WidgetLookFeel for the assigned look.
            const WidgetLookFeel& wlf = getLookNFeel();
            // try and get imagery for our current state
            if (!d_window->isDisabled())
                imagery = &wlf.getStateImagery((d_window->getParent() && d_window->getParent()->isActive()) ? "Active" : "Inactive");
            else
                imagery = &wlf.getStateImagery("Disabled");
        }
        catch (UnknownObjectException)
        {
            // log error so we know imagery is missing, and then quit.
            return;
        }

        // peform the rendering operation.
        imagery->render(*d_window);
    }

    Rect FalagardTitlebar::getPixelRect(void) const
    {
        // clip to grand-parent as needed
        if (d_window->getParent() && d_window->getParent()->getParent() && d_window->isClippedByParent())
        {
            return d_window->getParent()->getParent()->getInnerRect().getIntersection(d_window->getUnclippedPixelRect());
        }
        // clip to screen if no grand-parent, or if clipping has been disabled for us.
        else
        {
            return System::getSingleton().getRenderer()->getRect().getIntersection(d_window->getUnclippedPixelRect());
        }
    }

} // End of  CEGUI namespace section
