INCLUDES=-I$(top_srcdir)/include $(DevIL_CFLAGS)

pkgincludedir=$(prefix)/include/CEGUI/ImageCodecModules/DevILImageCodec

if MINGW32_BUILD
libCEGUIDevILImageCodec_la_CPPFLAGS = -DCEGUIDEVILIMAGECODEC_EXPORTS
libCEGUIDevILImageCodec_la_LDFLAGS = -no-undefined -avoid-version
endif

libCEGUIDevILImageCodec_la_LIBADD = $(DevIL_LIBS) $(top_builddir)/src/libCEGUIBase.la

lib_LTLIBRARIES=libCEGUIDevILImageCodec.la

libCEGUIDevILImageCodec_la_SOURCES = CEGUIDevILImageCodec.cpp CEGUIDevILImageCodecModule.cpp

pkginclude_HEADERS = CEGUIDevILImageCodec.h CEGUIDevILImageCodecModule.h

