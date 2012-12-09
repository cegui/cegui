INCLUDES = -I$(top_srcdir)/cegui/include \
           -I$(top_srcdir)/Samples/common/include \
           -I$(top_srcdir)/cegui/include/ScriptingModules/LuaScriptModule

noinst_PROGRAMS = Demo8@cegui_bsfx@

noinst_HEADERS =  Sample_Demo8.h

if CEGUI_BUILD_STATIC
Demo8@cegui_bsfx@_CPPFLAGS = -DCEGUI_STATIC
endif

Demo8@cegui_bsfx@_SOURCES = Sample_Demo8.cpp
Demo8@cegui_bsfx@_LDADD = $(top_builddir)/cegui/src/libCEGUIBase@cegui_bsfx@.la \
              $(top_builddir)/Samples/common/src/libCEGUISampleHelper@cegui_bsfx@.la \
              $(top_builddir)/cegui/src/ScriptingModules/LuaScriptModule/libCEGUILuaScriptModule@cegui_bsfx@.la \
			  $(SAMPLE_STATIC_LDFLAGS)

if CEGUI_BUILD_STATIC
Demo8@cegui_bsfx@_LDADD += $(top_builddir)/cegui/src/ScriptingModules/LuaScriptModule/libCEGUILuaScriptModule@cegui_bsfx@.la
endif

if CEGUI_BUILD_TOLUAPPLIB
Demo8@cegui_bsfx@_LDADD += $(top_builddir)/cegui/src/ScriptingModules/LuaScriptModule/support/tolua++/libCEGUItoluapp@cegui_bsfx@.la
else
Demo8@cegui_bsfx@_LDADD += $(toluapp_LIBS)
endif


install-exec-local:
	$(LN_S) -f `pwd`/.libs/Demo8@cegui_bsfx@$(EXEEXT) $(top_builddir)/bin/Demo8@cegui_bsfx@$(EXEEXT)
