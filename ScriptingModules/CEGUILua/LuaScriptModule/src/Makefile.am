INCLUDES = -I$(top_srcdir)/include \
           -I$(top_srcdir)/ScriptingModules/CEGUILua/LuaScriptModule/include

libCEGUILuaScriptModule_la_LDFLAGS =
libCEGUILuaScriptModule_la_CPPFLAGS = $(Lua_CFLAGS)

if MINGW32_BUILD
libCEGUILuaScriptModule_la_CPPFLAGS += -DCEGUILUA_EXPORTS
libCEGUILuaScriptModule_la_LDFLAGS += -no-undefined -avoid-version
endif

libCEGUILuaScriptModule_la_LDFLAGS += -version-info @CEGUI_VERSION_INFO@


lib_LTLIBRARIES = libCEGUILuaScriptModule.la

libCEGUILuaScriptModule_la_SOURCES = lua_CEGUI.cpp \
                                     CEGUILua.cpp \
                                     CEGUILuaFunctor.cpp \
                                     required.cpp
libCEGUILuaScriptModule_la_LIBADD = $(top_builddir)/src/libCEGUIBase.la \
                                    $(Lua_LIBS)
if CEGUI_BUILD_TOLUAPPLIB
INCLUDES += -I$(top_srcdir)/ScriptingModules/CEGUILua/tolua++
libCEGUILuaScriptModule_la_LIBADD += ../../tolua++/libCEGUItoluapp.la
else
INCLUDES += $(toluapp_CFLAGS)
libCEGUILuaScriptModule_la_LIBADD += $(toluapp_LIBS)
endif

noinst_HEADERS = required.h
