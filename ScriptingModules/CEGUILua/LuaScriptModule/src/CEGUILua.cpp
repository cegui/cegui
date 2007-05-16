/***********************************************************************
	filename: CEGUILua.cpp
	created:  16/3/2005
	author:   Tomas Lindquist Olsen
	
	purpose:  Implementation for LuaScriptModule class
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
#include "CEGUIPropertyHelper.h"
#include "CEGUILua.h"
#include "CEGUILuaFunctor.h"
#include <vector>

// include Lua libs and tolua++
extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

#include "tolua++.h"

// prototype for bindings initialisation function
int tolua_CEGUI_open(lua_State* tolua_S);


// Start of CEGUI namespace section
namespace CEGUI
{

/*************************************************************************
	Constructor (creates Lua state)
*************************************************************************/
LuaScriptModule::LuaScriptModule()
{
	// create a lua state
	d_ownsState = true;
	d_state = lua_open();

	// init all standard libraries
	luaopen_base(d_state);
	luaopen_io(d_state);
	luaopen_string(d_state);
	luaopen_table(d_state);
	luaopen_math(d_state);
#if defined(DEBUG) || defined (_DEBUG)
	luaopen_debug(d_state);
#endif

	setModuleIdentifierString();
}


/*************************************************************************
	Constructor (uses given Lua state)
*************************************************************************/
LuaScriptModule::LuaScriptModule(lua_State* state)
{
	// just use the given state
	d_ownsState = false;
	d_state = state;

	setModuleIdentifierString();
}


/*************************************************************************
	Destructor
*************************************************************************/
LuaScriptModule::~LuaScriptModule()
{
	if ( d_ownsState && d_state )
	{
		lua_close( d_state );
	}
}


/*************************************************************************
	Execute script file
*************************************************************************/
void LuaScriptModule::executeScriptFile(const String& filename, const String& resourceGroup)
{
	// load file
	RawDataContainer raw;
	System::getSingleton().getResourceProvider()->loadRawDataContainer(filename,
        raw, resourceGroup.empty() ? d_defaultResourceGroup : resourceGroup);

	// load code into lua
	int top = lua_gettop(d_state);
	int loaderr = luaL_loadbuffer(d_state, (char*)raw.getDataPtr(), raw.getSize(), filename.c_str());
	System::getSingleton().getResourceProvider()->unloadRawDataContainer( raw );
	if (loaderr)
	{
	    String errMsg = lua_tostring(d_state,-1);
		lua_settop(d_state,top);
		throw ScriptException("Unable to execute Lua script file: '"+filename+"'\n\n"+errMsg+"\n");
	}
    
    // call it
	if (lua_pcall(d_state,0,0,0))
	{
	    String errMsg = lua_tostring(d_state,-1);
		lua_settop(d_state,top);
		throw ScriptException("Unable to execute Lua script file: '"+filename+"'\n\n"+errMsg+"\n");
	}

	lua_settop(d_state,top); // just in case :P
}


/*************************************************************************
	Execute global script function
*************************************************************************/
int	LuaScriptModule::executeScriptGlobal(const String& function_name)
{
    int top = lua_gettop(d_state);

    // get the function from lua
    lua_getglobal(d_state, function_name.c_str());

    // is it a function
    if (!lua_isfunction(d_state,-1))
    {
        lua_settop(d_state,top);
        throw ScriptException("Unable to get Lua global: '"+function_name+"' as name not represent a global Lua function" );
    }

    // call it
    int error = lua_pcall(d_state,0,1,0);		

    // handle errors
    if (error)
    {
        String errMsg = lua_tostring(d_state,-1);
        lua_pop(d_state,1);
        throw ScriptException("Unable to evaluate Lua global: '"+function_name+"\n\n"+errMsg+"\n");
    }

    // get return value
    if (!lua_isnumber(d_state,-1))
    {
        // log that return value is invalid. return -1 and move on.
        lua_settop(d_state,top);
        ScriptException("Unable to get Lua global : '"+function_name+"' return value as it's not a number" );
        return -1;
    }

    int ret = (int)lua_tonumber(d_state,-1);
    lua_pop(d_state,1);

    // return it
    return ret;
}


/*************************************************************************
	Execute scripted event handler
*************************************************************************/
bool LuaScriptModule::executeScriptedEventHandler(const String& handler_name, const EventArgs& e)
{

	LuaFunctor::pushNamedFunction(d_state, handler_name);

	ScriptWindowHelper* helper = NULL;
	//If this is an event that was triggered by a window then make a "this" pointer to the window for the script.
	if(e.m_hasWindow)
	{
		WindowEventArgs& we = (WindowEventArgs&)e;
		helper = new ScriptWindowHelper(we.window);
		lua_pushlightuserdata(d_state,(void*)helper);
		lua_setglobal(d_state,"this");
	} // if(e.m_hasWindow)

    // push EventArgs as the first parameter
    tolua_pushusertype(d_state,(void*)&e,"const CEGUI::EventArgs");

    // call it
    int error = lua_pcall(d_state,1,0,0);

    // handle errors
    if (error)
    {
        String errStr(lua_tostring(d_state,-1));
        lua_pop(d_state,1);
		//cleanup the helper object if any
		if(helper)
		{
			delete helper;
			helper = NULL;
		}
        throw ScriptException("Unable to evaluate the Lua event handler: '"+handler_name+"'\n\n"+errStr+"\n");
    } // if (error)

	if(helper)
	{
		delete helper;
		helper = NULL;
	}

    return true;
}


/*************************************************************************
	Execute script code string
*************************************************************************/
void LuaScriptModule::executeString(const String& str)
{
    int top = lua_gettop(d_state);

    // load code into lua and call it
    int error =	luaL_loadbuffer(d_state, str.c_str(), str.length(), str.c_str()) || lua_pcall(d_state,0,0,0);

    // handle errors
    if (error)
    {
        String errMsg = lua_tostring(d_state,-1);
        lua_settop(d_state,top);
        throw ScriptException("Unable to execute Lua script string: '"+str+"'\n\n"+errMsg+"\n");
    }
}
	

/*************************************************************************
	Create Lua bindings
*************************************************************************/
void LuaScriptModule::createBindings(void)
{
	CEGUI::Logger::getSingleton().logEvent( "---- Creating Lua bindings ----" );
	// init CEGUI module
	tolua_CEGUI_open(d_state);
}


/*************************************************************************
	Destroy Lua bindings
*************************************************************************/
void LuaScriptModule::destroyBindings(void)
{
	CEGUI::Logger::getSingleton().logEvent( "---- Destroying Lua bindings ----" );
	// is this ok ?
	lua_pushnil(d_state);
	lua_setglobal(d_state,"CEGUI");
}


/*************************************************************************
	Set the ID string for the module
*************************************************************************/
void LuaScriptModule::setModuleIdentifierString()
{
    // set ID string
    d_identifierString = "CEGUI::LuaScriptModule - Official Lua based scripting module for CEGUI";
	d_language = "Lua";
}


/*************************************************************************
	Subscribe to a scripted event handler
*************************************************************************/
Event::Connection LuaScriptModule::subscribeEvent(EventSet* target, const String& event_name, const String& subscriber_name)
{
    // do the real subscription
    LuaFunctor functor(d_state,subscriber_name,LUA_NOREF);
    Event::Connection con = target->subscribeEvent(event_name, Event::Subscriber(functor));
    // make sure we don't release the reference we just made when this call returns
    functor.index = LUA_NOREF;

    // return the event connection
    return con;
}


/*************************************************************************
	Subscribe to a scripted event handler
*************************************************************************/
Event::Connection LuaScriptModule::subscribeEvent(EventSet* target, const String& event_name, Event::Group group, const String& subscriber_name)
{
    // do the real subscription
    LuaFunctor functor(d_state,subscriber_name,LUA_NOREF);
    Event::Connection con = target->subscribeEvent(event_name, group, Event::Subscriber(functor));

    // return the event connection
    return con;
}

} // namespace CEGUI
