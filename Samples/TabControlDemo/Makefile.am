INCLUDES = -I$(top_srcdir)/cegui/include -I$(top_srcdir)/Samples/common/include

noinst_PROGRAMS = TabControlDemo@cegui_bsfx@

if CEGUI_BUILD_STATIC
TabControlDemo@cegui_bsfx@_CPPFLAGS = -DCEGUI_STATIC
endif

TabControlDemo@cegui_bsfx@_SOURCES = Sample_TabControl.cpp
TabControlDemo@cegui_bsfx@_LDADD = $(top_builddir)/cegui/src/libCEGUIBase@cegui_bsfx@.la \
                       $(top_builddir)/Samples/common/src/libCEGUISampleHelper@cegui_bsfx@.la \
					   $(SAMPLE_STATIC_LDFLAGS)

install-exec-local:
	$(LN_S) -f `pwd`/.libs/TabControlDemo@cegui_bsfx@$(EXEEXT) $(top_builddir)/bin/TabControlDemo@cegui_bsfx@$(EXEEXT)
