INCLUDES = -I$(top_srcdir)/cegui/include -I$(top_srcdir)/Samples/common/include

noinst_PROGRAMS = TextDemo@cegui_bsfx@

noinst_HEADERS =  Sample_Text.h

if CEGUI_BUILD_STATIC
TextDemo@cegui_bsfx@_CPPFLAGS = -DCEGUI_STATIC
endif

TextDemo@cegui_bsfx@_SOURCES = Sample_Text.cpp
TextDemo@cegui_bsfx@_LDADD = $(top_builddir)/cegui/src/libCEGUIBase@cegui_bsfx@.la \
                 $(top_builddir)/Samples/common/src/libCEGUISampleHelper@cegui_bsfx@.la \
				 $(SAMPLE_STATIC_LDFLAGS)


install-exec-local:
	$(LN_S) -f `pwd`/.libs/TextDemo@cegui_bsfx@$(EXEEXT) $(top_builddir)/bin/TextDemo@cegui_bsfx@$(EXEEXT)
