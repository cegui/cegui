INCLUDES=-I$(top_srcdir)/include $(DevIL_CFLAGS)

pkgincludedir=$(prefix)/include/CEGUI/ImageCodecModules/DevILImageCodec

lib_LTLIBRARIES=libCEGUIDevILImageCodec.la
libCEGUIDevILImageCodec_la_SOURCES = CEGUIDevILImageCodec.cpp CEGUIDevILImageCodecModule.cpp
libCEGUIDevILImageCodec_la_LIBADD = $(DevIL_LIBS) 
pkginclude_HEADERS = CEGUIDevILImageCodec.h CEGUIDevILImageCodecModule.h

