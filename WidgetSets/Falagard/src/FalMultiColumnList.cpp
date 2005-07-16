/************************************************************************
    filename:   FalMultiColumnList.cpp
    created:    Wed Jul 6 2005
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
#include "FalMultiColumnList.h"
#include "falagard/CEGUIFalWidgetLookManager.h"
#include "falagard/CEGUIFalWidgetLookFeel.h"
#include "CEGUIWindowManager.h"
#include "elements/CEGUIScrollbar.h"
#include "elements/CEGUIListHeader.h"

// Start of CEGUI namespace section
namespace CEGUI
{
    const utf8 FalagardMultiColumnList::WidgetTypeName[] = "Falagard/MultiColumnList";


    FalagardMultiColumnList::FalagardMultiColumnList(const String& type, const String& name) :
        MultiColumnList(type, name)
    {
    }

    FalagardMultiColumnList::~FalagardMultiColumnList()
    {
    }

    Rect FalagardMultiColumnList::getListRenderArea(void) const
    {
        const WidgetLookFeel& wlf = WidgetLookManager::getSingleton().getWidgetLook(d_lookName);
        return wlf.getNamedArea("ItemRenderingArea").getArea().getPixelRect(*this);
    }

    ListHeader* FalagardMultiColumnList::createListHeader(const String& name) const
    {
        // return component created by look'n'feel assignment.
        return static_cast<ListHeader*>(WindowManager::getSingleton().getWindow(name));
    }

    Scrollbar* FalagardMultiColumnList::createVertScrollbar(const String& name) const
    {
        // return component created by look'n'feel assignment.
        return static_cast<Scrollbar*>(WindowManager::getSingleton().getWindow(name));
    }

    Scrollbar* FalagardMultiColumnList::createHorzScrollbar(const String& name) const
    {
        // return component created by look'n'feel assignment.
        return static_cast<Scrollbar*>(WindowManager::getSingleton().getWindow(name));
    }

    void FalagardMultiColumnList::cacheListboxBaseImagery()
    {
        const StateImagery* imagery;

        // get WidgetLookFeel for the assigned look.
        const WidgetLookFeel& wlf = WidgetLookManager::getSingleton().getWidgetLook(d_lookName);
        // try and get imagery for our current state
        imagery = &wlf.getStateImagery(d_enabled ? "Enabled" : "Disabled");
        // peform the rendering operation.
        imagery->render(*this);
    }


    //////////////////////////////////////////////////////////////////////////
    /*************************************************************************

        Factory Methods

    *************************************************************************/
    //////////////////////////////////////////////////////////////////////////
    Window* FalagardMultiColumnListFactory::createWindow(const String& name)
    {
        return new FalagardMultiColumnList(d_type, name);
    }

    void FalagardMultiColumnListFactory::destroyWindow(Window* window)
    {
        if (window->getType() == d_type)
            delete window;
    }

} // End of  CEGUI namespace section
