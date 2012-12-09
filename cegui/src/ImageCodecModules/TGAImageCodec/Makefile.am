INCLUDES=-I$(top_srcdir)/cegui/include \
         -I$(top_srcdir)/cegui/include/ImageCodecModules/TGAImageCodec

libCEGUITGAImageCodec@cegui_bsfx@_la_LDFLAGS = @CEGUI_LIB_LINK_FLAGS@

if CEGUI_BUILD_STATIC
libCEGUITGAImageCodec@cegui_bsfx@_la_CPPFLAGS = -DCEGUI_STATIC
else
if MINGW32_BUILD
libCEGUITGAImageCodec@cegui_bsfx@_la_CPPFLAGS = -DCEGUITGAIMAGECODEC_EXPORTS
endif
endif

lib_LTLIBRARIES=libCEGUITGAImageCodec@cegui_bsfx@.la
libCEGUITGAImageCodec@cegui_bsfx@_la_LIBADD = $(top_builddir)/cegui/src/libCEGUIBase@cegui_bsfx@.la
libCEGUITGAImageCodec@cegui_bsfx@_la_SOURCES = CEGUITGAImageCodec.cpp CEGUITGAImageCodecModule.cpp
