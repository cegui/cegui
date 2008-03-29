INCLUDES = -I$(top_srcdir)/cegui/include -I$(top_srcdir)/Samples/common/include

noinst_PROGRAMS = FirstWindow
noinst_HEADERS = Sample_FirstWindow.h
FirstWindow_SOURCES = Sample_FirstWindow.cpp
FirstWindow_LDFLAGS = -L$(top_builddir)/cegui/src -L$(top_builddir)/Samples/common/src
FirstWindow_LDADD = -lCEGUIBase -lCEGUISampleHelper

install-exec-local:
	$(LN_S) -f `pwd`/.libs/FirstWindow$(EXEEXT) $(top_builddir)/bin/FirstWindow$(EXEEXT)
