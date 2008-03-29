INCLUDES=-I$(top_srcdir)/cegui/include \
         -I$(top_srcdir)/cegui/include/ImageCodecModules/SILLYImageCodec \
         $(SILLY_CFLAGS)

libCEGUISILLYImageCodec_la_LIBADD =

if MINGW32_BUILD
libCEGUISILLYImageCodec_la_CPPFLAGS = -DCEGUISILLYIMAGECODEC_EXPORTS
libCEGUISILLYImageCodec_la_LDFLAGS = -no-undefined -avoid-version
libCEGUISILLYImageCodec_la_LIBADD += -L$(top_builddir)/src -lCEGUIBase
endif

libCEGUISILLYImageCodec_la_LIBADD += $(SILLY_LIBS)

lib_LTLIBRARIES=libCEGUISILLYImageCodec.la

libCEGUISILLYImageCodec_la_SOURCES = CEGUISILLYImageCodec.cpp CEGUISILLYImageCodecModule.cpp 
