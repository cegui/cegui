INCLUDES = -I$(top_srcdir)/cegui/include -I$(top_srcdir)/Samples/common/include

noinst_PROGRAMS = ScrollablePaneDemo@cegui_bsfx@

if CEGUI_BUILD_STATIC
ScrollablePaneDemo@cegui_bsfx@_CPPFLAGS = -DCEGUI_STATIC
endif

ScrollablePaneDemo@cegui_bsfx@_SOURCES = Sample_ScrollablePane.cpp
ScrollablePaneDemo@cegui_bsfx@_LDADD = $(top_builddir)/cegui/src/libCEGUIBase@cegui_bsfx@.la \
                           $(top_builddir)/Samples/common/src/libCEGUISampleHelper@cegui_bsfx@.la \
						   $(SAMPLE_STATIC_LDFLAGS)

install-exec-local:
	$(LN_S) -f `pwd`/.libs/ScrollablePaneDemo@cegui_bsfx@$(EXEEXT) $(top_builddir)/bin/ScrollablePaneDemo@cegui_bsfx@$(EXEEXT)
