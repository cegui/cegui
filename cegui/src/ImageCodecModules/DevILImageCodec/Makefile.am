INCLUDES=-I$(top_srcdir)/include $(DevIL_CFLAGS)

pkgincludedir=$(prefix)/include/CEGUI/ImageCodecModules/DevILImageCodec

libCEGUIDevILImageCodec_la_LIBADD =

if MINGW32_BUILD
libCEGUIDevILImageCodec_la_CPPFLAGS = -DCEGUIDEVILIMAGECODEC_EXPORTS
libCEGUIDevILImageCodec_la_LDFLAGS = -no-undefined -avoid-version
libCEGUIDevILImageCodec_la_LIBADD += -L$(top_builddir)/src -lCEGUIBase
endif

libCEGUIDevILImageCodec_la_LIBADD += $(DevIL_LIBS) 

lib_LTLIBRARIES=libCEGUIDevILImageCodec.la

libCEGUIDevILImageCodec_la_SOURCES = CEGUIDevILImageCodec.cpp CEGUIDevILImageCodecModule.cpp

pkginclude_HEADERS = CEGUIDevILImageCodec.h CEGUIDevILImageCodecModule.h

