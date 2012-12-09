SUBDIRS = package support

INCLUDES = -I$(top_srcdir)/cegui/include \
           -I$(top_srcdir)/cegui/include/ScriptingModules/LuaScriptModule

libCEGUILuaScriptModule@cegui_bsfx@_la_LDFLAGS = @CEGUI_LIB_LINK_FLAGS@
libCEGUILuaScriptModule@cegui_bsfx@_la_CPPFLAGS = $(Lua_CFLAGS)

if CEGUI_BUILD_STATIC
libCEGUILuaScriptModule@cegui_bsfx@_la_CPPFLAGS += -DCEGUI_STATIC
else
if MINGW32_BUILD
libCEGUILuaScriptModule@cegui_bsfx@_la_CPPFLAGS += -DCEGUILUA_EXPORTS
endif
endif

if CEGUI_BUILD_LUA_MODULE_UNSAFE
libCEGUILuaScriptModule@cegui_bsfx@_la_CPPFLAGS += -DTOLUA_RELEASE
endif

lib_LTLIBRARIES = libCEGUILuaScriptModule@cegui_bsfx@.la

libCEGUILuaScriptModule@cegui_bsfx@_la_SOURCES = lua_CEGUI.cpp \
                                     CEGUILua.cpp \
                                     CEGUILuaFunctor.cpp \
                                     required.cpp
libCEGUILuaScriptModule@cegui_bsfx@_la_LIBADD = $(top_builddir)/cegui/src/libCEGUIBase@cegui_bsfx@.la \
                                    $(Lua_LIBS)
if CEGUI_BUILD_TOLUAPPLIB
INCLUDES += -I$(top_srcdir)/cegui/include/ScriptingModules/LuaScriptModule/support/tolua++
libCEGUILuaScriptModule@cegui_bsfx@_la_LIBADD += support/tolua++/libCEGUItoluapp@cegui_bsfx@.la
else
INCLUDES += $(toluapp_CFLAGS)
libCEGUILuaScriptModule@cegui_bsfx@_la_LIBADD += $(toluapp_LIBS)
endif

noinst_HEADERS = required.h
