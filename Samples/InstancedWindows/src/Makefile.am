INCLUDES = -I$(top_srcdir)/Samples/Instanced/include \
           -I$(top_srcdir)/include \
           -I$(top_srcdir)/Samples/common/include \
           -I$(top_srcdir)/ScriptingModules/CEGUILua/LuaScriptModule/include

noinst_PROGRAMS = InstancedWindows

InstancedWindows_SOURCES = Instanced.cpp

InstancedWindows_LDADD = $(top_builddir)/src/libCEGUIBase.la \
                         $(top_builddir)/Samples/common/src/libCEGUISampleHelper.la \
                         $(top_builddir)/ScriptingModules/CEGUILua/LuaScriptModule/src/libCEGUILuaScriptModule.la


if CEGUI_BUILD_TOLUAPPLIB
InstancedWindows_LDADD += $(top_builddir)/ScriptingModules/CEGUILua/tolua++/libCEGUItoluapp.la
else
InstancedWindows_LDADD += $(toluapp_LIBS)
endif

install-exec-local:
	$(LN_S) -f `pwd`/.libs/InstancedWindows$(EXEEXT) $(top_builddir)/Samples/bin/InstancedWindows$(EXEEXT)
