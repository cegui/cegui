INCLUDES=-I$(top_srcdir)/include $(SILLY_CFLAGS)
pkgincludedir=$(prefix)/include/CEGUI/ImageCodecModules/SILLYImageCodec

if MINGW32_BUILD
libCEGUISILLYImageCodec_la_CPPFLAGS = -DCEGUISILLYIMAGECODEC_EXPORTS
libCEGUISILLYImageCodec_la_LDFLAGS = -no-undefined -avoid-version
endif

libCEGUISILLYImageCodec_la_LIBADD = $(SILLY_LIBS) $(top_builddir)/src/libCEGUIBase.la

lib_LTLIBRARIES=libCEGUISILLYImageCodec.la

libCEGUISILLYImageCodec_la_SOURCES = CEGUISILLYImageCodec.cpp CEGUISILLYImageCodecModule.cpp 

pkginclude_HEADERS = CEGUISILLYImageCodec.h CEGUISILLYImageCodecModule.h

