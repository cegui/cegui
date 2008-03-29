INCLUDES = -I$(top_srcdir)/cegui/include \
           -I$(top_srcdir)/Samples/common/include

noinst_PROGRAMS = Demo8

noinst_HEADERS =  Sample_Demo8.h

Demo8_SOURCES = Sample_Demo8.cpp
Demo8_LDFLAGS = -L$(top_builddir)/cegui/src -L$(top_builddir)/Samples/common/src \
                -L$(top_builddir)/cegui/src/ScriptingModules/LuaScriptModule \
                -L$(top_builddir)/cegui/src/ScriptingModules/LuaScriptModule/support/tolua++
Demo8_LDADD = -lCEGUIBase -lCEGUISampleHelper -lCEGUILuaScriptModule -lCEGUItoluapp


install-exec-local:
	$(LN_S) -f `pwd`/.libs/Demo8$(EXEEXT) $(top_builddir)/bin/Demo8$(EXEEXT)
