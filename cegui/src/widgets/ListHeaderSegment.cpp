/***********************************************************************
	created:	15/6/2004
	author:		Paul D Turner

	purpose:	Implementation of List header segment widget.
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
#include "CEGUI/widgets/ListHeaderSegment.h"
#include "CEGUI/Cursor.h"
#include "CEGUI/CoordConverter.h"
#include "CEGUI/ImageManager.h"
#include <CEGUI/WindowRendererSets/Core/ItemViewRenderer.h>

// Start of CEGUI namespace section
namespace CEGUI
{
const String ListHeaderSegment::EventNamespace("ListHeaderSegment");
const String ListHeaderSegment::WidgetTypeName("CEGUI/ListHeaderSegment");

/*************************************************************************
	Constants
*************************************************************************/
// Event names
const String ListHeaderSegment::EventSegmentClicked( "SegmentClicked" );
const String ListHeaderSegment::EventSplitterDoubleClicked( "SplitterDoubleClicked" );
const String ListHeaderSegment::EventSizingSettingChanged( "SizingSettingChanged" );
const String ListHeaderSegment::EventSortDirectionChanged( "SortDirectionChanged" );
const String ListHeaderSegment::EventMovableSettingChanged( "MovableSettingChanged" );
const String ListHeaderSegment::EventSegmentDragStart( "SegmentDragStart" );
const String ListHeaderSegment::EventSegmentDragStop( "SegmentDragStop" );
const String ListHeaderSegment::EventSegmentDragPositionChanged( "SegmentDragPositionChanged" );
const String ListHeaderSegment::EventSegmentSized( "SegmentSized" );
const String ListHeaderSegment::EventClickableSettingChanged( "ClickableSettingChanged" );

// Defaults
const float	ListHeaderSegment::DefaultSizingArea	= 8.0f;
const float	ListHeaderSegment::SegmentMoveThreshold	= 12.0f;


/*************************************************************************
	Constructor for list header segment base class
*************************************************************************/
ListHeaderSegment::ListHeaderSegment(const String& type, const String& name) :
	Window(type, name),
	d_sizingCursor(nullptr),
	d_movingCursor(nullptr),
	d_splitterSize(DefaultSizingArea),
	d_splitterHover(false),
	d_dragSizing(false),
	d_sortDir(SortDirection::NoSorting),
	d_segmentHover(false),
	d_segmentPushed(false),
	d_sizingEnabled(true),
	d_movingEnabled(true),
	d_dragMoving(false),
	d_allowClicks(true)
{
	addHeaderSegmentProperties();
}


/*************************************************************************
	Destructor for list header segment base class.
*************************************************************************/
ListHeaderSegment::~ListHeaderSegment(void)
{
}


/*************************************************************************
	Set whether this segment can be sized.
*************************************************************************/
void ListHeaderSegment::setSizingEnabled(bool setting)
{
	if (d_sizingEnabled != setting)
	{
		d_sizingEnabled = setting;

		// if sizing is now disabled, ensure sizing operation is cancelled
		if (!d_sizingEnabled && d_dragSizing)
		{
			releaseInput();
		}

		WindowEventArgs args(this);
		onSizingSettingChanged(args);
	}

}


/*************************************************************************
	Set the current sort direction set for this segment.
*************************************************************************/
void ListHeaderSegment::setSortDirection(SortDirection sort_dir)
{
	if (d_sortDir != sort_dir)
	{
		d_sortDir = sort_dir;

		WindowEventArgs args(this);
		onSortDirectionChanged(args);

		invalidate();
	}

}


/*************************************************************************
	Set whether drag moving is allowed for this segment.
*************************************************************************/
void ListHeaderSegment::setDragMovingEnabled(bool setting)
{
	if (d_movingEnabled != setting)
	{
		d_movingEnabled = setting;

		WindowEventArgs args(this);
		onMovableSettingChanged(args);
	}

}


/*************************************************************************
	Set whether the segment is clickable.
*************************************************************************/
void ListHeaderSegment::setClickable(bool setting)
{
	if (d_allowClicks != setting)
	{
		d_allowClicks = setting;

		WindowEventArgs args(this);
		onClickableSettingChanged(args);
	}

}


/*************************************************************************
	Handler called when segment is clicked.
*************************************************************************/
void ListHeaderSegment::onSegmentClicked(WindowEventArgs& e)
{
	fireEvent(EventSegmentClicked, e, EventNamespace);
}


/*************************************************************************
	Handler called when the sizer/splitter is double-clicked.
*************************************************************************/
void ListHeaderSegment::onSplitterDoubleClicked(WindowEventArgs& e)
{
	fireEvent(EventSplitterDoubleClicked, e, EventNamespace);
}


/*************************************************************************
	Handler called when sizing setting changes.
*************************************************************************/
void ListHeaderSegment::onSizingSettingChanged(WindowEventArgs& e)
{
	fireEvent(EventSizingSettingChanged, e, EventNamespace);
}


/*************************************************************************
	Handler called when the sort direction value changes.
*************************************************************************/
void ListHeaderSegment::onSortDirectionChanged(WindowEventArgs& e)
{
	invalidate();
	fireEvent(EventSortDirectionChanged, e, EventNamespace);
}


/*************************************************************************
	Handler called when the drag-movable setting is changed.
*************************************************************************/
void ListHeaderSegment::onMovableSettingChanged(WindowEventArgs& e)
{
	fireEvent(EventMovableSettingChanged, e, EventNamespace);
}


/*************************************************************************
	Handler called when the user starts dragging the segment.
*************************************************************************/
void ListHeaderSegment::onSegmentDragStart(WindowEventArgs& e)
{
	fireEvent(EventSegmentDragStart, e, EventNamespace);
}


/*************************************************************************
    Handler called when the user stops dragging the segment
    (releases pointer)
*************************************************************************/
void ListHeaderSegment::onSegmentDragStop(WindowEventArgs& e)
{
	fireEvent(EventSegmentDragStop, e, EventNamespace);
}


/*************************************************************************
	Handler called when the drag position changes.
*************************************************************************/
void ListHeaderSegment::onSegmentDragPositionChanged(WindowEventArgs& e)
{
	invalidate();
	fireEvent(EventSegmentDragPositionChanged, e, EventNamespace);
}


/*************************************************************************
	Handler called when the segment is sized.
*************************************************************************/
void ListHeaderSegment::onSegmentSized(WindowEventArgs& e)
{
	invalidate();
	fireEvent(EventSegmentSized, e, EventNamespace);
}


/*************************************************************************
	Handler called when the clickable setting for the segment changes
*************************************************************************/
void ListHeaderSegment::onClickableSettingChanged(WindowEventArgs& e)
{
	fireEvent(EventClickableSettingChanged, e, EventNamespace);
}


/*************************************************************************
	Processing for drag-sizing the segment
*************************************************************************/
void ListHeaderSegment::doDragSizing(const glm::vec2& local_cursor)
{
    float delta = local_cursor.x - d_dragPoint.x;

    // store this so we can work out how much size actually changed
    const float orgWidth = d_pixelSize.d_width;

    // ensure that we only size to the set constraints.
    //
    // NB: We are required to do this here due to our virtually unique sizing nature; the
    // normal system for limiting the window size is unable to supply the information we
    // require for updating our internal state used to manage the dragging, etc.
    const float maxWidth(CoordConverter::asAbsolute(d_maxSize.d_width, getRootContainerSize().d_width));
    const float minWidth(CoordConverter::asAbsolute(d_minSize.d_width, getRootContainerSize().d_width));
    const float newWidth = orgWidth + delta;

    if (maxWidth != 0.0f && newWidth > maxWidth)
        delta = maxWidth - orgWidth;
    else if (newWidth < minWidth)
        delta = minWidth - orgWidth;

    // update segment area rect
    // URGENT FIXME: The pixel alignment will be done automatically again, right? Why is it done here? setArea_impl will do it!
    URect area(d_area.d_min.d_x, d_area.d_min.d_y, d_area.d_max.d_x + UDim(0,/*PixelAligned(*/delta/*)*/), d_area.d_max.d_y);
    setArea_impl(area.d_min, area.getSize());

    // move the dragging point so cursor remains 'attached' to edge of segment
    d_dragPoint.x += d_pixelSize.d_width - orgWidth;

    WindowEventArgs args(this);
    onSegmentSized(args);
}


/*************************************************************************
    Processing for drag-moving the segment
*************************************************************************/
void ListHeaderSegment::doDragMoving(const glm::vec2& local_cursor)
{
    // calculate movement deltas.
    const float deltaX = local_cursor.x - d_dragPoint.x;
    const float deltaY = local_cursor.y - d_dragPoint.y;

	// update 'ghost' position
    d_dragPosition.x += deltaX;
    d_dragPosition.y += deltaY;

	// update drag point.
    d_dragPoint.x += deltaX;
    d_dragPoint.y += deltaY;

	WindowEventArgs args(this);
	onSegmentDragPositionChanged(args);
}


/*************************************************************************
	Initialise and enter the drag moving state.
*************************************************************************/
void ListHeaderSegment::initDragMoving(void)
{
	if (d_movingEnabled)
	{
		// initialise drag moving state
		d_dragMoving = true;
		d_segmentPushed = false;
		d_segmentHover = false;
        d_dragPosition = glm::vec2(0, 0);

        // setup new indicator
        getGUIContext().getCursor().setImage(d_movingCursor);

		// Trigger the event
		WindowEventArgs args(this);
		onSegmentDragStart(args);
	}

}


/*************************************************************************
	Initialise the state for hovering over sizing area.
*************************************************************************/
void ListHeaderSegment::initSizingHoverState(void)
{
	// only react if settings are changing.
	if (!d_splitterHover  && !d_segmentPushed)
	{
		d_splitterHover = true;

        // change the cursor.
        getGUIContext().getCursor().setImage(d_sizingCursor);

		// trigger redraw so 'sizing' area can be highlighted if needed.
		invalidate();
	}

	// reset segment hover as needed.
	if (d_segmentHover)
	{
		d_segmentHover = false;
		invalidate();
	}

}


/*************************************************************************
	Initialise the state for hovering over main segment area
*************************************************************************/
void ListHeaderSegment::initSegmentHoverState(void)
{
	// reset sizing area hover state if needed.
	if (d_splitterHover)
	{
		d_splitterHover = false;
        getGUIContext().getCursor().setImage(getCursor());
		invalidate();
	}

	// set segment hover state if not already set.
	if ((!d_segmentHover) && isClickable())
	{
		d_segmentHover = true;
		invalidate();
	}
}


/*************************************************************************
	Return true if move threshold for initiating drag-moving has been
	exceeded.
*************************************************************************/
bool ListHeaderSegment::isDragMoveThresholdExceeded(const glm::vec2& local_cursor)
{
    // see if cursor has moved far enough to start move operation
    // calculate movement deltas.
    const float deltaX = local_cursor.x - d_dragPoint.x;
    const float deltaY = local_cursor.y - d_dragPoint.y;

	if ((deltaX > SegmentMoveThreshold) || (deltaX < -SegmentMoveThreshold) ||
		(deltaY > SegmentMoveThreshold) || (deltaY < -SegmentMoveThreshold))
	{
		return true;
	}
	else
	{
		return false;
	}
}


/*************************************************************************
    Handler for when cursor position changes in widget area (or captured)
*************************************************************************/
void ListHeaderSegment::onCursorMove(CursorInputEventArgs& e)
{
	// base class processing
	Window::onCursorMove(e);

    // convert cursor position to something local
    const glm::vec2 local_cursor_pos(CoordConverter::screenToWindow(*this, e.position));

	// handle drag sizing
	if (d_dragSizing)
	{
        doDragSizing(local_cursor_pos);
	}
	// handle drag moving
	else if (d_dragMoving)
	{
        doDragMoving(local_cursor_pos);
    }
    // not sizing, is cursor in the widget area?
    else if (isHit(e.position))
    {
        // cursor in sizing area & sizing is enabled
        if ((local_cursor_pos.x > (d_pixelSize.d_width - d_splitterSize)) && d_sizingEnabled)
		{
			initSizingHoverState();
		}
        // cursor not in sizing area and/or sizing not enabled
		else
		{
			initSegmentHoverState();

			// if we are pushed but not yet drag moving
			if (d_segmentPushed && !d_dragMoving)
			{
                if (isDragMoveThresholdExceeded(local_cursor_pos))
                {
                    initDragMoving();
                }
			}
		}
	}
    // cursor is no longer within the widget area...
	else
	{
		// only change settings if change is required
		if (d_splitterHover)
		{
			d_splitterHover = false;
            getGUIContext().getCursor().setImage(getCursor());
			invalidate();
		}

		// reset segment hover state if not already done.
		if (d_segmentHover)
		{
			d_segmentHover = false;
			invalidate();
		}
	}

	++e.handled;
}


/*************************************************************************
    Handler for when cursor is pressed
*************************************************************************/
void ListHeaderSegment::onCursorPressHold(CursorInputEventArgs& e)
{
	// base class processing
    Window::onCursorPressHold(e);

    if (e.source == CursorInputSource::Left)
	{
		// ensure all inputs come to us for now
		if (captureInput())
		{
            // get position of cursor as co-ordinates local to this window.
            const glm::vec2 localPos(CoordConverter::screenToWindow(*this, e.position));

			// store drag point for possible sizing or moving operation.
			d_dragPoint = localPos;

            // if the cursor is in the sizing area
			if (d_splitterHover)
			{
				if (isSizingEnabled())
				{
					// setup the 'dragging' state variables
					d_dragSizing = true;
				}
			}
			else
			{
				d_segmentPushed = true;
			}
		}
		++e.handled;
	}
}


/*************************************************************************
    Handler for when the cursor is activated
*************************************************************************/
void ListHeaderSegment::onCursorActivate(CursorInputEventArgs& e)
{
	// base class processing
    Window::onCursorActivate(e);

    if (e.source == CursorInputSource::Left)
	{
		// if we were pushed and cursor was released (activated) within our segment area
		if (d_segmentPushed && d_segmentHover)
		{
			WindowEventArgs args(this);
			onSegmentClicked(args);
		}
		else if (d_dragMoving)
		{
            getGUIContext().getCursor().setImage(getCursor());

			WindowEventArgs args(this);
			onSegmentDragStop(args);
		}

		// release our capture on the input data
		releaseInput();
		++e.handled;
	}
}

/*************************************************************************
    Handler for when cursor leaves the widget area (uncaptured)
*************************************************************************/
void ListHeaderSegment::onCursorLeaves(CursorInputEventArgs& e)
{
	// base class processing
    Window::onCursorLeaves(e);

	d_splitterHover = false;
	d_dragSizing = false;
	d_segmentHover = false;
	invalidate();
}

void ListHeaderSegment::onSemanticInputEvent(SemanticEventArgs& e)
{
    // base class processing
    Window::onSemanticInputEvent(e);

    if (isDisabled())
        return;

    if (e.d_semanticValue == SemanticValue::SelectWord && e.d_payload.source == CursorInputSource::Left &&
        d_splitterHover)
    {
        WindowEventArgs args(this);
        onSplitterDoubleClicked(args);

        ++e.handled;
    }
}

/*************************************************************************
    Handler for when cursor input capture is lost
*************************************************************************/
void ListHeaderSegment::onCaptureLost(WindowEventArgs& e)
{
	// base class processing
	Window::onCaptureLost(e);

	// reset segment state
	d_dragSizing = false;
	d_segmentPushed = false;
	d_dragMoving = false;

	++e.handled;
}

/*************************************************************************
	adds properties for the class
*************************************************************************/
void ListHeaderSegment::addHeaderSegmentProperties(void)
{
    const String& propertyOrigin = WidgetTypeName;

    CEGUI_DEFINE_PROPERTY(ListHeaderSegment, bool,
        "Sizable", "Property to get/set the sizable setting of the header segment.  Value is either \"true\" or \"false\".",
        &ListHeaderSegment::setSizingEnabled, &ListHeaderSegment::isSizingEnabled, true /* TODO: Inconsistency */
    );

    CEGUI_DEFINE_PROPERTY(ListHeaderSegment, bool,
        "Clickable", "Property to get/set the click-able setting of the header segment.  Value is either \"true\" or \"false\".",
        &ListHeaderSegment::setClickable, &ListHeaderSegment::isClickable, true
    );

    CEGUI_DEFINE_PROPERTY(ListHeaderSegment, bool,
        "Dragable", "Property to get/set the drag-able setting of the header segment.  Value is either \"true\" or \"false\".",
        &ListHeaderSegment::setDragMovingEnabled, &ListHeaderSegment::isDragMovingEnabled, true /* TODO: Inconsistency */
    );

    CEGUI_DEFINE_PROPERTY(ListHeaderSegment, ListHeaderSegment::SortDirection,
        "SortDirection", "Property to get/set the sort direction setting of the header segment.  Value is the text of one of the SortDirection enumerated value names.",
        &ListHeaderSegment::setSortDirection, &ListHeaderSegment::getSortDirection, ListHeaderSegment::SortDirection::NoSorting
    );

    CEGUI_DEFINE_PROPERTY(ListHeaderSegment, Image*,
        "SizingCursorImage", "Property to get/set the sizing cursor image for the List Header Segment.  Value should be \"set:[imageset name] image:[image name]\".",
        &ListHeaderSegment::setSizingCursorImage, &ListHeaderSegment::getSizingCursorImage, nullptr
    );

    CEGUI_DEFINE_PROPERTY(ListHeaderSegment, Image*,
        "MovingCursorImage", "Property to get/set the moving cursor image for the List Header Segment.  Value should be \"set:[imageset name] image:[image name]\".",
        &ListHeaderSegment::setMovingCursorImage, &ListHeaderSegment::getMovingCursorImage, nullptr
    );
}

const Image* ListHeaderSegment::getSizingCursorImage() const
{
    return d_sizingCursor;
}

void ListHeaderSegment::setSizingCursorImage(const Image* image)
{
    d_sizingCursor = image;
}

void ListHeaderSegment::setSizingCursorImage(const String& name)
{
    d_sizingCursor = &ImageManager::getSingleton().get(name);
}

const Image* ListHeaderSegment::getMovingCursorImage() const
{
    return d_movingCursor;
}

void ListHeaderSegment::setMovingCursorImage(const Image* image)
{
    d_movingCursor = image;
}

void ListHeaderSegment::setMovingCursorImage(const String& name)
{
    d_movingCursor = &ImageManager::getSingleton().get(name);
}

} // End of  CEGUI namespace section
