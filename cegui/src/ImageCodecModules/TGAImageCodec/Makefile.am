INCLUDES=-I$(top_srcdir)/cegui/include \
         -I$(top_srcdir)/cegui/include/ImageCodecModules/TGAImageCodec

if MINGW32_BUILD
libCEGUITGAImageCodec_la_CPPFLAGS = -DCEGUITGAIMAGECODEC_EXPORTS
libCEGUITGAImageCodec_la_LDFLAGS = -no-undefined -avoid-version
else
libCEGUITGAImageCodec_la_LDFLAGS = -release @PACKAGE_VERSION@
endif

lib_LTLIBRARIES=libCEGUITGAImageCodec.la
libCEGUITGAImageCodec_la_LIBADD = $(top_builddir)/cegui/src/libCEGUIBase.la
libCEGUITGAImageCodec_la_SOURCES = CEGUITGAImageCodec.cpp CEGUITGAImageCodecModule.cpp
