INCLUDES = -I$(top_srcdir)/Samples/TreeDemo/include -I$(top_srcdir)/include -I$(top_srcdir)/Samples/common/include

noinst_PROGRAMS = TreeDemo

TreeDemo_SOURCES = Sample_TreeDemo.cpp
TreeDemo_LDFLAGS = -L$(top_builddir)/src -L$(top_builddir)/Samples/common/src
TreeDemo_LDADD = -lCEGUIBase -lCEGUISampleHelper

install-exec-local:
	$(LN_S) -f `pwd`/.libs/TreeDemo $(top_builddir)/Samples/bin/TreeDemo
