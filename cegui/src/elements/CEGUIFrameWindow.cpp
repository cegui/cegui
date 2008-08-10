/***********************************************************************
	filename: 	CEGUIFrameWindow.cpp
	created:	13/4/2004
	author:		Paul D Turner
	
	purpose:	Implementation of FrameWindow base class
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2006 Paul D Turner & The CEGUI Development Team
 *
 *   Permission is hereby granted, free of charge, to any person obtaining
 *   a copy of this software and associated documentation files (the
 *   "Software"), to deal in the Software without restriction, including
 *   without limitation the rights to use, copy, modify, merge, publish,
 *   distribute, sublicense, and/or sell copies of the Software, and to
 *   permit persons to whom the Software is furnished to do so, subject to
 *   the following conditions:
 *
 *   The above copyright notice and this permission notice shall be
 *   included in all copies or substantial portions of the Software.
 *
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 *   EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 *   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 *   IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 *   OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 *   ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 *   OTHER DEALINGS IN THE SOFTWARE.
 ***************************************************************************/
#include "elements/CEGUIFrameWindow.h"
#include "elements/CEGUITitlebar.h"
#include "elements/CEGUIPushButton.h"
#include "CEGUIMouseCursor.h"
#include "CEGUIWindowManager.h"
#include "CEGUIExceptions.h"
#include "CEGUIImagesetManager.h"
#include "CEGUIImageset.h"
#include "CEGUICoordConverter.h"

// Start of CEGUI namespace section
namespace CEGUI
{
const String FrameWindow::EventNamespace("FrameWindow");
const String FrameWindow::WidgetTypeName("CEGUI/FrameWindow");


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
FrameWindowProperties::NSSizingCursorImage      FrameWindow::d_nsSizingCursorProperty;
FrameWindowProperties::EWSizingCursorImage      FrameWindow::d_ewSizingCursorProperty;
FrameWindowProperties::NWSESizingCursorImage    FrameWindow::d_nwseSizingCursorProperty;
FrameWindowProperties::NESWSizingCursorImage    FrameWindow::d_neswSizingCursorProperty;


/*************************************************************************
	Constants
*************************************************************************/
// additional event names for this window
const String FrameWindow::EventRollupToggled( "RollupToggled" );
const String FrameWindow::EventCloseClicked( "CloseClicked" );

// other bits
const float FrameWindow::DefaultSizingBorderSize	= 8.0f;

/*************************************************************************
    Child Widget name suffix constants
*************************************************************************/
const String FrameWindow::TitlebarNameSuffix( "__auto_titlebar__" );
const String FrameWindow::CloseButtonNameSuffix( "__auto_closebutton__" );


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

	d_nsSizingCursor = d_ewSizingCursor = d_neswSizingCursor = d_nwseSizingCursor = 0;

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
void FrameWindow::initialiseComponents(void)
{
    // get component windows
    Titlebar* titlebar = getTitlebar();
    PushButton* closeButton = getCloseButton();

    // configure titlebar
    titlebar->setDraggingEnabled(d_dragMovable);
    titlebar->setText(d_text);

    // bind handler to close button 'Click' event
    closeButton->subscribeEvent(PushButton::EventClicked, Event::Subscriber(&CEGUI::FrameWindow::closeClickHandler, this));

    performChildWindowLayout();
}


/*************************************************************************
    Return whether the title bar for this window is enabled.
*************************************************************************/
bool FrameWindow::isTitleBarEnabled(void) const
{
    return !getTitlebar()->isDisabled();
}


/*************************************************************************
    Return whether this close button for this window is enabled.
*************************************************************************/
bool FrameWindow::isCloseButtonEnabled(void) const
{
    return !getCloseButton()->isDisabled();
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
    Window* titlebar = getTitlebar();
    titlebar->setEnabled(setting);
    titlebar->setVisible(setting);
}


/*************************************************************************
	Enables or disables the close button for the frame window.	
*************************************************************************/
void FrameWindow::setCloseButtonEnabled(bool setting)
{
    Window* closebtn = getCloseButton();
    closebtn->setEnabled(setting);
    closebtn->setVisible(setting);
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
        d_rolledup ^= true;
        
        // event notification.
        WindowEventArgs args(this);
        onRollupToggled(args);

        System::getSingleton().updateWindowContainingMouse();
    }

}


/*************************************************************************
	Move the window by the pixel offsets specified in 'offset'.	
*************************************************************************/
void FrameWindow::offsetPixelPosition(const Vector2& offset)
{
    UVector2 uOffset(cegui_absdim(PixelAligned(offset.d_x)),
                     cegui_absdim(PixelAligned(offset.d_y)));

    setPosition(d_area.getPosition() + uOffset);
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
    float orgWidth = d_pixelSize.d_width;
    URect area(d_area);

    // ensure that we only size to the set constraints.
    //
    // NB: We are required to do this here due to our virtually unique sizing nature; the
    // normal system for limiting the window size is unable to supply the information we
    // require for updating our internal state used to manage the dragging, etc.
    float maxWidth(d_maxSize.d_x.asAbsolute(System::getSingleton().getRenderer()->getWidth()));
    float minWidth(d_minSize.d_x.asAbsolute(System::getSingleton().getRenderer()->getWidth()));
    float newWidth = orgWidth - delta;

    if (newWidth > maxWidth)
        delta = orgWidth - maxWidth;
    else if (newWidth < minWidth)
        delta = orgWidth - minWidth;

    // ensure adjustment will be whole pixel
    float adjustment = PixelAligned(delta);

    if (d_horzAlign == HA_RIGHT)
    {
        area.d_max.d_x.d_offset -= adjustment;
    }
    else if (d_horzAlign == HA_CENTRE)
    {
        area.d_max.d_x.d_offset -= adjustment * 0.5f;
        area.d_min.d_x.d_offset += adjustment * 0.5f;
    }
    else
    {
        area.d_min.d_x.d_offset += adjustment;
    }

    setArea_impl(area.d_min, area.getSize(), d_horzAlign == HA_LEFT);
}
/*************************************************************************
	move the window's right edge by 'delta'.  The rest of the window
	does not move, thus this changes the size of the Window.
*************************************************************************/
void FrameWindow::moveRightEdge(float delta)
{
    // store this so we can work out how much size actually changed
    float orgWidth = d_pixelSize.d_width;
    URect area(d_area);

    // ensure that we only size to the set constraints.
    //
    // NB: We are required to do this here due to our virtually unique sizing nature; the
    // normal system for limiting the window size is unable to supply the information we
    // require for updating our internal state used to manage the dragging, etc.
    float maxWidth(d_maxSize.d_x.asAbsolute(System::getSingleton().getRenderer()->getWidth()));
    float minWidth(d_minSize.d_x.asAbsolute(System::getSingleton().getRenderer()->getWidth()));
    float newWidth = orgWidth + delta;

    if (newWidth > maxWidth)
        delta = maxWidth - orgWidth;
    else if (newWidth < minWidth)
        delta = minWidth - orgWidth;

    // ensure adjustment will be whole pixel
    float adjustment = PixelAligned(delta);

    area.d_max.d_x.d_offset += adjustment;

    if (d_horzAlign == HA_RIGHT)
    {
        area.d_max.d_x.d_offset += adjustment;
        area.d_min.d_x.d_offset += adjustment;
    }
    else if (d_horzAlign == HA_CENTRE)
    {
        area.d_max.d_x.d_offset += adjustment * 0.5f;
        area.d_min.d_x.d_offset += adjustment * 0.5f;
    }

    setArea_impl(area.d_min, area.getSize(), d_horzAlign == HA_RIGHT);

    // move the dragging point so mouse remains 'attached' to edge of window
    d_dragPoint.d_x += d_pixelSize.d_width - orgWidth;
}

/*************************************************************************
	move the window's top edge by 'delta'.  The rest of the window
	does not move, thus this changes the size of the Window.
*************************************************************************/
void FrameWindow::moveTopEdge(float delta)
{
    float orgHeight = d_pixelSize.d_height;
    URect area(d_area);

    // ensure that we only size to the set constraints.
    //
    // NB: We are required to do this here due to our virtually unique sizing nature; the
    // normal system for limiting the window size is unable to supply the information we
    // require for updating our internal state used to manage the dragging, etc.
    float maxHeight(d_maxSize.d_y.asAbsolute(System::getSingleton().getRenderer()->getHeight()));
    float minHeight(d_minSize.d_y.asAbsolute(System::getSingleton().getRenderer()->getHeight()));
    float newHeight = orgHeight - delta;

    if (newHeight > maxHeight)
        delta = orgHeight - maxHeight;
    else if (newHeight < minHeight)
        delta = orgHeight - minHeight;

    // ensure adjustment will be whole pixel
    float adjustment = PixelAligned(delta);

    if (d_vertAlign == VA_BOTTOM)
    {
        area.d_max.d_y.d_offset -= adjustment;
    }
    else if (d_vertAlign == VA_CENTRE)
    {
        area.d_max.d_y.d_offset -= adjustment * 0.5f;
        area.d_min.d_y.d_offset += adjustment * 0.5f;
    }
    else
    {
        area.d_min.d_y.d_offset += adjustment;
    }

    setArea_impl(area.d_min, area.getSize(), d_vertAlign == VA_TOP);
}


/*************************************************************************
	move the window's bottom edge by 'delta'.  The rest of the window
	does not move, thus this changes the size of the Window.	
*************************************************************************/
void FrameWindow::moveBottomEdge(float delta)
{
    // store this so we can work out how much size actually changed
    float orgHeight = d_pixelSize.d_height;
    URect area(d_area);

    // ensure that we only size to the set constraints.
    //
    // NB: We are required to do this here due to our virtually unique sizing nature; the
    // normal system for limiting the window size is unable to supply the information we
    // require for updating our internal state used to manage the dragging, etc.
    float maxHeight(d_maxSize.d_y.asAbsolute(System::getSingleton().getRenderer()->getHeight()));
    float minHeight(d_minSize.d_y.asAbsolute(System::getSingleton().getRenderer()->getHeight()));
    float newHeight = orgHeight + delta;

    if (newHeight > maxHeight)
        delta = maxHeight - orgHeight;
    else if (newHeight < minHeight)
        delta = minHeight - orgHeight;

    // ensure adjustment will be whole pixel
    float adjustment = PixelAligned(delta);

    area.d_max.d_y.d_offset += adjustment;

    if (d_vertAlign == VA_BOTTOM)
    {
        area.d_max.d_y.d_offset += adjustment;
        area.d_min.d_y.d_offset += adjustment;
    }
    else if (d_vertAlign == VA_CENTRE)
    {
        area.d_max.d_y.d_offset += adjustment * 0.5f;
        area.d_min.d_y.d_offset += adjustment * 0.5f;
    }

    setArea_impl(area.d_min, area.getSize(), d_vertAlign == VA_BOTTOM);

    // move the dragging point so mouse remains 'attached' to edge of window
    d_dragPoint.d_y += d_pixelSize.d_height - orgHeight;
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
    requestRedraw();
    notifyClippingChanged();

	fireEvent(EventRollupToggled, e, EventNamespace);
}


/*************************************************************************
	Event generated internally whenever the close button is clicked.	
*************************************************************************/
void FrameWindow::onCloseClicked(WindowEventArgs& e)
{
	fireEvent(EventCloseClicked, e, EventNamespace);
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
		Point localMousePos(CoordConverter::screenToWindow(*this, e.position));

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
			Point localPos(CoordConverter::screenToWindow(*this, e.position));

			// if the mouse is on the sizing border
			if (getSizingBorderAtPoint(localPos) != SizingNone)
			{
				// ensure all inputs come to us for now
				if (captureInput())
				{
					// setup the 'dragging' state variables
					d_beingSized = true;
					d_dragPoint = localPos;
					e.handled = true;
				}

			}

		}

	}

}


/*************************************************************************
	Handler for mouse button up events
*************************************************************************/
void FrameWindow::onMouseButtonUp(MouseEventArgs& e)
{
	// default processing (this is now essential as it controls event firing).
	Window::onMouseButtonUp(e);

	if (e.button == LeftButton && isCapturedByThis())
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
    Handler for when text changes
*************************************************************************/
void FrameWindow::onTextChanged(WindowEventArgs& e)
{
    Window::onTextChanged(e);
    // pass this onto titlebar component.
    getTitlebar()->setText(d_text);
    // maybe the user is using a fontdim for titlebar dimensions ;)
    performChildWindowLayout();
}


/*************************************************************************
    Handler for when this Window is activated
*************************************************************************/
void FrameWindow::onActivated(ActivationEventArgs& e)
{
	Window::onActivated(e);
	getTitlebar()->requestRedraw();
}


/*************************************************************************
    Handler for when this Window is deactivated
*************************************************************************/
void FrameWindow::onDeactivated(ActivationEventArgs& e)
{
	Window::onDeactivated(e);
	getTitlebar()->requestRedraw();
}


/*************************************************************************
	Set whether this FrameWindow can be moved by dragging the title bar.	
*************************************************************************/
void FrameWindow::setDragMovingEnabled(bool setting)
{
	if (d_dragMovable != setting)
	{
		d_dragMovable = setting;

        getTitlebar()->setDraggingEnabled(setting);
    }

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
    addProperty(&d_nsSizingCursorProperty);
    addProperty(&d_ewSizingCursorProperty);
    addProperty(&d_nwseSizingCursorProperty);
    addProperty(&d_neswSizingCursorProperty);
}


/*************************************************************************
    return the image used for the north-south sizing cursor.
*************************************************************************/
const Image* FrameWindow::getNSSizingCursorImage() const
{
    return d_nsSizingCursor;
}

/*************************************************************************
    return the image used for the east-west sizing cursor.
*************************************************************************/
const Image* FrameWindow::getEWSizingCursorImage() const
{
    return d_ewSizingCursor;
}

/*************************************************************************
    return the image used for the northwest-southeast sizing cursor.
*************************************************************************/
const Image* FrameWindow::getNWSESizingCursorImage() const
{
    return d_nwseSizingCursor;
}

/*************************************************************************
    return the image used for the northeast-southwest sizing cursor.
*************************************************************************/
const Image* FrameWindow::getNESWSizingCursorImage() const
{
    return d_neswSizingCursor;
}

/*************************************************************************
    set the image used for the north-south sizing cursor.
*************************************************************************/
void FrameWindow::setNSSizingCursorImage(const Image* image)
{
    d_nsSizingCursor = image;
}

/*************************************************************************
    set the image used for the east-west sizing cursor.
*************************************************************************/
void FrameWindow::setEWSizingCursorImage(const Image* image)
{
    d_ewSizingCursor = image;
}

/*************************************************************************
    set the image used for the northwest-southeast sizing cursor.
*************************************************************************/
void FrameWindow::setNWSESizingCursorImage(const Image* image)
{
    d_nwseSizingCursor = image;
}

/*************************************************************************
    set the image used for the northeast-southwest sizing cursor.
*************************************************************************/
void FrameWindow::setNESWSizingCursorImage(const Image* image)
{
    d_neswSizingCursor = image;
}

/*************************************************************************
    set the image used for the north-south sizing cursor.
*************************************************************************/
void FrameWindow::setNSSizingCursorImage(const String& imageset, const String& image)
{
    d_nsSizingCursor = &ImagesetManager::getSingleton().getImageset(imageset)->getImage(image);
}

/*************************************************************************
    set the image used for the east-west sizing cursor.
*************************************************************************/
void FrameWindow::setEWSizingCursorImage(const String& imageset, const String& image)
{
    d_ewSizingCursor = &ImagesetManager::getSingleton().getImageset(imageset)->getImage(image);
}

/*************************************************************************
    set the image used for the northwest-southeast sizing cursor.
*************************************************************************/
void FrameWindow::setNWSESizingCursorImage(const String& imageset, const String& image)
{
    d_nwseSizingCursor = &ImagesetManager::getSingleton().getImageset(imageset)->getImage(image);
}

/*************************************************************************
    set the image used for the northeast-southwest sizing cursor.
*************************************************************************/
void FrameWindow::setNESWSizingCursorImage(const String& imageset, const String& image)
{
    d_neswSizingCursor = &ImagesetManager::getSingleton().getImageset(imageset)->getImage(image);
}

/*************************************************************************
    Return a pointer to the Titlebar component widget for this FrameWindow.
*************************************************************************/
Titlebar* FrameWindow::getTitlebar() const
{
    return static_cast<Titlebar*>(WindowManager::getSingleton().getWindow(
                                  getName() + TitlebarNameSuffix));
}

/*************************************************************************
    Return a pointer to the close button component widget for this
    FrameWindow.
*************************************************************************/
PushButton* FrameWindow::getCloseButton() const
{
    return static_cast<PushButton*>(WindowManager::getSingleton().getWindow(
                                    getName() + CloseButtonNameSuffix));
}

} // End of  CEGUI namespace section
