INCLUDES = -I$(top_srcdir)/cegui/include -I$(top_srcdir)/Samples/common/include

noinst_PROGRAMS = FalagardDemo1
noinst_HEADERS =  Sample_FalagardDemo1.h
FalagardDemo1_SOURCES = Sample_FalagardDemo1.cpp
FalagardDemo1_LDADD = $(top_builddir)/cegui/src/libCEGUIBase.la \
                      $(top_builddir)/Samples/common/src/libCEGUISampleHelper.la

install-exec-local:
	$(LN_S) -f `pwd`/.libs/FalagardDemo1$(EXEEXT) $(top_builddir)/bin/FalagardDemo1$(EXEEXT)
