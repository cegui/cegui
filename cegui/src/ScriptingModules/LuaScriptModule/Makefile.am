SUBDIRS = package support

INCLUDES = -I$(top_srcdir)/cegui/include \
           -I$(top_srcdir)/cegui/include/ScriptingModules/LuaScriptModule

libCEGUILuaScriptModule_la_LDFLAGS =
libCEGUILuaScriptModule_la_CPPFLAGS = $(Lua_CFLAGS)

if MINGW32_BUILD
libCEGUILuaScriptModule_la_CPPFLAGS += -DCEGUILUA_EXPORTS
libCEGUILuaScriptModule_la_LDFLAGS += -no-undefined -avoid-version
endif

libCEGUILuaScriptModule_la_LDFLAGS += @CEGUI_LIB_LINK_FLAGS@


lib_LTLIBRARIES = libCEGUILuaScriptModule.la

libCEGUILuaScriptModule_la_SOURCES = lua_CEGUI.cpp \
                                     CEGUILua.cpp \
                                     CEGUILuaFunctor.cpp \
                                     required.cpp
libCEGUILuaScriptModule_la_LIBADD = $(top_builddir)/cegui/src/libCEGUIBase.la \
                                    $(Lua_LIBS)
if CEGUI_BUILD_TOLUAPPLIB
INCLUDES += -I$(top_srcdir)/cegui/include/ScriptingModules/LuaScriptModule/support/tolua++
libCEGUILuaScriptModule_la_LIBADD += support/tolua++/libCEGUItoluapp.la
else
INCLUDES += $(toluapp_CFLAGS)
libCEGUILuaScriptModule_la_LIBADD += $(toluapp_LIBS)
endif

noinst_HEADERS = required.h
