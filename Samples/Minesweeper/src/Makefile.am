INCLUDES = -I$(top_srcdir)/Samples/Minesweeper/include -I$(top_srcdir)/include -I$(top_srcdir)/Samples/common/include

noinst_PROGRAMS = Minesweeper

Minesweeper_SOURCES = Sample_Minesweeper.cpp
Minesweeper_LDFLAGS = -L$(top_builddir)/src -L$(top_builddir)/Samples/common/src
Minesweeper_LDADD = -lCEGUIBase -lCEGUISampleHelper

install-exec-local:
	$(LN_S) -f `pwd`/.libs/Minesweeper $(top_builddir)/Samples/bin/Minesweeper
