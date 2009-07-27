INCLUDES=-I$(top_srcdir)/cegui/include \
         -I$(top_srcdir)/cegui/include/ImageCodecModules/FreeImageImageCodec \
         $(FreeImage_CFLAGS)

if MINGW32_BUILD
libCEGUIFreeImageImageCodec@cegui_bsfx@_la_CPPFLAGS = -DCEGUIFREEIMAGEIMAGECODEC_EXPORTS
libCEGUIFreeImageImageCodec@cegui_bsfx@_la_LDFLAGS = -no-undefined -avoid-version
else
libCEGUIFreeImageImageCodec@cegui_bsfx@_la_LDFLAGS = @CEGUI_LIB_LINK_FLAGS@
endif

libCEGUIFreeImageImageCodec@cegui_bsfx@_la_LIBADD = $(FreeImage_LIBS) \
                                        $(top_builddir)/cegui/src/libCEGUIBase@cegui_bsfx@.la

lib_LTLIBRARIES=libCEGUIFreeImageImageCodec@cegui_bsfx@.la

libCEGUIFreeImageImageCodec@cegui_bsfx@_la_SOURCES = CEGUIFreeImageImageCodec.cpp CEGUIFreeImageImageCodecModule.cpp
