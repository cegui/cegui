INCLUDES = -I$(top_srcdir)/ScriptingModules/CEGUILua/lua_and_tolua++/include
lib_LTLIBRARIES = libCEGUIluatoluapp.la

libCEGUIluatoluapp_la_LDFLAGS = -version-info @CEGUI_VERSION_INFO@

libCEGUIluatoluapp_la_SOURCES = lapi.c \
                                lcode.c \
                                ldebug.c \
                                ldo.c \
                                ldump.c \
                                lfunc.c \
                                lgc.c \
                                llex.c \
                                lmem.c \
                                lobject.c \
                                lopcodes.c \
                                lparser.c \
                                lstate.c \
                                lstring.c \
                                ltable.c \
                                ltests.c \
                                ltm.c \
                                lundump.c \
                                lvm.c \
                                lzio.c \
                                lib/lauxlib.c  \
                                lib/lbaselib.c \
                                lib/ldblib.c \
                                lib/liolib.c \
                                lib/lmathlib.c \
                                lib/loadlib.c \
                                lib/lstrlib.c \
                                lib/ltablib.c \
                                tolua++/tolua_event.c \
                                tolua++/tolua_is.c \
                                tolua++/tolua_map.c \
                                tolua++/tolua_push.c \
                                tolua++/tolua_to.c


noinst_HEADERS = lapi.h \
                 lcode.h \
                 ldebug.h \
                 ldo.h \
                 lfunc.h \
                 lgc.h \
                 llex.h \
                 llimits.h \
                 lmem.h \
                 lobject.h \
                 lopcodes.h \
                 lparser.h \
                 lstate.h \
                 lstring.h \
                 ltable.h \
                 ltm.h \
                 lundump.h \
                 lvm.h \
                 lzio.h \
                 tolua++/tolua_event.h
