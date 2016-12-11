/***********************************************************************
	created:	13/6/2004
	author:		Paul D Turner

	purpose:	Implements the Combobox Drop-List widget base class
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
#include "CEGUI/widgets/ComboDropList.h"
#include "CEGUI/widgets/Scrollbar.h"

// Start of CEGUI namespace section
namespace CEGUI
{
const String ComboDropList::EventNamespace("ComboDropList");
const String ComboDropList::WidgetTypeName("CEGUI/ComboDropList");


/*************************************************************************
	Constants
*************************************************************************/
// Event names
const String ComboDropList::EventListSelectionAccepted( "ListSelectionAccepted" );


/*************************************************************************
	Constructor for ComboDropList base class
*************************************************************************/
ComboDropList::ComboDropList(const String& type, const String& name) :
    ListWidget(type, name)
{
	d_autoArm = false;
	d_armed = false;
    d_lastItemSelected = nullptr;

	hide();

    // pass captured inputs to children to enable scrollbars
    setDistributesCapturedInputs(true);
    banPropertyFromXML("DistributeCapturedInputs");
}


/*************************************************************************
	Destructor for ComboDropList base class
*************************************************************************/
ComboDropList::~ComboDropList(void)
{
}


/*************************************************************************
	Initialise the Window based object ready for use.
*************************************************************************/
void ComboDropList::initialiseComponents(void)
{
    ListWidget::initialiseComponents();

	// set-up scroll bars so they return capture to us.
	getVertScrollbar()->setRestoreOldCapture(true);
	getHorzScrollbar()->setRestoreOldCapture(true);

    // ban these properties from being written
    getVertScrollbar()->banPropertyFromXMLRecursive(Window::RestoreOldCapturePropertyName);
    getHorzScrollbar()->banPropertyFromXMLRecursive(Window::RestoreOldCapturePropertyName);
}

//----------------------------------------------------------------------------//
void ComboDropList::resizeToContent(bool fit_width, bool fit_height)
{
    if (!d_windowRenderer)
        throw InvalidRequestException(
            "Function requires a valid WindowRenderer object to be set.");

    static_cast<ItemViewWindowRenderer*>(d_windowRenderer)->
        resizeViewToContent(fit_width, fit_height);
}

/*************************************************************************
	Handler for when list selection is confirmed.
*************************************************************************/
void ComboDropList::onListSelectionAccepted(WindowEventArgs& e)
{
    d_lastItemSelected = getFirstSelectedItem();
	fireEvent(EventListSelectionAccepted, e, EventNamespace);
}

/*************************************************************************
    Handler for when list content has changed
*************************************************************************/
void ComboDropList::onViewContentsChanged(WindowEventArgs& e)
{
    // basically see if our 'sticky' selection was removed
    if ((d_lastItemSelected) && !isItemInList(d_lastItemSelected))
        d_lastItemSelected = nullptr;

    // base class processing
    ListWidget::onViewContentsChanged(e);
}

/*************************************************************************
    Handler for when list selection has changed
*************************************************************************/
void ComboDropList::onSelectionChanged(ItemViewEventArgs& e)
{
    if (!isActive())
        d_lastItemSelected = getFirstSelectedItem();

    ListWidget::onSelectionChanged(e);
}

/*************************************************************************
	Handler for cursor movement events
*************************************************************************/
void ComboDropList::onCursorMove(CursorInputEventArgs& e)
{
    ListWidget::onCursorMove(e);

    // if cursor is within our area (but not our children)
	if (isHit(e.position))
	{
		if (!getChildAtPosition(e.position))
		{
			// handle auto-arm
			if (d_autoArm)
			{
				d_armed = true;
			}

			if (d_armed)
			{
                // check for an item under the cursor
                StandardItem* item = d_itemModel.getItemForIndex(indexAt(e.position));

                // if an item is under cursor, select it
                if (item != nullptr)
                {
                    setIndexSelectionState(item, true);
                }
                else
                {
                    clearSelections();
                }
			}
		}

		++e.handled;
	}
	// not within the list area
	else
	{
		// if left cursor is held, clear any selection
		if (e.state.isHeld(CursorInputSource::Left))
		{
            clearSelections();
		}
	}
}


/*************************************************************************
    Handler for cursor pressed events
*************************************************************************/
void ComboDropList::onCursorPressHold(CursorInputEventArgs& e)
{
    ListWidget::onCursorPressHold(e);

    if (e.source == CursorInputSource::Left)
	{
		if (!isHit(e.position))
		{
            clearSelections();
			releaseInput();
		}
		else
		{
			d_armed = true;
		}

		++e.handled;
	}
}


/*************************************************************************
	Handler for cursor activation events
*************************************************************************/
void ComboDropList::onCursorActivate(CursorInputEventArgs& e)
{
    ListWidget::onCursorActivate(e);

    if (e.source == CursorInputSource::Left)
	{
		if (d_armed && (getChildAtPosition(e.position) == nullptr))
		{
            // if something was selected, confirm that selection.
            if (getIndexSelectionStates().size() > 0)
            {
                WindowEventArgs args(this);
                onListSelectionAccepted(args);
            }

            releaseInput();
		}
        // if we are not already armed, in response to a left cursor activation event, we auto-arm.
		else
		{
			d_armed = true;
		}

		++e.handled;
	}

}


/*************************************************************************
	Handler for when input capture is lost
*************************************************************************/
void ComboDropList::onCaptureLost(WindowEventArgs& e)
{
    ListWidget::onCaptureLost(e);
	d_armed = false;
	hide();
	++e.handled;

    // ensure 'sticky' selection remains.
    if (d_lastItemSelected != nullptr && isItemSelected(d_lastItemSelected))
    {
        clearSelections();
        setIndexSelectionState(d_lastItemSelected, true);
    }
}

} // End of  CEGUI namespace section
