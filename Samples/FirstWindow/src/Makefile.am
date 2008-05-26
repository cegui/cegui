INCLUDES = -I$(top_srcdir)/Samples/FirstWindow/include -I$(top_srcdir)/include -I$(top_srcdir)/Samples/common/include

noinst_PROGRAMS = FirstWindow

FirstWindow_SOURCES = Sample_FirstWindow.cpp
FirstWindow_LDADD = $(top_builddir)/src/libCEGUIBase.la \
                    $(top_builddir)/Samples/common/src/libCEGUISampleHelper.la

install-exec-local:
	$(LN_S) -f `pwd`/.libs/FirstWindow$(EXEEXT) $(top_builddir)/Samples/bin/FirstWindow$(EXEEXT)
