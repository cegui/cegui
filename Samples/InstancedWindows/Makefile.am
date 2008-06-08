INCLUDES = -I$(top_srcdir)/cegui/include \
           -I$(top_srcdir)/Samples/common/include \
           -I$(top_srcdir)/ScriptingModules/CEGUILua/LuaScriptModule/include

noinst_PROGRAMS = InstancedWindows
noinst_HEADERS = Instanced.h
InstancedWindows_SOURCES = Instanced.cpp
InstancedWindows_LDADD = $(top_builddir)/cegui/src/libCEGUIBase.la \
                         $(top_builddir)/Samples/common/src/libCEGUISampleHelper.la \
                         $(top_builddir)/cegui/src/ScriptingModules/LuaScriptModule/libCEGUILuaScriptModule.la


if CEGUI_BUILD_TOLUAPPLIB
InstancedWindows_LDADD += $(top_builddir)/cegui/src/ScriptingModules/LuaScriptModule/support/tolua++/libCEGUItoluapp.la
else
InstancedWindows_LDADD += $(toluapp_LIBS)
endif

install-exec-local:
	$(LN_S) -f `pwd`/.libs/InstancedWindows$(EXEEXT) $(top_builddir)/bin/InstancedWindows$(EXEEXT)
