INCLUDES = -I$(top_srcdir)/Samples/FalagardDemo1/include -I$(top_srcdir)/include -I$(top_srcdir)/Samples/common/include

noinst_PROGRAMS = FalagardDemo1

FalagardDemo1_SOURCES = Sample_FalagardDemo1.cpp
FalagardDemo1_LDADD = $(top_builddir)/src/libCEGUIBase.la \
                      $(top_builddir)/Samples/common/src/libCEGUISampleHelper.la

install-exec-local:
	$(LN_S) -f `pwd`/.libs/FalagardDemo1$(EXEEXT) $(top_builddir)/Samples/bin/FalagardDemo1$(EXEEXT)
