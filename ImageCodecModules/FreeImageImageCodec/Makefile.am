INCLUDES=-I$(top_srcdir)/include $(FreeImage_CFLAGS)
pkgincludedir=$(prefix)/include/CEGUI/ImageCodecModules/FreeImageImageCodec

if MINGW32_BUILD
libCEGUIFreeImageImageCodec_la_CPPFLAGS = -DCEGUIFREEIMAGEIMAGECODEC_EXPORTS
libCEGUIFreeImageImageCodec_la_LDFLAGS = -no-undefined -avoid-version
endif

libCEGUIFreeImageImageCodec_la_LIBADD = $(FreeImage_LIBS) $(top_builddir)/src/libCEGUIBase.la

lib_LTLIBRARIES=libCEGUIFreeImageImageCodec.la

libCEGUIFreeImageImageCodec_la_SOURCES = CEGUIFreeImageImageCodec.cpp CEGUIFreeImageImageCodecModule.cpp

pkginclude_HEADERS = CEGUIFreeImageImageCodec.h CEGUIFreeImageImageCodecModule.h

