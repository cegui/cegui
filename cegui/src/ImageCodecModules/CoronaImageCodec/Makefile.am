INCLUDES=-I$(top_srcdir)/cegui/include \
         -I$(top_srcdir)/cegui/include/ImageCodecModules/CoronaImageCodec \
         $(Corona_CFLAGS)

if MINGW32_BUILD
libCEGUICoronaImageCodec@cegui_bsfx@_la_CPPFLAGS = -DCEGUICORONAIMAGECODEC_EXPORTS
libCEGUICoronaImageCodec@cegui_bsfx@_la_LDFLAGS = -no-undefined -avoid-version
else
libCEGUICoronaImageCodec@cegui_bsfx@_la_LDFLAGS = @CEGUI_LIB_LINK_FLAGS@
endif

libCEGUICoronaImageCodec@cegui_bsfx@_la_LIBADD = $(Corona_LIBS) \
                                     $(top_builddir)/cegui/src/libCEGUIBase@cegui_bsfx@.la

lib_LTLIBRARIES=libCEGUICoronaImageCodec@cegui_bsfx@.la

libCEGUICoronaImageCodec@cegui_bsfx@_la_SOURCES = CEGUICoronaImageCodec.cpp CEGUICoronaImageCodecModule.cpp
