INCLUDES = -I$(top_srcdir)/include -I$(top_srcdir)/Samples/common/include

noinst_PROGRAMS = TextDemo

noinst_HEADERS =  Sample_Text.h

TextDemo_SOURCES = Sample_Text.cpp
TextDemo_LDFLAGS = -L$(top_builddir)/src -L$(top_builddir)/Samples/common/src
TextDemo_LDADD = -lCEGUIBase -lCEGUISampleHelper


install-exec-local:
	$(LN_S) -f `pwd`/.libs/TextDemo$(EXEEXT) $(top_builddir)/Samples/bin/TextDemo$(EXEEXT)
