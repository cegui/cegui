INCLUDES = -I$(top_srcdir)/include \
           -I$(top_srcdir)/Samples/common/include \
           -I$(top_srcdir)/ScriptingModules/CEGUILua/LuaScriptModule/include \
           -I$(top_srcdir)/ScriptingModules/CEGUILua/lua_and_tolua++/include

noinst_PROGRAMS = Demo8

noinst_HEADERS =  Sample_Demo8.h

Demo8_SOURCES = Sample_Demo8.cpp
Demo8_LDFLAGS = -L$(top_builddir)/src -L$(top_builddir)/Samples/common/src -L$(top_builddir)/ScriptingModules/CEGUILua/LuaScriptModule/src -L$(top_builddir)/ScriptingModules/CEGUILua/lua_and_tolua++/src
Demo8_LDADD = -lCEGUIBase -lCEGUISampleHelper -lCEGUILuaScriptModule -lCEGUIluatoluapp


install-exec-local:
	$(INSTALL_PROGRAM) .libs/Demo8 $(top_builddir)/Samples/bin/Demo8
