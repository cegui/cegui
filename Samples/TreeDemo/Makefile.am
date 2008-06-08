INCLUDES = -I$(top_srcdir)/cegui/include -I$(top_srcdir)/Samples/common/include

noinst_PROGRAMS = TreeDemo
noinst_HEADERS = Sample_TreeDemo.h
TreeDemo_SOURCES = Sample_TreeDemo.cpp
TreeDemo_LDADD = $(top_builddir)/cegui/src/libCEGUIBase.la \
                 $(top_builddir)/Samples/common/src/libCEGUISampleHelper.la

install-exec-local:
	$(LN_S) -f `pwd`/.libs/TreeDemo$(EXEEXT) $(top_builddir)/bin/TreeDemo$(EXEEXT)
