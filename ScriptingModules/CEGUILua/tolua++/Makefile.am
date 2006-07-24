INCLUDES =  $(Lua_CFLAGS) -I$(top_srcdir)/ScriptingModules/CEGUILua/tolua++
lib_LTLIBRARIES = libCEGUItoluapp.la

libCEGUItoluapp_la_LDFLAGS = -version-info @CEGUI_VERSION_INFO@

libCEGUItoluapp_la_SOURCES = tolua_event.c \
                             tolua_is.c \
                             tolua_map.c \
                             tolua_push.c \
                             tolua_to.c

libCEGUItoluapp_la_LIBADD = $(Lua_LIBS)

noinst_HEADERS = tolua_event.h
pkginclude_HEADERS = tolua++.h
