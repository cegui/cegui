INCLUDES = -I$(top_srcdir)/include -I$(top_srcdir) $(Irrlicht_CFLAGS)

pkgincludedir=$(prefix)/include/CEGUI/RendererModules/IrrlichtGUIRenderer

lib_LTLIBRARIES=libCEGUIIrrlichtRenderer.la
libCEGUIIrrlichtRenderer_la_LIBADD = $(Irrlicht_LIBS) $(top_builddir)/src/libCEGUIBase.la
libCEGUIIrrlichtRenderer_la_LDFLAGS = -version-info @CEGUI_IRRLICHT_VERSION_INFO@

if MINGW32_BUILD
libCEGUIIrrlichtRenderer_la_CPPFLAGS = -DIRRLICHT_GUIRENDERER_EXPORTS
libCEGUIIrrlichtRenderer_la_LDFLAGS += -no-undefined
endif

libCEGUIIrrlichtRenderer_la_SOURCES = CEGUIIrrlichtGeometryBuffer.cpp \
                                      CEGUIIrrlichtMemoryFile.cpp \
                                      CEGUIIrrlichtRenderer.cpp \
                                      CEGUIIrrlichtRenderTarget.cpp \
                                      CEGUIIrrlichtResourceProvider.cpp \
                                      CEGUIIrrlichtTexture.cpp \
                                      CEGUIIrrlichtTextureTarget.cpp \
                                      CEGUIIrrlichtWindowTarget.cpp

pkginclude_HEADERS = CEGUIIrrlichtEventPusher.h \
                     CEGUIIrrlichtGeometryBuffer.h \
                     CEGUIIrrlichtMemoryFile.h \
                     CEGUIIrrlichtRenderer.h \
                     CEGUIIrrlichtRendererDef.h \
                     CEGUIIrrlichtRenderTarget.h \
                     CEGUIIrrlichtResourceProvider.h \
                     CEGUIIrrlichtTexture.h \
                     CEGUIIrrlichtTextureTarget.h \
                     CEGUIIrrlichtWindowTarget.h

EXTRA_DIST = Readme.txt
