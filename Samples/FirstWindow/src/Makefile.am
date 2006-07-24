INCLUDES = -I$(top_srcdir)/Samples/FirstWindow/include -I$(top_srcdir)/include -I$(top_srcdir)/Samples/common/include

noinst_PROGRAMS = FirstWindow

FirstWindow_SOURCES = Sample_FirstWindow.cpp
FirstWindow_LDFLAGS = -L$(top_builddir)/src -L$(top_builddir)/Samples/common/src
FirstWindow_LDADD = -lCEGUIBase -lCEGUISampleHelper

install-exec-local:
	$(LN_S) -f `pwd`/.libs/FirstWindow $(top_builddir)/Samples/bin/FirstWindow
