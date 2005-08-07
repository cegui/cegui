/************************************************************************
	filename: required.h
	created:  16/3/2005
	author:   Tomas Lindquist Olsen
	
	purpose:  Header that includes the necessary stuff needed for the Lua bindings
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
#include "CEGUI.h"

typedef CEGUI::String string;

namespace CEGUI
{

/*************************************************************************
	Helper functions for easy casting of the EventArgs class
*************************************************************************/
const MouseCursorEventArgs&		ceguiLua_toMouseCursorEventArgs(const EventArgs& e);
const WindowEventArgs&			ceguiLua_toWindowEventArgs(const EventArgs& e);
const ActivationEventArgs&		ceguiLua_toActivationEventArgs(const EventArgs& e);
const HeaderSequenceEventArgs&	ceguiLua_toHeaderSequenceEventArgs(const EventArgs& e);
const MouseEventArgs&			ceguiLua_toMouseEventArgs(const EventArgs& e);
const KeyEventArgs&				ceguiLua_toKeyEventArgs(const EventArgs& e);


/*************************************************************************
	Helper functions for easy casting of the classes derived from
	CEGUI::Window (the widgets)
*************************************************************************/
ButtonBase*			ceguiLua_toButtonBase(Window* w);
Checkbox*			ceguiLua_toCheckbox(Window* w);
Combobox*			ceguiLua_toCombobox(Window* w);
ComboDropList*		ceguiLua_toComboDropList(Window* w);
DragContainer*		ceguiLua_toDragContainer(Window* w);
Editbox*			ceguiLua_toEditbox(Window* w);
FrameWindow*		ceguiLua_toFrameWindow(Window* w);
GUISheet*			ceguiLua_toGUISheet(Window* w);
ItemEntry*			ceguiLua_toItemEntry(Window* w);
ItemListBase*		ceguiLua_toItemListBase(Window* w);
Listbox*			ceguiLua_toListbox(Window* w);
ListHeader*			ceguiLua_toListHeader(Window* w);
ListHeaderSegment*	ceguiLua_toListHeaderSegment(Window* w);
Menubar*			ceguiLua_toMenubar(Window* w);
MenuBase*			ceguiLua_toMenuBase(Window* w);
MenuItem*			ceguiLua_toMenuItem(Window* w);
MultiColumnList*	ceguiLua_toMultiColumnList(Window* w);
MultiLineEditbox*	ceguiLua_toMultiLineEditbox(Window* w);
PopupMenu*			ceguiLua_toPopupMenu(Window* w);
ProgressBar*		ceguiLua_toProgressBar(Window* w);
PushButton*			ceguiLua_toPushButton(Window* w);
RadioButton*		ceguiLua_toRadioButton(Window* w);
ScrollablePane*		ceguiLua_toScrollablePane(Window* w);
Scrollbar*			ceguiLua_toScrollbar(Window* w);
ScrolledContainer*	ceguiLua_toScrolledContainer(Window* w);
Slider*				ceguiLua_toSlider(Window* w);
Spinner*			ceguiLua_toSpinner(Window* w);
Static*				ceguiLua_toStatic(Window* w);
StaticImage*		ceguiLua_toStaticImage(Window* w);
StaticText*			ceguiLua_toStaticText(Window* w);
TabButton*			ceguiLua_toTabButton(Window* w);
TabControl*			ceguiLua_toTabControl(Window* w);
TabPane*			ceguiLua_toTabPane(Window* w);
TextItem*			ceguiLua_toTextItem(Window* w);
Thumb*				ceguiLua_toThumb(Window* w);
Tooltip*			ceguiLua_toTooltip(Window* w);


/*************************************************************************
	Functions for getting Thumb range pairs as two return values
*************************************************************************/
void ceguiLua_Thumb_getHorzRange(Thumb* wnd, float* min, float* max);
void ceguiLua_Thumb_getVertRange(Thumb* wnd, float* min, float* max);


/*************************************************************************
	Functions for creating list box items
*************************************************************************/
ListboxTextItem* ceguiLua_createListboxTextItem(const String& text, uint item_id, void* item_data, bool disabled, bool auto_delete);

}
