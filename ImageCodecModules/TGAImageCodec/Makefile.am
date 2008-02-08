INCLUDES=-I$(top_srcdir)/include 
pkgincludedir=$(prefix)/include/CEGUI/ImageCodecModules/TGAImageCodec

if MINGW32_BUILD
libCEGUITGAImageCodec_la_CPPFLAGS = -DCEGUITGAIMAGECODEC_EXPORTS
libCEGUITGAImageCodec_la_LDFLAGS = -no-undefined -avoid-version
libCEGUITGAImageCodec_la_LIBADD = -L$(top_builddir)/src -lCEGUIBase
endif

lib_LTLIBRARIES=libCEGUITGAImageCodec.la
libCEGUITGAImageCodec_la_SOURCES = CEGUITGAImageCodec.cpp CEGUITGAImageCodecModule.cpp
pkginclude_HEADERS = CEGUITGAImageCodec.h CEGUITGAImageCodecModule.h 



