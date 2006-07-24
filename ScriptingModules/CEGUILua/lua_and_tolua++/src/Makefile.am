INCLUDES =  $(Lua_CFLAGS) -I$(top_srcdir)/ScriptingModules/CEGUILua/lua_and_tolua++/include
lib_LTLIBRARIES = libCEGUIluatoluapp.la

libCEGUIluatoluapp_la_LDFLAGS = -version-info @CEGUI_VERSION_INFO@

libCEGUIluatoluapp_la_SOURCES = tolua++/tolua_event.c \
                                tolua++/tolua_is.c \
                                tolua++/tolua_map.c \
                                tolua++/tolua_push.c \
                                tolua++/tolua_to.c

libCEGUIluatoluapp_la_LIBADD = $(Lua_LIBS)

noinst_HEADERS = tolua++/tolua_event.h
