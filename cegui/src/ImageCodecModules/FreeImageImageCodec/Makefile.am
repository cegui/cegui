INCLUDES=-I$(top_srcdir)/cegui/include \
         -I$(top_srcdir)/cegui/include/ImageCodecModules/FreeImageImageCodec \
         $(FreeImage_CFLAGS)

libCEGUIFreeImageImageCodec_la_LIBADD =

if MINGW32_BUILD
libCEGUIFreeImageImageCodec_la_CPPFLAGS = -DCEGUIFREEIMAGEIMAGECODEC_EXPORTS
libCEGUIFreeImageImageCodec_la_LDFLAGS = -no-undefined -avoid-version
libCEGUIFreeImageImageCodec_la_LIBADD += -L$(top_builddir)/src -lCEGUIBase
endif

libCEGUIFreeImageImageCodec_la_LIBADD += $(FreeImage_LIBS)

lib_LTLIBRARIES=libCEGUIFreeImageImageCodec.la

libCEGUIFreeImageImageCodec_la_SOURCES = CEGUIFreeImageImageCodec.cpp CEGUIFreeImageImageCodecModule.cpp
