INCLUDES = -I$(top_srcdir)/include -I$(top_srcdir)/Samples/common/include

noinst_PROGRAMS = Demo7

noinst_HEADERS =  Sample_Demo7.h

Demo7_SOURCES = Sample_Demo7.cpp
Demo7_LDFLAGS = -L$(top_builddir)/src -L$(top_builddir)/Samples/common/src
Demo7_LDADD = -lCEGUIBase -lCEGUISampleHelper


install-exec-local:
	$(LN_S) -f `pwd`/.libs/Demo7 $(top_builddir)/Samples/bin/Demo7
