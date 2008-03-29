INCLUDES=-I$(top_srcdir)/cegui/include \
         -I$(top_srcdir)/cegui/include/ImageCodecModules/DevILImageCodec \
         $(DevIL_CFLAGS)

libCEGUIDevILImageCodec_la_LIBADD =

if MINGW32_BUILD
libCEGUIDevILImageCodec_la_CPPFLAGS = -DCEGUIDEVILIMAGECODEC_EXPORTS
libCEGUIDevILImageCodec_la_LDFLAGS = -no-undefined -avoid-version
libCEGUIDevILImageCodec_la_LIBADD += -L$(top_builddir)/src -lCEGUIBase
endif

libCEGUIDevILImageCodec_la_LIBADD += $(DevIL_LIBS) 

lib_LTLIBRARIES=libCEGUIDevILImageCodec.la

libCEGUIDevILImageCodec_la_SOURCES = CEGUIDevILImageCodec.cpp CEGUIDevILImageCodecModule.cpp
