INCLUDES = -I$(top_srcdir)/cegui/include -I$(top_srcdir)/Samples/common/include

noinst_PROGRAMS = TreeDemo@cegui_bsfx@
noinst_HEADERS = Sample_TreeDemo.h

if CEGUI_BUILD_STATIC
TreeDemo@cegui_bsfx@_CPPFLAGS = -DCEGUI_STATIC
endif

TreeDemo@cegui_bsfx@_SOURCES = Sample_TreeDemo.cpp
TreeDemo@cegui_bsfx@_LDADD = $(top_builddir)/cegui/src/libCEGUIBase@cegui_bsfx@.la \
                 $(top_builddir)/Samples/common/src/libCEGUISampleHelper@cegui_bsfx@.la \
				 $(SAMPLE_STATIC_LDFLAGS)

install-exec-local:
	$(LN_S) -f `pwd`/.libs/TreeDemo@cegui_bsfx@$(EXEEXT) $(top_builddir)/bin/TreeDemo@cegui_bsfx@$(EXEEXT)
