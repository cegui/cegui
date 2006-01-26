INCLUDES = -I$(top_srcdir)/include $(Irrlicht_CFLAGS)
lib_LTLIBRARIES=libCEGUIIrrlichtRenderer.la

libCEGUIIrrlichtRenderer_la_LDFLAGS = -version-info @CEGUI_IRRLICHT_VERSION_INFO@

libCEGUIIrrlichtRenderer_la_SOURCES = irrlichtrenderer.cpp \
                                      IrrlichtResourceProvider.cpp \
                                      irrlichttexture.cpp

pkginclude_HEADERS = IrrlichtEventPusher.h

libCEGUIIrrlichtRenderer_la_LIBADD = $(Irrlicht_LIBS)
