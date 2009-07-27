INCLUDES=-I$(top_srcdir)/cegui/include \
         -I$(top_srcdir)/cegui/include/ImageCodecModules/TGAImageCodec

if MINGW32_BUILD
libCEGUITGAImageCodec@cegui_bsfx@_la_CPPFLAGS = -DCEGUITGAIMAGECODEC_EXPORTS
libCEGUITGAImageCodec@cegui_bsfx@_la_LDFLAGS = -no-undefined -avoid-version
else
libCEGUITGAImageCodec@cegui_bsfx@_la_LDFLAGS = @CEGUI_LIB_LINK_FLAGS@
endif

lib_LTLIBRARIES=libCEGUITGAImageCodec@cegui_bsfx@.la
libCEGUITGAImageCodec@cegui_bsfx@_la_LIBADD = $(top_builddir)/cegui/src/libCEGUIBase@cegui_bsfx@.la
libCEGUITGAImageCodec@cegui_bsfx@_la_SOURCES = CEGUITGAImageCodec.cpp CEGUITGAImageCodecModule.cpp
