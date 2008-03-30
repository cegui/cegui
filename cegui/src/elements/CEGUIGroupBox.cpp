/************************************************************************
	filename: 	CEGUIGroupBox.cpp
	created:	03/23/2004
	author:		Lars 'Levia' Wesselius (Content Pane based on Tomas Lindquist Olsen's code)

	purpose:	Implementation of base class for the groupbox
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
#include "CEGUIWindowManager.h"
#include "elements/CEGUIGroupBox.h"

// Start of CEGUI namespace section
namespace CEGUI
{

	const String GroupBox::EventNamespace("GroupBox");
	const String GroupBox::WidgetTypeName("CEGUI/GroupBox");
	const String GroupBox::ContentPaneNameSuffix("__auto_contentpane__");

	/*************************************************************************
	Base class constructor
	*************************************************************************/
GroupBox::GroupBox(const String& type, const String& name) :
	Window(type, name)
{
    // When clicked, don't rise. Required because a Groupbox does not have an actual parent child
    // relation with the widgets which appear inside it.
    d_riseOnClick = false;
}

GroupBox::~GroupBox()
{
}

void GroupBox::initialiseComponents()
{   // Add the auto-child which got defined in the looknfeel
	Window::addChild_impl(getContentPane());
	Window::initialiseComponents();
}

void GroupBox::addChild_impl(Window* wnd)
{
    // Only add it when it's not the __auto_contentpane__ (auto-child) itself
	if (wnd && wnd->getName().find(ContentPaneNameSuffix) == String::npos)
	{
		Window * pane = getContentPane();
		if (pane)
		{
			pane->addChildWindow(wnd);
		}
		else
		{
			Window::addChild_impl(wnd);
		}
	}
}

void GroupBox::removeChild_impl(Window* wnd)
{
	if (wnd)
	{   // Auto pane itself?
        if (wnd->getName().find(ContentPaneNameSuffix) != String::npos)
        {   // Yes
            Window::removeChild_impl(wnd);
            WindowManager::getSingleton().destroyWindow(wnd);
        }
        else
        {   // Remove child from out auto pane
            Window* wndPane = getContentPane();
            if (wndPane)
            {
                wndPane->removeChildWindow(wnd);
		        if (wnd->isDestroyedByParent())
		        {
			        WindowManager::getSingleton().destroyWindow(wnd);
		        }
            }
        }
	}
}

Window * GroupBox::getContentPane() const
{
    String paneName = d_name + ContentPaneNameSuffix;
    WindowManager* winMgr = WindowManager::getSingletonPtr();
    if (winMgr->isWindowPresent(paneName))
    {
        return winMgr->getWindow(paneName);
    }
    return 0;
}

bool GroupBox::drawAroundWidget(const CEGUI::Window * wnd)
{
    Logger::getSingleton().logEvent("TODO: GroupBox::drawAroundWidget");
    return true;
	/*if (!wnd)
	{
		return false;
	}
	UVector2 widgetSize = wnd->getSize();
	UVector2 widgetPosition = wnd->getPosition();
	UVector2 newSize = widgetSize;
	newSize.d_x.d_scale = widgetSize.d_x.d_scale + 0.04f;
	newSize.d_y.d_scale = widgetSize.d_y.d_scale + 0.06f;
	UVector2 newPos = widgetPosition;
	newPos.d_x.d_scale = widgetPosition.d_x.d_scale - 0.02f;
	newPos.d_y.d_scale = widgetPosition.d_y.d_scale - 0.04f;

	this->setSize(newSize);
	this->setPosition(newPos);

	return true;*/
}

bool GroupBox::drawAroundWidget(const String& name)
{
	return drawAroundWidget(WindowManager::getSingletonPtr()->getWindow(name));
}


} // End of  CEGUI namespace section
