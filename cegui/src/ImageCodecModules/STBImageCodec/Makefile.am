INCLUDES=-I$(top_srcdir)/cegui/include \
         -I$(top_srcdir)/cegui/include/ImageCodecModules/STBImageCodec

libCEGUISTBImageCodec@cegui_bsfx@_la_LDFLAGS = @CEGUI_LIB_LINK_FLAGS@

if MINGW32_BUILD
libCEGUISTBImageCodec@cegui_bsfx@_la_CPPFLAGS = -DCEGUISTBIMAGECODEC_EXPORTS
endif

libCEGUISTBImageCodec@cegui_bsfx@_la_LIBADD = $(top_builddir)/cegui/src/libCEGUIBase@cegui_bsfx@.la

lib_LTLIBRARIES=libCEGUISTBImageCodec@cegui_bsfx@.la

libCEGUISTBImageCodec@cegui_bsfx@_la_SOURCES = CEGUISTBImageCodec.cpp CEGUISTBImageCodecModule.cpp stb_image.cpp
