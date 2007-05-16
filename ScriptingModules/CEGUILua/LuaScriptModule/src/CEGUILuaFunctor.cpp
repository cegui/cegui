/***********************************************************************
    filename: CEGUILuaFunctor.cpp
    created:  Thu Jan 26 2006
    author:   Tomas Lindquist Olsen <tomas@famolsen.dk>

    purpose:  Implementation for LuaFunctor class
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
#include "CEGUILuaFunctor.h"
#include "CEGUILogger.h"
#include "CEGUIExceptions.h"
#include "CEGUIPropertyHelper.h"

// include Lua libs and tolua++
extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

#include "tolua++.h"

// Start of CEGUI namespace section
namespace CEGUI
{

/*************************************************************************
    Constructor
*************************************************************************/
LuaFunctor::LuaFunctor(lua_State* state, int func, int selfIndex) :
    L(state),
    index(func),
    self(selfIndex),
    needs_lookup(false)
{
}

/*************************************************************************
    Constructor
*************************************************************************/
LuaFunctor::LuaFunctor(lua_State* state, const String& func, int selfIndex) :
    L(state),
    index(LUA_NOREF),
    self(selfIndex),
    needs_lookup(true),
    function_name(func)
{
}

/*************************************************************************
    Constructor
*************************************************************************/
LuaFunctor::LuaFunctor(const LuaFunctor& cp) :
    L(cp.L),
    index(cp.index),
    self(cp.self),
    needs_lookup(cp.needs_lookup),
    function_name(cp.function_name)
{
}

/*************************************************************************
    Destructor
*************************************************************************/
LuaFunctor::~LuaFunctor()
{
    if (self!=LUA_NOREF)
    {
        luaL_unref(L, LUA_REGISTRYINDEX, self);
    }
    if (index!=LUA_NOREF)
    {
        luaL_unref(L, LUA_REGISTRYINDEX, index);
    }
}

/*************************************************************************
    Call operator
*************************************************************************/
bool LuaFunctor::operator()(const EventArgs& args) const
{
    // is this a late binding?
    if (needs_lookup)
    {
        pushNamedFunction(L, function_name);
        // reference function
        index = luaL_ref(L, LUA_REGISTRYINDEX);
        needs_lookup = false;
        CEGUI_LOGINSANE("Late binding of callback '"+function_name+"' performed");
        function_name.clear();
    } // if (needs_lookup)

	ScriptWindowHelper* helper = NULL;
	//Set a global for this window
	if(args.m_hasWindow)
	{
		WindowEventArgs& we = (WindowEventArgs&)args;
		helper = new ScriptWindowHelper(we.window);
		tolua_pushusertype(L,(void*)helper,"CEGUI::ScriptWindowHelper");
		lua_setglobal(L,"this");
	}

    // retrieve function
    lua_rawgeti(L, LUA_REGISTRYINDEX, index);

    // possibly self as well?
    int nargs = 1;
    if (self != LUA_NOREF)
    {
        lua_rawgeti(L, LUA_REGISTRYINDEX, self);
        ++nargs;
    }

    // push EventArgs  parameter
    tolua_pushusertype(L, (void*)&args, "const CEGUI::EventArgs");

    // call it
    int error = lua_pcall(L, nargs, 0, 0);

    // handle errors
    if (error)
    {
        String errStr(lua_tostring(L, -1));
        lua_pop(L, 1);
		if(helper)
		{
			delete helper;
			helper = NULL;
		}
        throw ScriptException("Unable to call Lua event handler:\n\n"+errStr+"\n");
    } // if (error)
	
	if(helper)
	{
		delete helper;
		helper = NULL;
	}

    return true;
}

/*************************************************************************
    do event subscription by reference instead of by name
*************************************************************************/
Event::Connection LuaFunctor::SubscribeEvent(EventSet* self, const String& event_name, int funcIndex, int selfIndex, lua_State* L)
{
    // should we pass a self to the callback?
    int thisIndex = LUA_NOREF;
    if (selfIndex != LUA_NOREF)
    {
        // reference self
        thisIndex = luaL_ref(L, LUA_REGISTRYINDEX);
    }

    // do the real subscription
    int type = lua_type(L,-1);
    Event::Connection con;
    if (type == LUA_TFUNCTION)
    {
        // reference function
        int index = luaL_ref(L, LUA_REGISTRYINDEX);
        LuaFunctor functor(L,index,thisIndex);
        con = self->subscribeEvent(String(event_name), Event::Subscriber(functor));
        // make sure we don't release the reference(s) we just made when this call returns
        functor.index = LUA_NOREF;
        functor.self = LUA_NOREF;
    }
    else if (type == LUA_TSTRING)
    {
        const char* str = lua_tostring(L, -1);
        LuaFunctor functor(L,String(str),thisIndex);
        con = self->subscribeEvent(String(event_name), Event::Subscriber(functor));
        // make sure we don't release the reference we just (maybe) made when this call returns
        functor.self = LUA_NOREF;
    }
    else
    {
        luaL_error(L,"bad function passed to subscribe function. must be a real function, or a string for late binding");
    }

    // return the event connection
    return con;
}

/*************************************************************************
    Pushes a named function on the stack
*************************************************************************/
void LuaFunctor::pushNamedFunction(lua_State* L, const String& handler_name)
{
    int top = lua_gettop(L);

    // do we have any dots in the handler name? if so we grab the function as a table field
    String::size_type i = handler_name.find_first_of((utf32)'.');
    if (i!=String::npos)
    {
        // split the rest of the string up in parts seperated by '.'
        // TODO: count the dots and size the vector accordingly from the beginning.
        std::vector<String> parts;
        String::size_type start = 0;
        do
        {
            parts.push_back(handler_name.substr(start,i-start));
            start = i+1;
            i = handler_name.find_first_of((utf32)'.',start);
        } while(i!=String::npos);

        // add last part
        parts.push_back(handler_name.substr(start));

        // first part is the global
        lua_getglobal(L, parts[0].c_str());
        if (!lua_istable(L,-1))
        {
            lua_settop(L,top);
            throw ScriptException("Unable to get the Lua event handler: '"+handler_name+"' as first part is not a table");
        }

        // if there is more than two parts, we have more tables to go through
        std::vector<String>::size_type visz = parts.size();
        if (visz-- > 2) // avoid subtracting one later on
        {
            // go through all the remaining parts to (hopefully) have a valid Lua function in the end
            std::vector<String>::size_type vi = 1;
            while (vi<visz)
            {
                // push key, and get the next table
                lua_pushstring(L,parts[vi].c_str());
                lua_gettable(L,-2);
                if (!lua_istable(L,-1))
                {
                    lua_settop(L,top);
                    throw ScriptException("Unable to get the Lua event handler: '"+handler_name+"' as part #"+PropertyHelper::uintToString(uint(vi+1))+" ("+parts[vi]+") is not a table");
                }
                // get rid of the last table and move on
                lua_remove(L,-2);
                vi++;
            }
        }

        // now we are ready to get the function to call ... phew :)
        lua_pushstring(L,parts[visz].c_str());
        lua_gettable(L,-2);
        lua_remove(L,-2); // get rid of the table
    }
    // just a regular global function
    else
    {
        lua_getglobal(L, handler_name.c_str());
    }

    // is it a function
    if (!lua_isfunction(L,-1))
    {
        lua_settop(L,top);
        throw ScriptException("The Lua event handler: '"+handler_name+"' does not represent a Lua function");
    }
}

} // namespace CEGUI
