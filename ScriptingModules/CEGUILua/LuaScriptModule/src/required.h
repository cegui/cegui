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
#include "CEGUILua.h"
#include "CEGUILuaFunctor.h"

#define __operator_increment    operator++
#define __operator_decrement    operator--
#define __operator_dereference  operator*

#define LuaFunctorSubscribeEvent CEGUI::LuaFunctor::SubscribeEvent

#if defined(_MSC_VER) && !defined(snprintf)
#   define snprintf   _snprintf
#endif

typedef CEGUI::String string;

namespace CEGUI
{

typedef Event::Connection EventConnection;


/*************************************************************************
	Functions for getting Thumb range pairs as two return values
*************************************************************************/
void ceguiLua_Thumb_getHorzRange(Thumb* wnd, float* min, float* max);
void ceguiLua_Thumb_getVertRange(Thumb* wnd, float* min, float* max);


/*************************************************************************
	Functions for creating list box items
*************************************************************************/
ListboxTextItem* ceguiLua_createListboxTextItem(const String& text, uint item_id, void* item_data, bool disabled, bool auto_delete);


/************************************************************************
    Stuff needed to make the iterators work
*************************************************************************/
typedef PropertySet::PropertyIterator PropertyIterator;
typedef EventSet::EventIterator EventIterator;
typedef WindowManager::WindowIterator WindowIterator;
typedef WindowFactoryManager::WindowFactoryIterator WindowFactoryIterator;
typedef WindowFactoryManager::FalagardMappingIterator FalagardMappingIterator;
typedef WindowFactoryManager::FalagardWindowMapping FalagardWindowMapping;
typedef ImagesetManager::ImagesetIterator ImagesetIterator;
typedef Imageset::ImageIterator ImageIterator;
typedef SchemeManager::SchemeIterator SchemeIterator;
typedef FontManager::FontIterator FontIterator;

template <typename T>
inline PropertyIterator ceguiLua_getPropertyIterator(const T* self)
{
    return static_cast<const PropertySet*>(self)->getIterator();
}

template <typename T>
inline EventIterator ceguiLua_getEventIterator(const T* self)
{
    return static_cast<const EventSet*>(self)->getIterator();
}


/************************************************************************
    writeWindowLayoutToStream -> file
*************************************************************************/
void ceguiLua_WindowManager_writeWindowLayoutToFile(const WindowManager* wm, const String& window, const String& filename, bool writeParent);
void ceguiLua_WindowManager_writeWindowLayoutToFile(const WindowManager* wm, const Window& window, const String& filename, bool writeParent);


/************************************************************************
    CEGUI::System::getSystemKeys alternative
*************************************************************************/
bool ceguiLua_System_isSystemKeyDown(const System* sys, SystemKey k);

}
