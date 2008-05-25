INCLUDES = -I$(top_srcdir)/include $(Irrlicht_CFLAGS)

pkgincludedir=$(prefix)/include/CEGUI/RendererModules/IrrlichtRenderer
pkginclude_HEADERS = irrlichtrenderer.h \
					 IrrlichtEventPusher.h \
					 IrrlichtRendererDef.h \
					 IrrlichtResourceProvider.h \
					 irrlichttexture.h \
					 IrrlichtMemoryFile.h

libCEGUIIrrlichtRenderer_la_LDFLAGS =

if MINGW32_BUILD
libCEGUIIrrlichtRenderer_la_CPPFLAGS = -DIRRRENDERER_EXPORTS
libCEGUIIrrlichtRenderer_la_LDFLAGS += -no-undefined
endif

libCEGUIIrrlichtRenderer_la_LDFLAGS += -version-info @CEGUI_IRRLICHT_VERSION_INFO@
libCEGUIIrrlichtRenderer_la_LIBADD = $(Irrlicht_LIBS) $(top_builddir)/src/libCEGUIBase.la

lib_LTLIBRARIES=libCEGUIIrrlichtRenderer.la

libCEGUIIrrlichtRenderer_la_SOURCES = irrlichtrenderer.cpp \
                                      IrrlichtResourceProvider.cpp \
                                      irrlichttexture.cpp \
                                      IrrlichtMemoryFile.cpp

EXTRA_DIST = Readme.txt
