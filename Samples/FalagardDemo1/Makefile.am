INCLUDES = -I$(top_srcdir)/cegui/include -I$(top_srcdir)/Samples/common/include

noinst_PROGRAMS = FalagardDemo1@cegui_bsfx@
noinst_HEADERS =  Sample_FalagardDemo1.h

if CEGUI_BUILD_STATIC
FalagardDemo1@cegui_bsfx@_CPPFLAGS = -DCEGUI_STATIC
endif

FalagardDemo1@cegui_bsfx@_SOURCES = Sample_FalagardDemo1.cpp
FalagardDemo1@cegui_bsfx@_LDADD = $(top_builddir)/cegui/src/libCEGUIBase@cegui_bsfx@.la \
                      $(top_builddir)/Samples/common/src/libCEGUISampleHelper@cegui_bsfx@.la \
					  $(SAMPLE_STATIC_LDFLAGS)

install-exec-local:
	$(LN_S) -f `pwd`/.libs/FalagardDemo1@cegui_bsfx@$(EXEEXT) $(top_builddir)/bin/FalagardDemo1@cegui_bsfx@$(EXEEXT)
