INCLUDES = -I$(top_srcdir)/include -I$(top_srcdir)/Samples/common/include

noinst_PROGRAMS = DragDropDemo

noinst_HEADERS =  Sample_DragDropDemo.h

DragDropDemo_SOURCES = Sample_DragDropDemo.cpp
DragDropDemo_LDFLAGS = -L$(top_builddir)/src -L$(top_builddir)/Samples/common/src
DragDropDemo_LDADD = -lCEGUIBase -lCEGUISampleHelper


install-exec-local:
	$(LN_S) -f `pwd`/.libs/DragDropDemo$(EXEEXT) $(top_builddir)/Samples/bin/DragDropDemo$(EXEEXT)
