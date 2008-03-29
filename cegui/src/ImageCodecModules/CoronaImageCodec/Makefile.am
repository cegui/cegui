INCLUDES=-I$(top_srcdir)/cegui/include \
         -I$(top_srcdir)/cegui/include/ImageCodecModules/CoronamageCodec \
         $(Corona_CFLAGS)

libCEGUICoronaImageCodec_la_LIBADD =

if MINGW32_BUILD
libCEGUICoronaImageCodec_la_CPPFLAGS = -DCEGUICORONAIMAGECODEC_EXPORTS
libCEGUICoronaImageCodec_la_LDFLAGS = -no-undefined -avoid-version
libCEGUICoronaImageCodec_la_LIBADD += -L$(top_builddir)/src -lCEGUIBase
endif

libCEGUICoronaImageCodec_la_LIBADD += $(Corona_LIBS)

lib_LTLIBRARIES=libCEGUICoronaImageCodec.la

libCEGUICoronaImageCodec_la_SOURCES = CEGUICoronaImageCodec.cpp CEGUICoronaImageCodecModule.cpp
