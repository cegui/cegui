INCLUDES=-I$(top_srcdir)/cegui/include \
         -I$(top_srcdir)/cegui/include/ImageCodecModules/SILLYImageCodec \
         $(SILLY_CFLAGS)

if MINGW32_BUILD
libCEGUISILLYImageCodec_la_CPPFLAGS = -DCEGUISILLYIMAGECODEC_EXPORTS
libCEGUISILLYImageCodec_la_LDFLAGS = -no-undefined -avoid-version
else
libCEGUISILLYImageCodec_la_LDFLAGS = -release @PACKAGE_VERSION@
endif

libCEGUISILLYImageCodec_la_LIBADD = $(SILLY_LIBS) \
                                    $(top_builddir)/cegui/src/libCEGUIBase.la

lib_LTLIBRARIES=libCEGUISILLYImageCodec.la

libCEGUISILLYImageCodec_la_SOURCES = CEGUISILLYImageCodec.cpp CEGUISILLYImageCodecModule.cpp
