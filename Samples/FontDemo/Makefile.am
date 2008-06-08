INCLUDES = -I$(top_srcdir)/cegui/include -I$(top_srcdir)/Samples/common/include

noinst_PROGRAMS = FontDemo

FontDemo_SOURCES = Sample_FontDemo.cpp
FontDemo_LDADD = $(top_builddir)/cegui/src/libCEGUIBase.la \
                 $(top_builddir)/Samples/common/src/libCEGUISampleHelper.la

install-exec-local:
	$(LN_S) -f `pwd`/.libs/FontDemo$(EXEEXT) $(top_builddir)/bin/FontDemo$(EXEEXT)
