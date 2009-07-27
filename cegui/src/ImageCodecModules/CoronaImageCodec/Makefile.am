INCLUDES=-I$(top_srcdir)/cegui/include \
         -I$(top_srcdir)/cegui/include/ImageCodecModules/CoronaImageCodec \
         $(Corona_CFLAGS)

if MINGW32_BUILD
libCEGUICoronaImageCodec_la_CPPFLAGS = -DCEGUICORONAIMAGECODEC_EXPORTS
libCEGUICoronaImageCodec_la_LDFLAGS = -no-undefined -avoid-version
else
libCEGUICoronaImageCodec_la_LDFLAGS = @CEGUI_LIB_LINK_FLAGS@
endif

libCEGUICoronaImageCodec_la_LIBADD = $(Corona_LIBS) \
                                     $(top_builddir)/cegui/src/libCEGUIBase.la

lib_LTLIBRARIES=libCEGUICoronaImageCodec.la

libCEGUICoronaImageCodec_la_SOURCES = CEGUICoronaImageCodec.cpp CEGUICoronaImageCodecModule.cpp
