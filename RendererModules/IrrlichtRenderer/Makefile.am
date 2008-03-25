INCLUDES = -I$(top_srcdir)/include $(Irrlicht_CFLAGS)

pkgincludedir=$(prefix)/include/CEGUI/RendererModules/IrrlichtRenderer
pkginclude_HEADERS = irrlichtrenderer.h \
					 IrrlichtEventPusher.h \
					 IrrlichtRendererDef.h \
					 IrrlichtResourceProvider.h \
					 irrlichttexture.h \
					 IrrlichtMemoryFile.h

libCEGUIIrrlichtRenderer_la_LDFLAGS =
libCEGUIIrrlichtRenderer_la_LIBADD =

if MINGW32_BUILD
libCEGUIIrrlichtRenderer_la_CPPFLAGS = -DIRRRENDERER_EXPORTS
libCEGUIIrrlichtRenderer_la_LDFLAGS += -no-undefined
libCEGUIIrrlichtRenderer_la_LIBADD += -L$(top_builddir)/src -lCEGUIBase
endif

libCEGUIIrrlichtRenderer_la_LDFLAGS += -version-info @CEGUI_IRRLICHT_VERSION_INFO@
libCEGUIIrrlichtRenderer_la_LIBADD += $(Irrlicht_LIBS)

lib_LTLIBRARIES=libCEGUIIrrlichtRenderer.la

libCEGUIIrrlichtRenderer_la_SOURCES = irrlichtrenderer.cpp \
                                      IrrlichtResourceProvider.cpp \
                                      irrlichttexture.cpp \
                                      IrrlichtMemoryFile.cpp

EXTRA_DIST = Readme.txt
