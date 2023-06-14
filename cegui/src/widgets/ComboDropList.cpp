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

namespace CEGUI
{
const String ComboDropList::EventNamespace("ComboDropList");
const String ComboDropList::WidgetTypeName("CEGUI/ComboDropList");
const String ComboDropList::EventListSelectionAccepted("ListSelectionAccepted");

//----------------------------------------------------------------------------//
ComboDropList::ComboDropList(const String& type, const String& name)
    : ListWidget(type, name)
{
    // pass captured inputs to children to enable scrollbars
    setDistributesCapturedInputs(true);
    banPropertyFromXML("DistributeCapturedInputs");
}

//----------------------------------------------------------------------------//
void ComboDropList::initialiseComponents()
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
        throw InvalidRequestException("Function requires a valid WindowRenderer object to be set.");

    static_cast<ItemViewWindowRenderer*>(d_windowRenderer)->resizeViewToContent(fit_width, fit_height);
}

//----------------------------------------------------------------------------//
void ComboDropList::onListSelectionAccepted(WindowEventArgs& e)
{
	fireEvent(EventListSelectionAccepted, e, EventNamespace);
}

//----------------------------------------------------------------------------//
void ComboDropList::onViewContentsChanged(WindowEventArgs& e)
{
    // basically see if our 'sticky' selection was removed
    if (d_lastItemSelected && !isItemInList(d_lastItemSelected))
        d_lastItemSelected = nullptr;

    ListWidget::onViewContentsChanged(e);
}

//----------------------------------------------------------------------------//
void ComboDropList::onSelectionChanged(ItemViewEventArgs& e)
{
    if (!isActive())
        d_lastItemSelected = getFirstSelectedItem();

    ListWidget::onSelectionChanged(e);
}

//----------------------------------------------------------------------------//
void ComboDropList::onCursorMove(CursorMoveEventArgs& e)
{
    ListWidget::onCursorMove(e);

    // check if cursor is within our area (but not our children)
	if (isHit(e.d_surfacePos))
	{
		if (!getChildAtPosition(e.d_surfacePos))
		{
			if (d_autoArm)
				d_armed = true;

			if (d_armed)
			{
                if (auto item = d_itemModel.getItemForIndex(indexAtLocal(e.d_localPos)))
                    setIndexSelectionState(item, true);
                else
                    clearSelections();
			}
		}

		++e.handled;
	}
	else
	{
		// if left cursor is held, clear any selection
		if (e.d_buttons.has(MouseButton::Left))
            clearSelections();
	}
}


//----------------------------------------------------------------------------//
void ComboDropList::onMouseButtonDown(MouseButtonEventArgs& e)
{
    ListWidget::onMouseButtonDown(e);

    if (e.d_button == MouseButton::Left)
	{
		if (!isHit(e.d_surfacePos))
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

//----------------------------------------------------------------------------//
void ComboDropList::onClick(MouseButtonEventArgs& e)
{
    ListWidget::onClick(e);

    if (e.d_button == MouseButton::Left)
	{
		if (d_armed && !getChildAtPosition(e.d_surfacePos))
		{
            // if something was selected, confirm that selection.
            if (getIndexSelectionStates().size() > 0)
            {
                d_lastItemSelected = getFirstSelectedItem();

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

//----------------------------------------------------------------------------//
void ComboDropList::onCaptureLost(WindowEventArgs& e)
{
    ListWidget::onCaptureLost(e);

	d_armed = false;
	hide();
	++e.handled;

    // ensure 'sticky' selection remains.
    if (d_lastItemSelected && isItemSelected(d_lastItemSelected))
    {
        clearSelections();
        setIndexSelectionState(d_lastItemSelected, true);
    }
}

}
