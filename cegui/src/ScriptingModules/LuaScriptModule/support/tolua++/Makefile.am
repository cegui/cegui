INCLUDES =  $(Lua_CFLAGS) \
            -I$(top_srcdir)/cegui/include/ScriptingModules/LuaScriptModule/support/tolua++

libCEGUItoluapp@cegui_bsfx@_la_LDFLAGS =

if MINGW32_BUILD
libCEGUItoluapp@cegui_bsfx@_la_CPPFLAGS = -DTOLUA_EXPORTS
libCEGUItoluapp@cegui_bsfx@_la_LDFLAGS += -no-undefined
endif

libCEGUItoluapp@cegui_bsfx@_la_LDFLAGS += @CEGUI_LIB_LINK_FLAGS@

lib_LTLIBRARIES = libCEGUItoluapp@cegui_bsfx@.la

libCEGUItoluapp@cegui_bsfx@_la_SOURCES = tolua_event.c \
                             tolua_is.c \
                             tolua_map.c \
                             tolua_push.c \
                             tolua_to.c

libCEGUItoluapp@cegui_bsfx@_la_LIBADD = $(Lua_LIBS)

noinst_HEADERS = tolua_event.h

EXTRA_DIST = INFO
