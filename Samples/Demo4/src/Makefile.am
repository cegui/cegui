INCLUDES = -I$(top_srcdir)/include -I$(top_srcdir)/Samples/common/include

noinst_PROGRAMS = Demo4

noinst_HEADERS =  Sample_Demo4.h

Demo4_SOURCES = Sample_Demo4.cpp
Demo4_LDFLAGS = -L$(top_builddir)/src -L$(top_builddir)/Samples/common/src
Demo4_LDADD = -lCEGUIBase -lCEGUISampleHelper


install-exec-local:
	$(LN_S) -f `pwd`/.libs/Demo4 $(top_builddir)/Samples/bin/Demo4
