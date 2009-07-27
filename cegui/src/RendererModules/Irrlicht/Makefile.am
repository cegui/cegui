INCLUDES = -I$(top_srcdir)/cegui/include \
           -I$(top_srcdir)/cegui/include/RendererModules/Irrlicht \
            $(Irrlicht_CFLAGS)

lib_LTLIBRARIES=libCEGUIIrrlichtRenderer.la
libCEGUIIrrlichtRenderer_la_LIBADD = $(Irrlicht_LIBS) $(top_builddir)/cegui/src/libCEGUIBase.la
libCEGUIIrrlichtRenderer_la_LDFLAGS = @CEGUI_LIB_LINK_FLAGS@

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
