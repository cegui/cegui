INCLUDES = -I$(top_srcdir)/include \
           -I$(top_srcdir)/ScriptingModules/CEGUILua/tolua++ \
           -I$(top_srcdir)/ScriptingModules/CEGUILua/LuaScriptModule/include

libCEGUILuaScriptModule_la_LDFLAGS =
libCEGUILuaScriptModule_la_CPPFLAGS = $(Lua_CFLAGS)

if MINGW32_BUILD
libCEGUILuaScriptModule_la_CPPFLAGS += -DCEGUILUA_EXPORTS
libCEGUILuaScriptModule_la_LDFLAGS += -no-undefined -avoid-version
libCEGUILuaScriptModule_la_LIBADD = -L$(top_builddir)/src/ \
                                    -L$(top_builddir)/ScriptingModules/CEGUILua/tolua++ \
                                    -lCEGUIBase -lCEGUItoluapp $(Lua_LIBS)
endif

libCEGUILuaScriptModule_la_LDFLAGS += -version-info @CEGUI_VERSION_INFO@


lib_LTLIBRARIES = libCEGUILuaScriptModule.la

libCEGUILuaScriptModule_la_SOURCES = lua_CEGUI.cpp \
                                     CEGUILua.cpp \
                                     CEGUILuaFunctor.cpp \
                                     required.cpp

noinst_HEADERS = required.h
