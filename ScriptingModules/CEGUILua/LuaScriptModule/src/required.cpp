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
void ceguiLua_WindowManager_writeWindowLayoutToFile(const WindowManager* wm, const Window& window, const String& filename, bool writeParent)
{
    std::ofstream os(filename.c_str(), std::ios::binary);
    wm->writeWindowLayoutToStream(window,os,writeParent);
    os.close();
}


void ceguiLua_WindowManager_writeWindowLayoutToFile(const WindowManager* wm, const String& window, const String& filename, bool writeParent)
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
