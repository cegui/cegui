INCLUDES = -I$(top_srcdir)/include -I$(top_srcdir)/Samples/common/include

noinst_PROGRAMS = Demo6

noinst_HEADERS =  Sample_Demo6.h

Demo6_SOURCES = Sample_Demo6.cpp
Demo6_LDADD = $(top_builddir)/src/libCEGUIBase.la \
              $(top_builddir)/Samples/common/src/libCEGUISampleHelper.la


install-exec-local:
	$(LN_S) -f `pwd`/.libs/Demo6$(EXEEXT) $(top_builddir)/Samples/bin/Demo6$(EXEEXT)
