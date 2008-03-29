INCLUDES=-I$(top_srcdir)/cegui/include \
         -I$(top_srcdir)/cegui/include/ImageCodecModules/TGAImageCodec

if MINGW32_BUILD
libCEGUITGAImageCodec_la_CPPFLAGS = -DCEGUITGAIMAGECODEC_EXPORTS
libCEGUITGAImageCodec_la_LDFLAGS = -no-undefined -avoid-version
libCEGUITGAImageCodec_la_LIBADD = -L$(top_builddir)/src -lCEGUIBase
endif

lib_LTLIBRARIES=libCEGUITGAImageCodec.la
libCEGUITGAImageCodec_la_SOURCES = CEGUITGAImageCodec.cpp CEGUITGAImageCodecModule.cpp
