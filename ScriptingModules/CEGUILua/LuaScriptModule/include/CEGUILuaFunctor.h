/************************************************************************
    filename: CEGUILuaFunctor.h
    created:  Thu Jan 26 2006
    author:   Tomas Lindquist Olsen <tomas@famolsen.dk>

    purpose:  Defines interface for LuaFunctor class
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
#ifndef _CEGUILuaFunctor_h_
#define _CEGUILuaFunctor_h_

#include "CEGUIEventSet.h"

// include Lua
extern "C" {
#include "lua.h"
}

// Start of CEGUI namespace section
namespace CEGUI
{

// forward declaration
class LuaScriptModule;

/*!
\brief
    Functor class used for subscribing Lua functions to CEGUI events
*/
class LuaFunctor
{
public:
    LuaFunctor(lua_State* state, int func, int selfIndex);
    LuaFunctor(lua_State* state, const String& func, int selfIndex);
    LuaFunctor(const LuaFunctor& cp);
    ~LuaFunctor();

    bool operator()(const EventArgs& args) const;

    /*!
    \brief
        function used to subscribe any Lua function as event handler.
        References using the Lua registry.
        To be called from Lua only.
    */
    static Event::Connection SubscribeEvent(EventSet* self, const String& eventName, int funcIndex, int selfIndex, lua_State* L);

    /*!
    \brief
        Pushes the Lua function named \param name on top of the Lua stack.
        The name may contain '.' (dots) character for (nested) table values.
    */
    static bool pushNamedFunction(lua_State* L, const String& name);

private:
    lua_State* L;
    mutable int index;
    int self;
    mutable bool needs_lookup;
    mutable String function_name;

    friend class LuaScriptModule;
};

} // namespace CEGUI

#endif // end of guard _CEGUILuaFunctor_h_
