INCLUDES=-I$(top_srcdir)/include $(FreeImage_CFLAGS)
pkgincludedir=$(prefix)/include/CEGUI/ImageCodecModules/FreeImageImageCodec

lib_LTLIBRARIES=libCEGUIFreeImageImageCodec.la
libCEGUIFreeImageImageCodec_la_SOURCES = CEGUIFreeImageImageCodec.cpp CEGUIFreeImageImageCodecModule.cpp
libCEGUIFreeImageImageCodec_la_LIBADD = $(FreeImage_LIBS)
pkginclude_HEADERS = CEGUIFreeImageImageCodec.h CEGUIFreeImageImageCodecModule.h

