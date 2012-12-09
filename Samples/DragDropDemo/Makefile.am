INCLUDES = -I$(top_srcdir)/cegui/include -I$(top_srcdir)/Samples/common/include

noinst_PROGRAMS = DragDropDemo@cegui_bsfx@

noinst_HEADERS =  Sample_DragDropDemo.h

if CEGUI_BUILD_STATIC
DragDropDemo@cegui_bsfx@_CPPFLAGS = -DCEGUI_STATIC
endif

DragDropDemo@cegui_bsfx@_SOURCES = Sample_DragDropDemo.cpp
DragDropDemo@cegui_bsfx@_LDADD = $(top_builddir)/cegui/src/libCEGUIBase@cegui_bsfx@.la \
                     $(top_builddir)/Samples/common/src/libCEGUISampleHelper@cegui_bsfx@.la \
					 $(SAMPLE_STATIC_LDFLAGS)


install-exec-local:
	$(LN_S) -f `pwd`/.libs/DragDropDemo@cegui_bsfx@$(EXEEXT) $(top_builddir)/bin/DragDropDemo@cegui_bsfx@$(EXEEXT)
