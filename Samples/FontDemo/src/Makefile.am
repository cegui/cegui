INCLUDES = -I$(top_srcdir)/include -I$(top_srcdir)/Samples/common/include

noinst_PROGRAMS = FontDemo

FontDemo_SOURCES = Sample_FontDemo.cpp
FontDemo_LDFLAGS = -L$(top_builddir)/src -L$(top_builddir)/Samples/common/src
FontDemo_LDADD = -lCEGUIBase -lCEGUISampleHelper

install-exec-local:
	$(LN_S) -f `pwd`/.libs/FontDemo $(top_builddir)/Samples/bin/FontDemo
