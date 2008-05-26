SUBDIRS = package support

INCLUDES = -I$(top_srcdir)/cegui/include \
           -I$(top_srcdir)/cegui/include/ScriptingModules/LuaScriptModule \
           -I$(top_srcdir)/cegui/include/ScriptingModules/LuaScriptModule/support/tolua++

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
libCEGUILuaScriptModule_la_LIBADD = $(top_builddir)/cegui/src/libCEGUIBase.la \
                                    support/tolua++/libCEGUItoluapp.la \
                                    $(Lua_LIBS)

noinst_HEADERS = required.h
