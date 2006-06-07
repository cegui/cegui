INCLUDES=-I$(top_srcdir)/include $(DevIL_CFLAGS)

pkgincludedir=$(prefix)/include/CEGUI/ImageCodecModules/DevILImageCodec

noinst_LTLIBRARIES=libCEGUIDevILImageCodec.la
libCEGUIDevILImageCodec_la_SOURCES = CEGUIDevILImageCodec.cpp
libCEGUIDevILImageCodec_la_LIBADD = $(DevIL_LIBS) 
noinst_HEADERS = CEGUIDevILImageCodec.h

