INCLUDES = -I$(top_srcdir)/include \
           -I$(top_srcdir)/ScriptingModules/CEGUILua/tolua++ \
           -I$(top_srcdir)/ScriptingModules/CEGUILua/LuaScriptModule/include
lib_LTLIBRARIES = libCEGUILuaScriptModule.la

libCEGUILuaScriptModule_la_LDFLAGS = -version-info @CEGUI_VERSION_INFO@

libCEGUILuaScriptModule_la_SOURCES = lua_CEGUI.cpp \
                                     CEGUILua.cpp \
                                     CEGUILuaFunctor.cpp \
                                     required.cpp

noinst_HEADERS = required.h
