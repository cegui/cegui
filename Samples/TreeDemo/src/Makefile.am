INCLUDES = -I$(top_srcdir)/Samples/TreeDemo/include -I$(top_srcdir)/include -I$(top_srcdir)/Samples/common/include

noinst_PROGRAMS = TreeDemo

TreeDemo_SOURCES = Sample_TreeDemo.cpp
TreeDemo_LDADD = $(top_builddir)/src/libCEGUIBase.la \
                 $(top_builddir)/Samples/common/src/libCEGUISampleHelper.la

install-exec-local:
	$(LN_S) -f `pwd`/.libs/TreeDemo$(EXEEXT) $(top_builddir)/Samples/bin/TreeDemo$(EXEEXT)
