INCLUDES=-I$(top_srcdir)/cegui/include \
         -I$(top_srcdir)/cegui/include/ImageCodecModules/DevILImageCodec \
         $(DevIL_CFLAGS)

libCEGUIDevILImageCodec@cegui_bsfx@_la_LDFLAGS = @CEGUI_LIB_LINK_FLAGS@

if CEGUI_BUILD_STATIC
libCEGUIDevILImageCodec@cegui_bsfx@_la_CPPFLAGS = -DCEGUI_STATIC
else
if MINGW32_BUILD
libCEGUIDevILImageCodec@cegui_bsfx@_la_CPPFLAGS = -DCEGUIDEVILIMAGECODEC_EXPORTS
endif
endif

libCEGUIDevILImageCodec@cegui_bsfx@_la_LIBADD = $(DevIL_LIBS) \
                                    $(top_builddir)/cegui/src/libCEGUIBase@cegui_bsfx@.la

lib_LTLIBRARIES=libCEGUIDevILImageCodec@cegui_bsfx@.la

libCEGUIDevILImageCodec@cegui_bsfx@_la_SOURCES = CEGUIDevILImageCodec.cpp CEGUIDevILImageCodecModule.cpp
