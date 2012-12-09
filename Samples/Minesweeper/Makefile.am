INCLUDES = -I$(top_srcdir)/cegui/include -I$(top_srcdir)/Samples/common/include

noinst_PROGRAMS = Minesweeper@cegui_bsfx@
noinst_HEADERS=Minesweeper_Timer.h

if CEGUI_BUILD_STATIC
Minesweeper@cegui_bsfx@_CPPFLAGS = -DCEGUI_STATIC
endif

Minesweeper@cegui_bsfx@_SOURCES = Sample_Minesweeper.cpp Minesweeper_Timer.cpp
Minesweeper@cegui_bsfx@_LDADD = $(top_builddir)/cegui/src/libCEGUIBase@cegui_bsfx@.la \
                    $(top_builddir)/Samples/common/src/libCEGUISampleHelper@cegui_bsfx@.la \
					$(SAMPLE_STATIC_LDFLAGS)

install-exec-local:
	$(LN_S) -f `pwd`/.libs/Minesweeper@cegui_bsfx@$(EXEEXT) $(top_builddir)/bin/Minesweeper@cegui_bsfx@$(EXEEXT)
