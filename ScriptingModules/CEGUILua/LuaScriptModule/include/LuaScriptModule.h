/************************************************************************
    filename:   LuaScriptModule.h
    created:    16/3/2005
    author:     Tomas Lindquist Olsen
*************************************************************************/
#ifndef LuaScriptModule_H
#define LuaScriptModule_H

extern "C" {
#include "lua.h"
} // "C"

namespace CEGUI
{

class LuaScriptModule : public CEGUI::ScriptModule
{
public:
	LuaScriptModule();
	LuaScriptModule(lua_State* state);
	~LuaScriptModule();
	
	void executeScriptFile(const String& filename, const String& resourceGroup);
	int executeScriptGlobal(const String& function_name);
	bool executeScriptedEventHandler(const String& handler_name, const EventArgs& e);
	void executeString(const String& str);
	void createBindings(void);
	void destroyBindings(void);



	lua_State* getLuaState(void) const	{return d_state;}

private:
	bool d_ownsState;
	lua_State* d_state;
};

} // namespace CEGUI

#endif
