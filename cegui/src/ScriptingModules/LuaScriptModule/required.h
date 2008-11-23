/***********************************************************************
	filename: required.h
	created:  16/3/2005
	author:   Tomas Lindquist Olsen

	purpose:  Header that includes the necessary stuff needed for the Lua bindings
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
#include "CEGUILua.h"
#include "CEGUILuaFunctor.h"
#include <fstream>

#define __operator_increment    operator++
#define __operator_decrement    operator--
#define __operator_dereference  operator*

#define LuaFunctorSubscribeEvent CEGUI::LuaFunctor::SubscribeEvent

//This is used to keep compilers happy
#define CEGUIDeadException(e) &

#if defined(_MSC_VER) && !defined(snprintf)
#   define snprintf   _snprintf
#endif

typedef CEGUI::String string;

namespace CEGUI
{

/*************************************************************************
    Helper class to enable us to represent an Event::Connection object
    (which is actually a RefCounted<BoundSlot> object)
*************************************************************************/
class EventConnection
{
    RefCounted<BoundSlot> d_slot;

public:
    EventConnection(RefCounted<BoundSlot> slot);
    bool connected() const;
    void disconnect();
};

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
typedef PropertySet::Iterator PropertyIterator;
typedef EventSet::Iterator EventIterator;
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
    OutStream
*************************************************************************/
typedef std::ofstream FileStream;
void ceguiLua_FileStream_open(FileStream*, const char* filename);


/************************************************************************
    CEGUI::System::getSystemKeys alternative
*************************************************************************/
bool ceguiLua_System_isSystemKeyDown(const System* sys, SystemKey k);

}

// the binding file generates alot of
// warning C4800: 'int' : forcing value to bool 'true' or 'false' (performance warning)
#if defined(_MSC_VER)
#   pragma warning(disable : 4800)
#endif
