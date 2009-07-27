INCLUDES=-I$(top_srcdir)/cegui/include \
         -I$(top_srcdir)/cegui/include/ImageCodecModules/DevILImageCodec \
         $(DevIL_CFLAGS)

if MINGW32_BUILD
libCEGUIDevILImageCodec@cegui_bsfx@_la_CPPFLAGS = -DCEGUIDEVILIMAGECODEC_EXPORTS
libCEGUIDevILImageCodec@cegui_bsfx@_la_LDFLAGS = -no-undefined -avoid-version
else
libCEGUIDevILImageCodec@cegui_bsfx@_la_LDFLAGS = @CEGUI_LIB_LINK_FLAGS@
endif

libCEGUIDevILImageCodec@cegui_bsfx@_la_LIBADD = $(DevIL_LIBS) \
                                    $(top_builddir)/cegui/src/libCEGUIBase@cegui_bsfx@.la

lib_LTLIBRARIES=libCEGUIDevILImageCodec@cegui_bsfx@.la

libCEGUIDevILImageCodec@cegui_bsfx@_la_SOURCES = CEGUIDevILImageCodec.cpp CEGUIDevILImageCodecModule.cpp
