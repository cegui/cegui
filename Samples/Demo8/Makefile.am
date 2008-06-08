INCLUDES = -I$(top_srcdir)/cegui/include \
           -I$(top_srcdir)/Samples/common/include \
           -I$(top_srcdir)/cegui/include/ScriptingModules/LuaScriptModule

noinst_PROGRAMS = Demo8

noinst_HEADERS =  Sample_Demo8.h

Demo8_SOURCES = Sample_Demo8.cpp
Demo8_LDADD = $(top_builddir)/cegui/src/libCEGUIBase.la \
              $(top_builddir)/Samples/common/src/libCEGUISampleHelper.la \
              $(top_builddir)/cegui/src/ScriptingModules/LuaScriptModule/libCEGUILuaScriptModule.la

if CEGUI_BUILD_TOLUAPPLIB
Demo8_LDADD += $(top_builddir)/cegui/src/ScriptingModules/LuaScriptModule/support/tolua++/libCEGUItoluapp.la
else
Demo8_LDADD += $(toluapp_LIBS)
endif


install-exec-local:
	$(LN_S) -f `pwd`/.libs/Demo8$(EXEEXT) $(top_builddir)/bin/Demo8$(EXEEXT)
