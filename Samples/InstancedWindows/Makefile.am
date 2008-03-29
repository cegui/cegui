INCLUDES = -I$(top_srcdir)/cegui/include \
           -I$(top_srcdir)/Samples/common/include

noinst_PROGRAMS = InstancedWindows
noinst_HEADERS = Instanced.h
InstancedWindows_SOURCES = Instanced.cpp
InstancedWindows_LDFLAGS = -L$(top_builddir)/cegui/src -L$(top_builddir)/Samples/common/src \
                           -L$(top_builddir)/cegui/src/ScriptingModules/LuaScriptModule \
                           -L$(top_builddir)/cegui/src/ScriptingModules/LuaScriptModule/support/tolua++
InstancedWindows_LDADD = -lCEGUIBase -lCEGUISampleHelper -lCEGUILuaScriptModule -lCEGUItoluapp

install-exec-local:
	$(LN_S) -f `pwd`/.libs/InstancedWindows$(EXEEXT) $(top_builddir)/bin/InstancedWindows$(EXEEXT)
