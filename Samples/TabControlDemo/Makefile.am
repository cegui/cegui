INCLUDES = -I$(top_srcdir)/cegui/include -I$(top_srcdir)/Samples/common/include

noinst_PROGRAMS = TabControlDemo

TabControlDemo_SOURCES = Sample_TabControl.cpp
TabControlDemo_LDFLAGS = -L$(top_builddir)/cegui/src -L$(top_builddir)/Samples/common/src
TabControlDemo_LDADD = -lCEGUIBase -lCEGUISampleHelper

install-exec-local:
	$(LN_S) -f `pwd`/.libs/TabControlDemo$(EXEEXT) $(top_builddir)/bin/TabControlDemo$(EXEEXT)
