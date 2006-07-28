INCLUDES=-I$(top_srcdir)/include $(Corona_CFLAGS)
pkgincludedir=$(prefix)/include/CEGUI/ImageCodecModules/CoronaImageCodec

lib_LTLIBRARIES=libCEGUICoronaImageCodec.la
libCEGUICoronaImageCodec_la_SOURCES = CEGUICoronaImageCodec.cpp CEGUICoronaImageCodecModule.cpp
libCEGUICoronaImageCodec_la_LIBADD = $(Corona_LIBS)
pkginclude_HEADERS = CEGUICoronaImageCodec.h CEGUICoronaImageCodecModule.h

