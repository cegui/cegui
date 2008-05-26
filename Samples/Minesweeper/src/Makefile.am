INCLUDES = -I$(top_srcdir)/Samples/Minesweeper/include -I$(top_srcdir)/include -I$(top_srcdir)/Samples/common/include

noinst_PROGRAMS = Minesweeper
noinst_HEADERS=Minesweeper_Timer.h

Minesweeper_SOURCES = Sample_Minesweeper.cpp Minesweeper_Timer.cpp
Minesweeper_LDADD = $(top_builddir)/src/libCEGUIBase.la \
                    $(top_builddir)/Samples/common/src/libCEGUISampleHelper.la

install-exec-local:
	$(LN_S) -f `pwd`/.libs/Minesweeper$(EXEEXT) $(top_builddir)/Samples/bin/Minesweeper$(EXEEXT)
