INCLUDES=-I$(top_srcdir)/include 
pkgincludedir=$(prefix)/include/CEGUI/ImageCodecModules/TGAImageCodec

if MINGW32_BUILD
libCEGUITGAImageCodec_la_CPPFLAGS = -DCEGUITGAIMAGECODEC_EXPORTS
libCEGUITGAImageCodec_la_LDFLAGS = -no-undefined -avoid-version
endif

lib_LTLIBRARIES=libCEGUITGAImageCodec.la
libCEGUITGAImageCodec_la_SOURCES = CEGUITGAImageCodec.cpp CEGUITGAImageCodecModule.cpp
libCEGUITGAImageCodec_la_LIBADD = $(top_builddir)/src/libCEGUIBase.la
pkginclude_HEADERS = CEGUITGAImageCodec.h CEGUITGAImageCodecModule.h 



