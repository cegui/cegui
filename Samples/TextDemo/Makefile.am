INCLUDES = -I$(top_srcdir)/cegui/include -I$(top_srcdir)/Samples/common/include

noinst_PROGRAMS = TextDemo

noinst_HEADERS =  Sample_Text.h

TextDemo_SOURCES = Sample_Text.cpp
TextDemo_LDADD = $(top_builddir)/cegui/src/libCEGUIBase.la \
                 $(top_builddir)/Samples/common/src/libCEGUISampleHelper.la


install-exec-local:
	$(LN_S) -f `pwd`/.libs/TextDemo$(EXEEXT) $(top_builddir)/bin/TextDemo$(EXEEXT)
