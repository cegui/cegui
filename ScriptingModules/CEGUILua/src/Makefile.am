INCLUDES = -I$(top_srcdir)/include \
           -I$(top_srcdir)/ScriptingModules/CEGUILua/lua_and_tolua++/include \
           -I$(top_srcdir)/ScriptingModules/CEGUILua/LuaScriptModule/include
lib_LTLIBRARIES = libCEGUILuaScriptModule.la

libCEGUILuaScriptModule_la_SOURCES = lua_CEGUI.cpp \
                                     LuaScriptModule.cpp \
                                     required.cpp

noinst_HEADERS = required.h
