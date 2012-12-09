INCLUDES=-I$(top_srcdir)/cegui/include \
         -I$(top_srcdir)/cegui/include/ImageCodecModules/SILLYImageCodec \
         $(SILLY_CFLAGS)

libCEGUISILLYImageCodec@cegui_bsfx@_la_LDFLAGS = @CEGUI_LIB_LINK_FLAGS@

if CEGUI_BUILD_STATIC
libCEGUISILLYImageCodec@cegui_bsfx@_la_CPPFLAGS = -DCEGUI_STATIC
else
if MINGW32_BUILD
libCEGUISILLYImageCodec@cegui_bsfx@_la_CPPFLAGS = -DCEGUISILLYIMAGECODEC_EXPORTS
endif
endif

libCEGUISILLYImageCodec@cegui_bsfx@_la_LIBADD = $(SILLY_LIBS) \
                                    $(top_builddir)/cegui/src/libCEGUIBase@cegui_bsfx@.la

lib_LTLIBRARIES=libCEGUISILLYImageCodec@cegui_bsfx@.la

libCEGUISILLYImageCodec@cegui_bsfx@_la_SOURCES = CEGUISILLYImageCodec.cpp CEGUISILLYImageCodecModule.cpp
