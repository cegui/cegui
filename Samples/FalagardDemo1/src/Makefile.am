INCLUDES = -I$(top_srcdir)/Samples/FalagardDemo1/include -I$(top_srcdir)/include -I$(top_srcdir)/Samples/common/include

noinst_PROGRAMS = FalagardDemo1

FalagardDemo1_SOURCES = Sample_FalagardDemo1.cpp
FalagardDemo1_LDFLAGS = -L$(top_builddir)/src -L$(top_builddir)/Samples/common/src
FalagardDemo1_LDADD = -lCEGUIBase -lCEGUISampleHelper

install-exec-local:
	$(LN_S) -f `pwd`/.libs/FalagardDemo1 $(top_builddir)/Samples/bin/FalagardDemo1
