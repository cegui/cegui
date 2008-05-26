INCLUDES = -I$(top_srcdir)/include -I$(top_srcdir)/Samples/common/include

noinst_PROGRAMS = DragDropDemo

noinst_HEADERS =  Sample_DragDropDemo.h

DragDropDemo_SOURCES = Sample_DragDropDemo.cpp
DragDropDemo_LDADD = $(top_builddir)/src/libCEGUIBase.la \
                     $(top_builddir)/Samples/common/src/libCEGUISampleHelper.la


install-exec-local:
	$(LN_S) -f `pwd`/.libs/DragDropDemo$(EXEEXT) $(top_builddir)/Samples/bin/DragDropDemo$(EXEEXT)
