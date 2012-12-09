INCLUDES = -I$(top_srcdir)/cegui/include -I$(top_srcdir)/Samples/common/include

noinst_PROGRAMS = FontDemo@cegui_bsfx@

if CEGUI_BUILD_STATIC
FontDemo@cegui_bsfx@_CPPFLAGS = -DCEGUI_STATIC
endif

FontDemo@cegui_bsfx@_SOURCES = Sample_FontDemo.cpp
FontDemo@cegui_bsfx@_LDADD = $(top_builddir)/cegui/src/libCEGUIBase@cegui_bsfx@.la \
                 $(top_builddir)/Samples/common/src/libCEGUISampleHelper@cegui_bsfx@.la \
				 $(SAMPLE_STATIC_LDFLAGS)

install-exec-local:
	$(LN_S) -f `pwd`/.libs/FontDemo@cegui_bsfx@$(EXEEXT) $(top_builddir)/bin/FontDemo@cegui_bsfx@$(EXEEXT)
