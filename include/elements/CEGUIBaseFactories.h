/************************************************************************
    filename:   CEGUIBaseFactories.h
    created:    Sun Feb 5 2006
    author:     Tomas Lindquist Olsen <tomas@famolsen.dk>

    purpose:    Declarations of all the base window factories
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
#ifndef _CEGUIBaseFactories_h_
#define _CEGUIBaseFactories_h_

#include "CEGUIWindowFactory.h"
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

// Start of CEGUI namespace section
namespace CEGUI
{

    CEGUI_DECLARE_WINDOW_FACTORY(Checkbox);
    CEGUI_DECLARE_WINDOW_FACTORY(ComboDropList);
    CEGUI_DECLARE_WINDOW_FACTORY(Combobox);
    CEGUI_DECLARE_WINDOW_FACTORY(Editbox);
    CEGUI_DECLARE_WINDOW_FACTORY(FrameWindow);
    CEGUI_DECLARE_WINDOW_FACTORY(ItemEntry);
    CEGUI_DECLARE_WINDOW_FACTORY(ListHeader);
    CEGUI_DECLARE_WINDOW_FACTORY(ListHeaderSegment);
    CEGUI_DECLARE_WINDOW_FACTORY(Listbox);
    CEGUI_DECLARE_WINDOW_FACTORY(MenuItem);
    CEGUI_DECLARE_WINDOW_FACTORY(Menubar);
    CEGUI_DECLARE_WINDOW_FACTORY(MultiColumnList);
    CEGUI_DECLARE_WINDOW_FACTORY(MultiLineEditbox);
    CEGUI_DECLARE_WINDOW_FACTORY(PopupMenu);
    CEGUI_DECLARE_WINDOW_FACTORY(ProgressBar);
    CEGUI_DECLARE_WINDOW_FACTORY(PushButton);
    CEGUI_DECLARE_WINDOW_FACTORY(RadioButton);
    CEGUI_DECLARE_WINDOW_FACTORY(ScrollablePane);
    CEGUI_DECLARE_WINDOW_FACTORY(Scrollbar);
    CEGUI_DECLARE_WINDOW_FACTORY(Slider);
    CEGUI_DECLARE_WINDOW_FACTORY(Spinner);
    CEGUI_DECLARE_WINDOW_FACTORY(TabButton);
    CEGUI_DECLARE_WINDOW_FACTORY(TabControl);
    CEGUI_DECLARE_WINDOW_FACTORY(Thumb);
    CEGUI_DECLARE_WINDOW_FACTORY(Titlebar);
    CEGUI_DECLARE_WINDOW_FACTORY(Tooltip);

} // End of  CEGUI namespace section

#endif // End of guard _CEGUIBaseFactories_h_
