INCLUDES = $(Lua_CFLAGS)

bin_PROGRAMS = tolua++cegui
tolua__cegui_SOURCES = tolua.c toluabind.c toluabind.h

EXTRA_DIST = MANUAL README remake_pkg.bat remake_pkg.sh tolua_scons.pkg \
             lua/all.lua \
             lua/array.lua \
             lua/basic.lua \
             lua/class.lua \
             lua/clean.lua \
             lua/code.lua \
             lua/compat-5.1.lua \
             lua/compat.lua \
             lua/container.lua \
             lua/custom.lua \
             lua/custom_hide.lua \
             lua/declaration.lua \
             lua/define.lua \
             lua/doit.lua \
             lua/enumerate.lua \
             lua/feature.lua \
             lua/function.lua \
             lua/module.lua \
             lua/namespace.lua \
             lua/operator.lua \
             lua/package.lua \
             lua/typedef.lua \
             lua/variable.lua \
             lua/verbatim.lua

if CEGUI_BUILD_TOLUAPPLIB
INCLUDES += -I$(top_srcdir)/ScriptingModules/CEGUILua/tolua++
tolua__cegui_LDADD = $(top_builddir)/ScriptingModules/CEGUILua/tolua++/libCEGUItoluapp.la
else
INCLUDES += $(toluapp_CFLAGS)
tolua__cegui_LDADD = $(toluapp_LIBS)
endif