INCLUDES=-I$(top_srcdir)/cegui/include \
         -I$(top_srcdir)/cegui/include/ImageCodecModules/FreeImageImageCodec \
         $(FreeImage_CFLAGS)

if MINGW32_BUILD
libCEGUIFreeImageImageCodec_la_CPPFLAGS = -DCEGUIFREEIMAGEIMAGECODEC_EXPORTS
libCEGUIFreeImageImageCodec_la_LDFLAGS = -no-undefined -avoid-version
else
libCEGUIFreeImageImageCodec_la_LDFLAGS = -release @PACKAGE_VERSION@
endif

libCEGUIFreeImageImageCodec_la_LIBADD = $(FreeImage_LIBS) \
                                        $(top_builddir)/cegui/src/libCEGUIBase.la

lib_LTLIBRARIES=libCEGUIFreeImageImageCodec.la

libCEGUIFreeImageImageCodec_la_SOURCES = CEGUIFreeImageImageCodec.cpp CEGUIFreeImageImageCodecModule.cpp
