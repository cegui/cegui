INCLUDES = -I$(top_srcdir)/include -I$(top_srcdir)/Samples/common/include

noinst_PROGRAMS = Demo6

noinst_HEADERS =  Sample_Demo6.h

Demo6_SOURCES = Sample_Demo6.cpp
Demo6_LDFLAGS = -L$(top_builddir)/src -L$(top_builddir)/Samples/common/src
Demo6_LDADD = -lCEGUIBase -lCEGUISampleHelper


install-exec-local:
	$(LN_S) -f `pwd`/.libs/Demo6 $(top_builddir)/Samples/bin/Demo6
