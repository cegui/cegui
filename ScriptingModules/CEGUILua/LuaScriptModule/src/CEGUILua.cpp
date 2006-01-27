/************************************************************************
	filename: CEGUILua.cpp
	created:  16/3/2005
	author:   Tomas Lindquist Olsen
	
	purpose:  Implementation for LuaScriptModule class
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
#include "CEGUIPropertyHelper.h"
#include "CEGUILua.h"
#include "CEGUILuaFunctor.h"
#include <vector>

// include Lua libs and tolua++
extern "C" {
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
	System::getSingleton().getResourceProvider()->loadRawDataContainer(filename, raw, resourceGroup);

	// load code into lua and call it
	int top = lua_gettop(d_state);
	int error = luaL_loadbuffer(d_state, (char*)raw.getDataPtr(), raw.getSize(), filename.c_str()) || lua_pcall(d_state,0,0,0);

	System::getSingleton().getResourceProvider()->unloadRawDataContainer( raw );

	// handle errors
	if (error)
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

    // push EventArgs as the first parameter
    tolua_pushusertype(d_state,(void*)&e,"const CEGUI::EventArgs");

    // call it
    int error = lua_pcall(d_state,1,0,0);

    // handle errors
    if (error)
    {
        String errStr(lua_tostring(d_state,-1));
        lua_pop(d_state,1);
        throw ScriptException("Unable to evaluate the Lua event handler: '"+handler_name+"'\n\n"+errStr+"\n");
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
