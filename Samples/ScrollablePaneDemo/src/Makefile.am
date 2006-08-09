INCLUDES = -I$(top_srcdir)/include -I$(top_srcdir)/Samples/common/include

noinst_PROGRAMS = ScrollablePaneDemo

ScrollablePaneDemo_SOURCES = Sample_ScrollablePane.cpp
ScrollablePaneDemo_LDFLAGS = -L$(top_builddir)/src -L$(top_builddir)/Samples/common/src
ScrollablePaneDemo_LDADD = -lCEGUIBase -lCEGUISampleHelper

install-exec-local:
	$(LN_S) -f `pwd`/.libs/ScrollablePaneDemo $(top_builddir)/Samples/bin/ScrollablePaneDemo
