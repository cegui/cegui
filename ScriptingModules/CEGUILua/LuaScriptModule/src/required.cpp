/************************************************************************
	filename: required.cpp
	created:  16/3/2005
	author:   Tomas Lindquist Olsen
	
	purpose:  Implementation of helper functions
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


// Start of CEGUI namespace section
namespace CEGUI
{

/*************************************************************************
	Helper functions for easy casting of the EventArgs class
*************************************************************************/

// converts EventArgs to MouseCursorEventArgs
const MouseCursorEventArgs& ceguiLua_toMouseCursorEventArgs(const EventArgs& e)
{
	return (const MouseCursorEventArgs&)e;
}


// converts EventArgs to WindowEventArgs
const WindowEventArgs& ceguiLua_toWindowEventArgs(const EventArgs& e)
{
return (const WindowEventArgs&)e;
}


// converts EventArgs to ActivationEventArgs
const ActivationEventArgs& ceguiLua_toActivationEventArgs(const EventArgs& e)
{
return (const ActivationEventArgs&)e;
}


// converts EventArgs to HeaderSequenceEventArgs
const HeaderSequenceEventArgs& ceguiLua_toHeaderSequenceEventArgs(const EventArgs& e)
{
	return (const HeaderSequenceEventArgs&)e;
}


// converts EventArgs to MouseEventArgs
const MouseEventArgs& ceguiLua_toMouseEventArgs(const EventArgs& e)
{
	return (const MouseEventArgs&)e;
}


// converts EventArgs to KeyEventArgs
const KeyEventArgs& ceguiLua_toKeyEventArgs(const EventArgs& e)
{
	return (const KeyEventArgs&)e;
}


// converts EventArgs to DragDropEventArgs
const DragDropEventArgs& ceguiLua_toDragDropEventArgs(const EventArgs& e)
{
	return (const DragDropEventArgs&)e;
}


/*************************************************************************
	Helper functions for easy casting of the classes derived from
	CEGUI::Window (the widgets)
*************************************************************************/

ButtonBase*			ceguiLua_toButtonBase(Window* w)
{
	return (ButtonBase*)w;
}


Checkbox*			ceguiLua_toCheckbox(Window* w)
{
	return (Checkbox*)w;
}


Combobox*			ceguiLua_toCombobox(Window* w)
{
	return (Combobox*)w;
}


ComboDropList*		ceguiLua_toComboDropList(Window* w)
{
	return (ComboDropList*)w;
}


DragContainer*		ceguiLua_toDragContainer(Window* w)
{
	return (DragContainer*)w;
}


Editbox*			ceguiLua_toEditbox(Window* w)
{
	return (Editbox*)w;
}


FrameWindow*		ceguiLua_toFrameWindow(Window* w)
{
	return (FrameWindow*)w;
}


GUISheet*			ceguiLua_toGUISheet(Window* w)
{
	return (GUISheet*)w;
}


ItemEntry*			ceguiLua_toItemEntry(Window* w)
{
	return (ItemEntry*)w;
}


ItemListBase*		ceguiLua_toItemListBase(Window* w)
{
	return (ItemListBase*)w;
}


Listbox*			ceguiLua_toListbox(Window* w)
{
	return (Listbox*)w;
}


ListHeader*			ceguiLua_toListHeader(Window* w)
{
	return (ListHeader*)w;
}


ListHeaderSegment*	ceguiLua_toListHeaderSegment(Window* w)
{
	return (ListHeaderSegment*)w;
}


Menubar*			ceguiLua_toMenubar(Window* w)
{
	return (Menubar*)w;
}


MenuBase*			ceguiLua_toMenuBase(Window* w)
{
	return (MenuBase*)w;
}


MenuItem*			ceguiLua_toMenuItem(Window* w)
{
	return (MenuItem*)w;
}


MultiColumnList*	ceguiLua_toMultiColumnList(Window* w)
{
	return (MultiColumnList*)w;
}


MultiLineEditbox*	ceguiLua_toMultiLineEditbox(Window* w)
{
	return (MultiLineEditbox*)w;
}


PopupMenu*			ceguiLua_toPopupMenu(Window* w)
{
	return (PopupMenu*)w;
}


ProgressBar*		ceguiLua_toProgressBar(Window* w)
{
	return (ProgressBar*)w;
}


PushButton*			ceguiLua_toPushButton(Window* w)
{
	return (PushButton*)w;
}


RadioButton*		ceguiLua_toRadioButton(Window* w)
{
	return (RadioButton*)w;
}


ScrollablePane*		ceguiLua_toScrollablePane(Window* w)
{
	return (ScrollablePane*)w;
}


Scrollbar*			ceguiLua_toScrollbar(Window* w)
{
	return (Scrollbar*)w;
}


ScrolledContainer*	ceguiLua_toScrolledContainer(Window* w)
{
	return (ScrolledContainer*)w;
}


Slider*				ceguiLua_toSlider(Window* w)
{
	return (Slider*)w;
}


Spinner*			ceguiLua_toSpinner(Window* w)
{
	return (Spinner*)w;
}


TabButton*			ceguiLua_toTabButton(Window* w)
{
	return (TabButton*)w;
}


TabControl*			ceguiLua_toTabControl(Window* w)
{
	return (TabControl*)w;
}


TabPane*			ceguiLua_toTabPane(Window* w)
{
	return (TabPane*)w;
}


Thumb*				ceguiLua_toThumb(Window* w)
{
	return (Thumb*)w;
}


Tooltip*			ceguiLua_toTooltip(Window* w)
{
	return (Tooltip*)w;
}


/*************************************************************************
	Functions for getting Thumb range pairs as two return values
*************************************************************************/

// returns horizontal range as two values for lua
void ceguiLua_Thumb_getHorzRange(Thumb* wnd, float* min, float* max)
{
	std::pair<float,float> range_pair = wnd->getHorzRange();
	*min = range_pair.first;
	*max = range_pair.second;
}


// returns vertical range as two values for lua
void ceguiLua_Thumb_getVertRange(Thumb* wnd, float* min, float* max)
{
	std::pair<float,float> range_pair = wnd->getVertRange();
	*min = range_pair.first;
	*max = range_pair.second;
}


/*************************************************************************
	Functions for creating list box items
*************************************************************************/

// allocates and returns a new ListboxTextItem
ListboxTextItem* ceguiLua_createListboxTextItem(const String& text, uint item_id, void* item_data, bool disabled, bool auto_delete)
{
	return new ListboxTextItem(text,item_id,item_data,disabled,auto_delete);
}


/************************************************************************
    writeWindowLayoutToFile
*************************************************************************/
void ceguiLua_WindowManager_writeWindowLayoutToStream(const WindowManager* wm, const Window& window, const String& filename, bool writeParent)
{
    std::ofstream os(filename.c_str(), std::ios::binary);
    wm->writeWindowLayoutToStream(window,os,writeParent);
    os.close();
}


void ceguiLua_WindowManager_writeWindowLayoutToStream(const WindowManager* wm, const String& window, const String& filename, bool writeParent)
{
    std::ofstream os(filename.c_str(), std::ios::binary);
    wm->writeWindowLayoutToStream(window,os,writeParent);
    os.close();
}


/************************************************************************
    CEGUI::System::getSystemKeys alternative
*************************************************************************/
bool ceguiLua_System_isSystemKeyDown(const System* sys, SystemKey k)
{
    return (k & sys->getSystemKeys()) != 0;
}


} // namespace CEGUI
