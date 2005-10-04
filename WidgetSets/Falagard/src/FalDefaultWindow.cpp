/************************************************************************
    filename:   FalItemEntry.cpp
    created:    Thu Sep 22 2005
    author:     Tomas Lindquist Olsen
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
#include "FalDefaultWindow.h"
#include "falagard/CEGUIFalWidgetLookManager.h"
#include "falagard/CEGUIFalWidgetLookFeel.h"

// Start of CEGUI namespace section
namespace CEGUI
{
    const utf8 FalagardDefaultWindow::WidgetTypeName[] = "Falagard/DefaultWindow";

    FalagardDefaultWindow::FalagardDefaultWindow(const String& type, const String& name) :
        DefaultWindow(type, name)
    {
    }

    FalagardDefaultWindow::~FalagardDefaultWindow()
    {
    }

    void FalagardDefaultWindow::populateRenderCache()
    {
        // get WidgetLookFeel for the assigned look.
        const WidgetLookFeel& wlf = WidgetLookManager::getSingleton().getWidgetLook(d_lookName);
        // render basic imagery
        wlf.getStateImagery(isDisabled() ? "Disabled" : "Enabled").render(*this);
    }

    void FalagardDefaultWindow::drawSelf(float z)
    {
        // we want back the default functionality from Window and not the empty override from GUISheet
        // a little hackish maybe...
        Window::drawSelf(z);
    }

} // End of  CEGUI namespace section
