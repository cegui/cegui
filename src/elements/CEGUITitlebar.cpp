/************************************************************************
	filename: 	CEGUITitlebar.cpp
	created:	25/4/2004
	author:		Paul D Turner
	
	purpose:	Implementation of common Titlebar parts.
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
#include "elements/CEGUITitlebar.h"
#include "elements/CEGUIFrameWindow.h"
#include "CEGUIMouseCursor.h"

// Start of CEGUI namespace section
namespace CEGUI
{
/*************************************************************************
	Constructor
*************************************************************************/
Titlebar::Titlebar(const String& type, const String& name) :
	Window(type, name)
{
	setAlwaysOnTop(true);

	// basic initialisation
	d_dragging = false;
}

/*************************************************************************
	Destructor
*************************************************************************/
Titlebar::~Titlebar(void)
{
}


/*************************************************************************
	Handler for mouse movement events
*************************************************************************/
void Titlebar::onMouseMove(MouseEventArgs& e)
{
	// Base class processing.
	Window::onMouseMove(e);

	if (d_dragging && (d_parent != NULL))
	{
		Vector2 delta(screenToWindow(e.position));

		if (getMetricsMode() == Relative)
		{
			delta = relativeToAbsolute(delta);
		}

		// calculate amount that window has been moved
		delta -= d_dragPoint;

		// move the window.  *** Again: Titlebar objects should only be attached to FrameWindow derived classes. ***
		((FrameWindow*)d_parent)->offsetPixelPosition(delta);

		e.handled = true;
	}
}


/*************************************************************************
	Handler for mouse button press events
*************************************************************************/
void Titlebar::onMouseButtonDown(MouseEventArgs& e)
{
	// Base class processing
	Window::onMouseButtonDown(e);

	if (e.button == LeftButton)
	{
		if (d_parent != NULL)
		{
			// we want all mouse inputs from now on
			captureInput();

			// initialise the dragging state
			d_dragging = true;
			d_dragPoint = screenToWindow(e.position);

			if (getMetricsMode() == Relative)
			{
				d_dragPoint = relativeToAbsolute(d_dragPoint);
			}

			// store old constraint area
			d_oldCursorArea = MouseCursor::getSingleton().getConstraintArea();

			// setup new constraint area to be the intersection of the old area and our grand-parent's clipped inner-area
			if ((d_parent == NULL) || (d_parent->getParent() == NULL))
			{
				MouseCursor::getSingleton().setConstraintArea(&System::getSingleton().getRenderer()->getRect().getIntersection(d_oldCursorArea));
			}
			else 
			{
				MouseCursor::getSingleton().setConstraintArea(&d_parent->getParent()->getInnerRect().getIntersection(d_oldCursorArea));
			}

		}

		e.handled = true;
	}
}


/*************************************************************************
	Handler for mouse button release events
*************************************************************************/
void Titlebar::onMouseButtonUp(MouseEventArgs& e)
{
	// Base class processing
	Window::onMouseButtonUp(e);

	if (e.button == LeftButton)
	{
		releaseInput();
		e.handled = true;
	}

}


/*************************************************************************
	Handler for mouse button double-click events
*************************************************************************/
void Titlebar::onMouseDoubleClicked(MouseEventArgs& e)
{
	// Base class processing
	Window::onMouseDoubleClicked(e);

	if (e.button == LeftButton)
	{
		// if we do not have a parent window, then obviously nothing should happen.
		if (d_parent != NULL)
		{
			// we should only ever be attached to a FrameWindow (or derived) class
			((FrameWindow*)d_parent)->toggleRollup();
		}

		e.handled = true;
	}

}


/*************************************************************************
	Handler for if the window loses capture of the mouse.
*************************************************************************/
void Titlebar::onCaptureLost(EventArgs& e)
{
	// Base class processing
	Window::onCaptureLost(e);

	// when we lose out hold on the mouse inputs, we are no longer dragging.
	d_dragging = false;

	// restore old constraint area
	MouseCursor::getSingleton().setConstraintArea(&d_oldCursorArea);
}

} // End of  CEGUI namespace section
