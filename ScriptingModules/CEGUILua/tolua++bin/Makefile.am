INCLUDES = -I$(top_srcdir)/ScriptingModules/CEGUILua/tolua++

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

tolua__cegui_LDFLAGS = -L$(top_builddir)/ScriptingModules/CEGUILua/tolua++/.libs
tolua__cegui_LDADD = -lCEGUItoluapp
