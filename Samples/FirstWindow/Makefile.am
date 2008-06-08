INCLUDES = -I$(top_srcdir)/cegui/include -I$(top_srcdir)/Samples/common/include

noinst_PROGRAMS = FirstWindow
noinst_HEADERS = Sample_FirstWindow.h
FirstWindow_SOURCES = Sample_FirstWindow.cpp
FirstWindow_LDADD = $(top_builddir)/cegui/src/libCEGUIBase.la \
                    $(top_builddir)/Samples/common/src/libCEGUISampleHelper.la

install-exec-local:
	$(LN_S) -f `pwd`/.libs/FirstWindow$(EXEEXT) $(top_builddir)/bin/FirstWindow$(EXEEXT)
