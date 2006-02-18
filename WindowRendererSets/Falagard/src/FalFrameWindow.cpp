/************************************************************************
    filename:   FalFrameWindow.cpp
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
#include "FalFrameWindow.h"
#include "elements/CEGUIFrameWindow.h"
#include "falagard/CEGUIFalWidgetLookManager.h"
#include "falagard/CEGUIFalWidgetLookFeel.h"
#include "elements/CEGUITitlebar.h"

// Start of CEGUI namespace section
namespace CEGUI
{
    const utf8 FalagardFrameWindow::TypeName[] = "Falagard/FrameWindow";

    FalagardFrameWindow::FalagardFrameWindow(const String& type) :
        WindowRenderer(type)
    {
    }

    void FalagardFrameWindow::render()
    {
        FrameWindow* w = (FrameWindow*)d_window;
        // do not render anything for the rolled-up state.
        if (w->isRolledup())
            return;

        // build state name
        String stateName(w->isDisabled() ? "Disabled" : (w->isActive() ? "Active" : "Inactive"));
        stateName += w->getTitlebar()->isVisible() ? "WithTitle" : "NoTitle";
        stateName += w->isFrameEnabled() ? "WithFrame" : "NoFrame";

        const StateImagery* imagery;

        try
        {
            // get WidgetLookFeel for the assigned look.
            const WidgetLookFeel& wlf = getLookNFeel();
            // try and get imagery for our current state
            imagery = &wlf.getStateImagery(stateName);
        }
        catch (UnknownObjectException)
        {
            // log error so we know imagery is missing, and then quit.
            return;
        }

        // peform the rendering operation.
        imagery->render(*w);
    }

    Rect FalagardFrameWindow::getUnclippedInnerRect(void) const
    {
        FrameWindow* w = (FrameWindow*)d_window;
        if (w->isRolledup())
            return Rect(0,0,0,0);

        // build name of area to fetch
        String areaName("Client");
        areaName += w->getTitlebar()->isVisible() ? "WithTitle" : "NoTitle";
        areaName += w->isFrameEnabled() ? "WithFrame" : "NoFrame";

        // get WidgetLookFeel for the assigned look.
        const WidgetLookFeel& wlf = getLookNFeel();
        return wlf.getNamedArea(areaName).getArea().getPixelRect(*w, w->getUnclippedPixelRect());
    }

} // End of  CEGUI namespace section
