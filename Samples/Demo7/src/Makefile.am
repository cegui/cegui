INCLUDES = -I$(top_srcdir)/include -I$(top_srcdir)/Samples/common/include

noinst_PROGRAMS = Demo7

noinst_HEADERS =  Sample_Demo7.h

Demo7_SOURCES = Sample_Demo7.cpp
Demo7_LDADD = $(top_builddir)/src/libCEGUIBase.la \
              $(top_builddir)/Samples/common/src/libCEGUISampleHelper.la


install-exec-local:
	$(LN_S) -f `pwd`/.libs/Demo7$(EXEEXT) $(top_builddir)/Samples/bin/Demo7$(EXEEXT)
