/************************************************************************
	filename: 	CEGUIFrameWindow.cpp
	created:	13/4/2004
	author:		Paul D Turner
	
	purpose:	Implementation of FrameWindow base class
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
#include "elements/CEGUIFrameWindow.h"
#include "elements/CEGUITitlebar.h"
#include "elements/CEGUIPushButton.h"
#include "CEGUIMouseCursor.h"


// Start of CEGUI namespace section
namespace CEGUI
{
/*************************************************************************
	Definitions for Properties
*************************************************************************/
FrameWindowProperties::SizingEnabled			FrameWindow::d_sizingEnabledProperty;
FrameWindowProperties::FrameEnabled				FrameWindow::d_frameEnabledProperty;
FrameWindowProperties::TitlebarEnabled			FrameWindow::d_titlebarEnabledProperty;
FrameWindowProperties::CloseButtonEnabled		FrameWindow::d_closeButtonEnabledProperty;
FrameWindowProperties::RollUpState				FrameWindow::d_rollUpStateProperty;
FrameWindowProperties::RollUpEnabled			FrameWindow::d_rollUpEnabledProperty;
FrameWindowProperties::DragMovingEnabled		FrameWindow::d_dragMovingEnabledProperty;
FrameWindowProperties::SizingBorderThickness	FrameWindow::d_sizingBorderThicknessProperty;
FrameWindowProperties::TitlebarFont				FrameWindow::d_titlebarFontProperty;
FrameWindowProperties::CaptionColour			FrameWindow::d_captionColourProperty;


/*************************************************************************
	Constants
*************************************************************************/
// additional event names for this window
const utf8	FrameWindow::EventRollupToggled[]	= "RollupToggled";
const utf8	FrameWindow::EventCloseClicked[]		= "CloseClicked";

// other bits
const float FrameWindow::DefaultSizingBorderSize	= 8.0f;


/*************************************************************************
	Constructor
*************************************************************************/
FrameWindow::FrameWindow(const String& type, const String& name) :
	Window(type, name)
{
	d_frameEnabled		= true;
	d_rollupEnabled		= true;
	d_rolledup			= false;
	d_sizingEnabled		= true;
	d_beingSized		= false;
	d_dragMovable		= true;

	d_borderSize		= DefaultSizingBorderSize;

	d_nsSizingCursor = d_ewSizingCursor = d_neswSizingCursor = d_nwseSizingCursor = NULL;

	addFrameWindowEvents();
	addFrameWindowProperties();
}


/*************************************************************************
	Destructor
*************************************************************************/
FrameWindow::~FrameWindow(void)
{
}


/*************************************************************************
	Initialises the Window based object ready for use.
*************************************************************************/
void FrameWindow::initialise(void)
{
	// create child windows
	d_titlebar		= createTitlebar();
	d_closeButton	= createCloseButton();

	// add child controls
	if (d_titlebar != NULL)
	{
		d_titlebar->setDraggingEnabled(d_dragMovable);
		addChildWindow(d_titlebar);
	}

	if (d_closeButton != NULL)
	{
		addChildWindow(d_closeButton);

		// bind handler to close button 'Click' event
		d_closeButton->subscribeEvent(PushButton::EventClicked, Event::Subscriber(&CEGUI::FrameWindow::closeClickHandler, this));
	}

	layoutComponentWidgets();
}


/*************************************************************************
	Enables or disables sizing for this window.	
*************************************************************************/
void FrameWindow::setSizingEnabled(bool setting)
{
	d_sizingEnabled = setting;
}


/*************************************************************************
	Enables or disables the frame for this window.	
*************************************************************************/
void FrameWindow::setFrameEnabled(bool setting)
{
	d_frameEnabled = setting;
	requestRedraw();
}


/*************************************************************************
	Enables or disables the title bar for the frame window.	
*************************************************************************/
void FrameWindow::setTitleBarEnabled(bool setting)
{
	if (d_titlebar != NULL)
	{
		d_titlebar->setEnabled(setting);
	}

}


/*************************************************************************
	Enables or disables the close button for the frame window.	
*************************************************************************/
void FrameWindow::setCloseButtonEnabled(bool setting)
{
	if (d_closeButton != NULL)
	{
		d_closeButton->setEnabled(setting);
	}

}


/*************************************************************************
	Enables or disables roll-up (shading) for this window.	
*************************************************************************/
void FrameWindow::setRollupEnabled(bool setting)
{
	if ((setting == false) && isRolledup())
	{
		toggleRollup();
	}

	d_rollupEnabled = setting;
}


/*************************************************************************
	Toggles the state of the window between rolled-up (shaded) and normal
	sizes.  This requires roll-up to be enabled.	
*************************************************************************/
void FrameWindow::toggleRollup(void)
{
	if (isRollupEnabled())
	{
		if (isRolledup())
		{
			d_rolledup = false;
			setSize((getMetricsMode()== Relative) ? d_rel_openSize : d_abs_openSize);
		}
		else
		{
			// store original sizes for window
			d_abs_openSize = d_abs_area.getSize();
			d_rel_openSize = d_rel_area.getSize();

			// get the current size of the title bar (if any)
			Size titleSize;
			if (d_titlebar != NULL)
			{
				titleSize = d_titlebar->getSize();
			}

			// work-around minimum size setting
			Size orgmin(d_minSize);
			d_minSize.d_width = d_minSize.d_height = 0;

			// set size of this window to 0x0, since the title/close controls are not clipped by us, they will still be visible
			setSize(Size(0.0f, 0.0f));

			// restore original min size;
			d_minSize = orgmin;

			// re-set the size of the title bar
			if (d_titlebar != NULL)
			{
				d_titlebar->setSize(titleSize);
			}

			// this must be done last so onSize does not store 0x0 as our original size
			d_rolledup = true;
			layoutComponentWidgets();
		}

		// event notification.
        WindowEventArgs args(this);
		onRollupToggled(args);
	}

}


/*************************************************************************
	Set the font to use for the title bar text	
*************************************************************************/
void FrameWindow::setTitlebarFont(const String& name)
{
	if (d_titlebar != NULL)
	{
		d_titlebar->setFont(name);
	}

}


/*************************************************************************
	Set the font to use for the title bar text	
*************************************************************************/
void FrameWindow::setTitlebarFont(Font* font)
{
	if (d_titlebar != NULL)
	{
		d_titlebar->setFont(font);
	}

}


/*************************************************************************
	Move the window by the pixel offsets specified in 'offset'.	
*************************************************************************/
void FrameWindow::offsetPixelPosition(const Vector2& offset)
{
	// update window state
	Point pos = d_abs_area.getPosition();

	pos.d_x += PixelAligned(offset.d_x);
	pos.d_y += PixelAligned(offset.d_y);

	d_abs_area.setPosition(pos);

	d_rel_area = absoluteToRelative_impl(getParent(), d_abs_area);

    WindowEventArgs args(this);
	onMoved(args);
}


/*************************************************************************	
	check local pixel co-ordinate point 'pt' and return one of the
	SizingLocation enumerated values depending where the point falls on
	the sizing border.
*************************************************************************/
FrameWindow::SizingLocation FrameWindow::getSizingBorderAtPoint(const Point& pt) const
{
	Rect	frame(getSizingRect());

	// we can only size if the frame is enabled and sizing is on
	if (isSizingEnabled() && isFrameEnabled())
	{
		// point must be inside the outer edge
		if (frame.isPointInRect(pt))
		{
			// adjust rect to get inner edge
			frame.d_left	+= d_borderSize;
			frame.d_top		+= d_borderSize;
			frame.d_right	-= d_borderSize;
			frame.d_bottom	-= d_borderSize;

			// detect which edges we are on
			bool top	= (pt.d_y < frame.d_top);
			bool bottom = (pt.d_y >= frame.d_bottom);
			bool left	= (pt.d_x < frame.d_left);
			bool right	= (pt.d_x >= frame.d_right);

			// return appropriate 'SizingLocation' value
			if (top && left)
			{
				return SizingTopLeft;
			}
			else if (top && right)
			{
				return SizingTopRight;
			}
			else if (bottom && left)
			{
				return SizingBottomLeft;
			}
			else if (bottom && right)
			{
				return SizingBottomRight;
			}
			else if (top)
			{
				return SizingTop;
			}
			else if (bottom)
			{
				return SizingBottom;
			}
			else if (left)
			{
				return SizingLeft;
			}
			else if (right)
			{
				return SizingRight;
			}

		}

	}

	// deafult: None.
	return SizingNone;
}


/*************************************************************************
	move the window's left edge by 'delta'.  The rest of the window
	does not move, thus this changes the size of the Window.	
*************************************************************************/
void FrameWindow::moveLeftEdge(float delta)
{
	delta = PixelAligned(delta);

	float width = d_abs_area.getWidth();

	// limit size to within max/min values
	if ((width - delta) < d_minSize.d_width) {
		delta = width - d_minSize.d_width;
	}
	else if ((width - delta) > d_maxSize.d_width) {
		delta = width - d_maxSize.d_width;
	}

	// update window state
	d_abs_area.d_left += delta;

	d_rel_area = absoluteToRelative_impl(getParent(), d_abs_area);

    WindowEventArgs args(this);
	onMoved(args);
	onSized(args);
}


/*************************************************************************
	move the window's right edge by 'delta'.  The rest of the window
	does not move, thus this changes the size of the Window.
*************************************************************************/
void FrameWindow::moveRightEdge(float delta)
{
	delta = PixelAligned(delta);

	float width = d_abs_area.getWidth();

	// limit size to within max/min values
	if ((width + delta) < d_minSize.d_width) {
		delta = d_minSize.d_width - width;
	}
	else if ((width + delta) > d_maxSize.d_width) {
		delta = d_maxSize.d_width - width;
	}

	// update window state
	d_abs_area.d_right += delta;
	d_dragPoint.d_x += delta;

	d_rel_area = absoluteToRelative_impl(getParent(), d_abs_area);

    WindowEventArgs args(this);
	onSized(args);
}


/*************************************************************************
	move the window's top edge by 'delta'.  The rest of the window
	does not move, thus this changes the size of the Window.
*************************************************************************/
void FrameWindow::moveTopEdge(float delta)
{
	delta = PixelAligned(delta);

	float height = d_abs_area.getHeight();

	// limit size to within max/min values
	if ((height - delta) < d_minSize.d_height) {
		delta = height - d_minSize.d_height;
	}
	else if ((height - delta) > d_maxSize.d_height) {
		delta = height - d_maxSize.d_height;
	}

	// update window state
	d_abs_area.d_top += delta;

	d_rel_area = absoluteToRelative_impl(getParent(), d_abs_area);

    WindowEventArgs args(this);
	onMoved(args);
	onSized(args);
}


/*************************************************************************
	move the window's bottom edge by 'delta'.  The rest of the window
	does not move, thus this changes the size of the Window.	
*************************************************************************/
void FrameWindow::moveBottomEdge(float delta)
{
	delta = PixelAligned(delta);

	float height = d_abs_area.getHeight();

	// limit size to within max/min values
	if ((height + delta) < d_minSize.d_height) {
		delta = d_minSize.d_height - height;
	}
	else if ((height + delta) > d_maxSize.d_height) {
		delta = d_maxSize.d_height - height;
	}

	// update window state
	d_abs_area.d_bottom += delta;
	d_dragPoint.d_y += delta;

	d_rel_area = absoluteToRelative_impl(getParent(), d_abs_area);

    WindowEventArgs args(this);
	onSized(args);
}


/*************************************************************************
	Add frame window specific events	
*************************************************************************/
void FrameWindow::addFrameWindowEvents(void)
{
	addEvent(EventRollupToggled);
	addEvent(EventCloseClicked);
}


/*************************************************************************
	Handler to map close button clicks to FrameWindow 'CloseCliked' events
*************************************************************************/
bool FrameWindow::closeClickHandler(const EventArgs& e)
{
    WindowEventArgs args(this);
	onCloseClicked(args);

	return true;
}


/*************************************************************************
	Set the appropriate mouse cursor for the given window-relative pixel
	point.
*************************************************************************/
void FrameWindow::setCursorForPoint(const Point& pt) const
{
	switch(getSizingBorderAtPoint(pt))
	{
	case SizingTop:
	case SizingBottom:
		MouseCursor::getSingleton().setImage(d_nsSizingCursor);
		break;

	case SizingLeft:
	case SizingRight:
		MouseCursor::getSingleton().setImage(d_ewSizingCursor);
		break;

	case SizingTopLeft:
	case SizingBottomRight:
		MouseCursor::getSingleton().setImage(d_nwseSizingCursor);
		break;

	case SizingTopRight:
	case SizingBottomLeft:
		MouseCursor::getSingleton().setImage(d_neswSizingCursor);
		break;

	default:
		MouseCursor::getSingleton().setImage(getMouseCursor());
		break;
	}

}


/*************************************************************************
	Event generated internally whenever the roll-up / shade state of the
	window changes.
*************************************************************************/
void FrameWindow::onRollupToggled(WindowEventArgs& e)
{
	fireEvent(EventRollupToggled, e);
}


/*************************************************************************
	Event generated internally whenever the close button is clicked.	
*************************************************************************/
void FrameWindow::onCloseClicked(WindowEventArgs& e)
{
	fireEvent(EventCloseClicked, e);
}


/*************************************************************************
	Handler for mouse move events
*************************************************************************/
void FrameWindow::onMouseMove(MouseEventArgs& e)
{
	// default processing (this is now essential as it controls event firing).
	Window::onMouseMove(e);

	// if we are not the window containing the mouse, do NOT change the cursor
	if (System::getSingleton().getWindowContainingMouse() != this)
	{
		return;
	}

	if (isSizingEnabled())
	{
		Point localMousePos(screenToWindow(e.position));

		if (getMetricsMode() == Relative)
		{
			localMousePos = relativeToAbsolute(localMousePos);
		}

		if (d_beingSized)
		{
			SizingLocation dragEdge = getSizingBorderAtPoint(d_dragPoint);

			// calculate sizing deltas...
			float	deltaX = localMousePos.d_x - d_dragPoint.d_x;
			float	deltaY = localMousePos.d_y - d_dragPoint.d_y;

			// size left or right edges
			if (isLeftSizingLocation(dragEdge))
			{
				moveLeftEdge(deltaX);
			}
			else if (isRightSizingLocation(dragEdge))
			{
				moveRightEdge(deltaX);
			}

			// size top or bottom edges
			if (isTopSizingLocation(dragEdge))
			{
				moveTopEdge(deltaY);
			}
			else if (isBottomSizingLocation(dragEdge))
			{
				moveBottomEdge(deltaY);
			}

		}
		else
		{
			setCursorForPoint(localMousePos);
		}

	}

	// mark event as handled
	e.handled = true;
}


/*************************************************************************
	Handler for mouse button down events
*************************************************************************/
void FrameWindow::onMouseButtonDown(MouseEventArgs& e)
{
	// default processing (this is now essential as it controls event firing).
	Window::onMouseButtonDown(e);

	if (e.button == LeftButton)
	{
		if (isSizingEnabled())
		{
			// get position of mouse as co-ordinates local to this window.
			Point localPos(screenToWindow(e.position));

			if (getMetricsMode() == Relative)
			{
				localPos = relativeToAbsolute(localPos);
			}

			// if the mouse is on the sizing border
			if (getSizingBorderAtPoint(localPos) != SizingNone)
			{
				// ensure all inputs come to us for now
				if (captureInput())
				{
					// setup the 'dragging' state variables
					d_beingSized = true;
					d_dragPoint = localPos;
				}

			}

		}

		e.handled = true;
	}

}


/*************************************************************************
	Handler for mouse button up events
*************************************************************************/
void FrameWindow::onMouseButtonUp(MouseEventArgs& e)
{
	// default processing (this is now essential as it controls event firing).
	Window::onMouseButtonUp(e);

	if (e.button == LeftButton)
	{
		// release our capture on the input data
		releaseInput();
		e.handled = true;
	}

}


/*************************************************************************
	Handler for when mouse capture is lost
*************************************************************************/
void FrameWindow::onCaptureLost(WindowEventArgs& e)
{
	// default processing (this is now essential as it controls event firing).
	Window::onCaptureLost(e);

	// reset sizing state
	d_beingSized = false;

	e.handled = true;
}


/*************************************************************************
	Handler for when frame window is re-sized
*************************************************************************/
void FrameWindow::onSized(WindowEventArgs& e)
{
	if (isRolledup())
	{
		// capture changed size(s)
		d_rel_openSize = d_rel_area.getSize();
		d_abs_openSize = d_abs_area.getSize();

		// re-set window size to 0x0
		Size nullsz(0,0);
		d_abs_area.setSize(nullsz);
		d_rel_area.setSize(nullsz);
	}

	layoutComponentWidgets();

	// MUST call base class handler no matter what.  This is now required 100%
	Window::onSized(e);
}


/*************************************************************************
	Handler for when a frame windows parent is sized.
*************************************************************************/
void FrameWindow::onParentSized(WindowEventArgs& e)
{
	// if we are rolled up we temporarily need to restore the original sizes so
	// that the required calculations can occur when our parent is sized.
	if (isRolledup() && (getMetricsMode() == Relative))
	{
		d_rel_area.setSize(d_rel_openSize);
		d_abs_area.setSize(d_abs_openSize);
	}

	Window::onParentSized(e);
}


/*************************************************************************
	Set whether this FrameWindow can be moved by dragging the title bar.	
*************************************************************************/
void FrameWindow::setDragMovingEnabled(bool setting)
{
	if (d_dragMovable != setting)
	{
		d_dragMovable = setting;

		if (d_titlebar != NULL)
		{
			d_titlebar->setDraggingEnabled(setting);
		}

	}

}


/*************************************************************************
	Return the font being used for the title bar text
*************************************************************************/
const Font* FrameWindow::getTitlebarFont(void) const
{
	return (d_titlebar != NULL) ? d_titlebar->getFont() : NULL;
}


/*************************************************************************
	Add properties for this class
*************************************************************************/
void FrameWindow::addFrameWindowProperties(void)
{
	addProperty(&d_sizingEnabledProperty);
	addProperty(&d_frameEnabledProperty);
	addProperty(&d_titlebarEnabledProperty);
	addProperty(&d_closeButtonEnabledProperty);
	addProperty(&d_rollUpEnabledProperty);
	addProperty(&d_rollUpStateProperty);
	addProperty(&d_dragMovingEnabledProperty);
	addProperty(&d_sizingBorderThicknessProperty);
	addProperty(&d_titlebarFontProperty);
	addProperty(&d_captionColourProperty);
}


/*************************************************************************
	Return the current colour used for rendering the caption text
*************************************************************************/
colour FrameWindow::getCaptionColour(void) const
{
	return d_titlebar->getCaptionColour();
}


/*************************************************************************
	Sets the colour to be used for rendering the caption text.
*************************************************************************/
void FrameWindow::setCaptionColour(colour col)
{
	d_titlebar->setCaptionColour(col);
}


} // End of  CEGUI namespace section
