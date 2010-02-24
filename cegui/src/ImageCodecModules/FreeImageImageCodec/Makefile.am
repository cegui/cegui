INCLUDES=-I$(top_srcdir)/cegui/include \
         -I$(top_srcdir)/cegui/include/ImageCodecModules/FreeImageImageCodec \
         $(FreeImage_CFLAGS)

libCEGUIFreeImageImageCodec@cegui_bsfx@_la_LDFLAGS = @CEGUI_LIB_LINK_FLAGS@

if CEGUI_BUILD_STATIC
libCEGUIFreeImageImageCodec@cegui_bsfx@_la_CPPFLAGS = -DCEGUI_STATIC
else
if MINGW32_BUILD
libCEGUIFreeImageImageCodec@cegui_bsfx@_la_CPPFLAGS = -DCEGUIFREEIMAGEIMAGECODEC_EXPORTS
endif
endif

libCEGUIFreeImageImageCodec@cegui_bsfx@_la_LIBADD = $(FreeImage_LIBS) \
                                        $(top_builddir)/cegui/src/libCEGUIBase@cegui_bsfx@.la

lib_LTLIBRARIES=libCEGUIFreeImageImageCodec@cegui_bsfx@.la

libCEGUIFreeImageImageCodec@cegui_bsfx@_la_SOURCES = CEGUIFreeImageImageCodec.cpp CEGUIFreeImageImageCodecModule.cpp
