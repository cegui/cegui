INCLUDES = -I$(top_srcdir)/include -I$(top_srcdir)/Samples/common/include

noinst_PROGRAMS = TextDemo

noinst_HEADERS =  Sample_Text.h

TextDemo_SOURCES = Sample_Text.cpp
TextDemo_LDADD = $(top_builddir)/src/libCEGUIBase.la \
                 $(top_builddir)/Samples/common/src/libCEGUISampleHelper.la


install-exec-local:
	$(LN_S) -f `pwd`/.libs/TextDemo$(EXEEXT) $(top_builddir)/Samples/bin/TextDemo$(EXEEXT)
