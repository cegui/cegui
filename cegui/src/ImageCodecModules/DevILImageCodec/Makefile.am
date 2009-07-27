INCLUDES=-I$(top_srcdir)/cegui/include \
         -I$(top_srcdir)/cegui/include/ImageCodecModules/DevILImageCodec \
         $(DevIL_CFLAGS)

if MINGW32_BUILD
libCEGUIDevILImageCodec_la_CPPFLAGS = -DCEGUIDEVILIMAGECODEC_EXPORTS
libCEGUIDevILImageCodec_la_LDFLAGS = -no-undefined -avoid-version
else
libCEGUIDevILImageCodec_la_LDFLAGS = @CEGUI_LIB_LINK_FLAGS@
endif

libCEGUIDevILImageCodec_la_LIBADD = $(DevIL_LIBS) \
                                    $(top_builddir)/cegui/src/libCEGUIBase.la

lib_LTLIBRARIES=libCEGUIDevILImageCodec.la

libCEGUIDevILImageCodec_la_SOURCES = CEGUIDevILImageCodec.cpp CEGUIDevILImageCodecModule.cpp
