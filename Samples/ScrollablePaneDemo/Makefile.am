INCLUDES = -I$(top_srcdir)/cegui/include -I$(top_srcdir)/Samples/common/include

noinst_PROGRAMS = ScrollablePaneDemo

ScrollablePaneDemo_SOURCES = Sample_ScrollablePane.cpp
ScrollablePaneDemo_LDADD = $(top_builddir)/cegui/src/libCEGUIBase.la \
                           $(top_builddir)/Samples/common/src/libCEGUISampleHelper.la

install-exec-local:
	$(LN_S) -f `pwd`/.libs/ScrollablePaneDemo$(EXEEXT) $(top_builddir)/bin/ScrollablePaneDemo$(EXEEXT)
