INCLUDES = -I$(top_srcdir)/cegui/include -I$(top_srcdir)/Samples/common/include

noinst_PROGRAMS = TreeDemo
noinst_HEADERS = Sample_TreeDemo.h
TreeDemo_SOURCES = Sample_TreeDemo.cpp
TreeDemo_LDFLAGS = -L$(top_builddir)/cegui/src -L$(top_builddir)/Samples/common/src
TreeDemo_LDADD = -lCEGUIBase -lCEGUISampleHelper

install-exec-local:
	$(LN_S) -f `pwd`/.libs/TreeDemo$(EXEEXT) $(top_builddir)/bin/TreeDemo$(EXEEXT)
