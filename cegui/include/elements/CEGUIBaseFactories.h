/***********************************************************************
    filename:   CEGUIBaseFactories.h
    created:    Sun Feb 5 2006
    author:     Tomas Lindquist Olsen <tomas@famolsen.dk>

    purpose:    Declarations of all the base window factories
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
#ifndef _CEGUIBaseFactories_h_
#define _CEGUIBaseFactories_h_

#include "CEGUIWindowFactory.h"
#include "elements/CEGUIGUISheet.h"
#include "elements/CEGUIDragContainer.h"
#include "elements/CEGUIScrolledContainer.h"
#include "elements/CEGUIClippedContainer.h"
#include "elements/CEGUICheckbox.h"
#include "elements/CEGUIPushButton.h"
#include "elements/CEGUIRadioButton.h"
#include "elements/CEGUITabButton.h"
#include "elements/CEGUICombobox.h"
#include "elements/CEGUIComboDropList.h"
#include "elements/CEGUIEditbox.h"
#include "elements/CEGUIFrameWindow.h"
#include "elements/CEGUIItemEntry.h"
#include "elements/CEGUIListbox.h"
#include "elements/CEGUIListHeader.h"
#include "elements/CEGUIMenubar.h"
#include "elements/CEGUIPopupMenu.h"
#include "elements/CEGUIMenuItem.h"
#include "elements/CEGUIMultiColumnList.h"
#include "elements/CEGUIMultiLineEditbox.h"
#include "elements/CEGUIProgressBar.h"
#include "elements/CEGUIScrollablePane.h"
#include "elements/CEGUIScrollbar.h"
#include "elements/CEGUISlider.h"
#include "elements/CEGUISpinner.h"
#include "elements/CEGUITabButton.h"
#include "elements/CEGUITabControl.h"
#include "elements/CEGUIThumb.h"
#include "elements/CEGUITitlebar.h"
#include "elements/CEGUITooltip.h"
#include "elements/CEGUIItemListbox.h"
#include "elements/CEGUIGroupBox.h"
#include "elements/CEGUITree.h"

// Start of CEGUI namespace section
namespace CEGUI
{
    CEGUI_DECLARE_WINDOW_FACTORY(GUISheet)
    CEGUI_DECLARE_WINDOW_FACTORY(DragContainer)
    CEGUI_DECLARE_WINDOW_FACTORY(ScrolledContainer)
    CEGUI_DECLARE_WINDOW_FACTORY(ClippedContainer)
    CEGUI_DECLARE_WINDOW_FACTORY(Checkbox)
    CEGUI_DECLARE_WINDOW_FACTORY(ComboDropList)
    CEGUI_DECLARE_WINDOW_FACTORY(Combobox)
    CEGUI_DECLARE_WINDOW_FACTORY(Editbox)
    CEGUI_DECLARE_WINDOW_FACTORY(FrameWindow)
    CEGUI_DECLARE_WINDOW_FACTORY(ItemEntry)
    CEGUI_DECLARE_WINDOW_FACTORY(ListHeader)
    CEGUI_DECLARE_WINDOW_FACTORY(ListHeaderSegment)
    CEGUI_DECLARE_WINDOW_FACTORY(Listbox)
    CEGUI_DECLARE_WINDOW_FACTORY(MenuItem)
    CEGUI_DECLARE_WINDOW_FACTORY(Menubar)
    CEGUI_DECLARE_WINDOW_FACTORY(MultiColumnList)
    CEGUI_DECLARE_WINDOW_FACTORY(MultiLineEditbox)
    CEGUI_DECLARE_WINDOW_FACTORY(PopupMenu)
    CEGUI_DECLARE_WINDOW_FACTORY(ProgressBar)
    CEGUI_DECLARE_WINDOW_FACTORY(PushButton)
    CEGUI_DECLARE_WINDOW_FACTORY(RadioButton)
    CEGUI_DECLARE_WINDOW_FACTORY(ScrollablePane)
    CEGUI_DECLARE_WINDOW_FACTORY(Scrollbar)
    CEGUI_DECLARE_WINDOW_FACTORY(Slider)
    CEGUI_DECLARE_WINDOW_FACTORY(Spinner)
    CEGUI_DECLARE_WINDOW_FACTORY(TabButton)
    CEGUI_DECLARE_WINDOW_FACTORY(TabControl)
    CEGUI_DECLARE_WINDOW_FACTORY(Thumb)
    CEGUI_DECLARE_WINDOW_FACTORY(Titlebar)
    CEGUI_DECLARE_WINDOW_FACTORY(Tooltip)
    CEGUI_DECLARE_WINDOW_FACTORY(ItemListbox)
	CEGUI_DECLARE_WINDOW_FACTORY(GroupBox)
	CEGUI_DECLARE_WINDOW_FACTORY(Tree)

} // End of  CEGUI namespace section

#endif // End of guard _CEGUIBaseFactories_h_
