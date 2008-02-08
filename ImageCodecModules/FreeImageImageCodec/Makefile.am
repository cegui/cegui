INCLUDES=-I$(top_srcdir)/include $(FreeImage_CFLAGS)
pkgincludedir=$(prefix)/include/CEGUI/ImageCodecModules/FreeImageImageCodec

libCEGUIFreeImageImageCodec_la_LIBADD =

if MINGW32_BUILD
libCEGUIFreeImageImageCodec_la_CPPFLAGS = -DCEGUIFREEIMAGEIMAGECODEC_EXPORTS
libCEGUIFreeImageImageCodec_la_LDFLAGS = -no-undefined -avoid-version
libCEGUIFreeImageImageCodec_la_LIBADD += -L$(top_builddir)/src -lCEGUIBase
endif

libCEGUIFreeImageImageCodec_la_LIBADD += $(FreeImage_LIBS)

lib_LTLIBRARIES=libCEGUIFreeImageImageCodec.la

libCEGUIFreeImageImageCodec_la_SOURCES = CEGUIFreeImageImageCodec.cpp CEGUIFreeImageImageCodecModule.cpp

pkginclude_HEADERS = CEGUIFreeImageImageCodec.h CEGUIFreeImageImageCodecModule.h

