/***********************************************************************
	filename: required.cpp
	created:  16/3/2005
	author:   Tomas Lindquist Olsen
	
	purpose:  Implementation of helper functions
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
#include "CEGUI.h"
#include "required.h"

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
    OutStream
*************************************************************************/
void ceguiLua_FileStream_open(FileStream* os, const char* filename)
{
    os->open(filename, std::ios::binary);
}


/************************************************************************
    CEGUI::System::getSystemKeys alternative
*************************************************************************/
bool ceguiLua_System_isSystemKeyDown(const System* sys, SystemKey k)
{
    return (k & sys->getSystemKeys()) != 0;
}


} // namespace CEGUI
