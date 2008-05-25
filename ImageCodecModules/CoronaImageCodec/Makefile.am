INCLUDES=-I$(top_srcdir)/include $(Corona_CFLAGS)
pkgincludedir=$(prefix)/include/CEGUI/ImageCodecModules/CoronaImageCodec

if MINGW32_BUILD
libCEGUICoronaImageCodec_la_CPPFLAGS = -DCEGUICORONAIMAGECODEC_EXPORTS
libCEGUICoronaImageCodec_la_LDFLAGS = -no-undefined -avoid-version
endif

libCEGUICoronaImageCodec_la_LIBADD = $(Corona_LIBS) $(top_builddir)/src/libCEGUIBase.la

lib_LTLIBRARIES=libCEGUICoronaImageCodec.la

libCEGUICoronaImageCodec_la_SOURCES = CEGUICoronaImageCodec.cpp CEGUICoronaImageCodecModule.cpp

pkginclude_HEADERS = CEGUICoronaImageCodec.h CEGUICoronaImageCodecModule.h

