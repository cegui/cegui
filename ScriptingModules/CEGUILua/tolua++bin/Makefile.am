INCLUDES = -I$(top_srcdir)/ScriptingModules/CEGUILua/tolua++

bin_PROGRAMS = tolua++cegui
tolua__cegui_SOURCES = tolua.c toluabind.c toluabind.h

EXTRA_DIST = MANUAL README

tolua__cegui_LDFLAGS = -L$(top_builddir)/ScriptingModules/CEGUILua/tolua++/.libs
tolua__cegui_LDADD = -lCEGUItoluapp
