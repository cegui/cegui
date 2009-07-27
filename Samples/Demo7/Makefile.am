INCLUDES = -I$(top_srcdir)/cegui/include -I$(top_srcdir)/Samples/common/include

noinst_PROGRAMS = Demo7@cegui_bsfx@

noinst_HEADERS =  Sample_Demo7.h

Demo7@cegui_bsfx@_SOURCES = Sample_Demo7.cpp
Demo7@cegui_bsfx@_LDADD = $(top_builddir)/cegui/src/libCEGUIBase@cegui_bsfx@.la \
              $(top_builddir)/Samples/common/src/libCEGUISampleHelper@cegui_bsfx@.la


install-exec-local:
	$(LN_S) -f `pwd`/.libs/Demo7@cegui_bsfx@$(EXEEXT) $(top_builddir)/bin/Demo7@cegui_bsfx@$(EXEEXT)
