INCLUDES = -I$(top_srcdir)/cegui/include -I$(top_srcdir)/Samples/common/include

noinst_PROGRAMS = TabControlDemo

TabControlDemo_SOURCES = Sample_TabControl.cpp
TabControlDemo_LDADD = $(top_builddir)/cegui/src/libCEGUIBase.la \
                       $(top_builddir)/Samples/common/src/libCEGUISampleHelper.la

install-exec-local:
	$(LN_S) -f `pwd`/.libs/TabControlDemo$(EXEEXT) $(top_builddir)/bin/TabControlDemo$(EXEEXT)
