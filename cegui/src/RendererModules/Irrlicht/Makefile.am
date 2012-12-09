INCLUDES = -I$(top_srcdir)/cegui/include \
           -I$(top_srcdir)/cegui/include/RendererModules/Irrlicht \
            $(Irrlicht_CFLAGS)

lib_LTLIBRARIES=libCEGUIIrrlichtRenderer@cegui_bsfx@.la
libCEGUIIrrlichtRenderer@cegui_bsfx@_la_LIBADD = $(Irrlicht_LIBS) $(top_builddir)/cegui/src/libCEGUIBase@cegui_bsfx@.la
libCEGUIIrrlichtRenderer@cegui_bsfx@_la_LDFLAGS = @CEGUI_LIB_LINK_FLAGS@

if CEGUI_BUILD_STATIC
libCEGUIIrrlichtRenderer@cegui_bsfx@_la_CPPFLAGS = -DCEGUI_STATIC
else
if MINGW32_BUILD
libCEGUIIrrlichtRenderer@cegui_bsfx@_la_CPPFLAGS = -DIRRLICHT_GUIRENDERER_EXPORTS
endif
endif

libCEGUIIrrlichtRenderer@cegui_bsfx@_la_SOURCES = CEGUIIrrlichtGeometryBuffer.cpp \
                                      CEGUIIrrlichtImageCodec.cpp \
                                      CEGUIIrrlichtMemoryFile.cpp \
                                      CEGUIIrrlichtRenderer.cpp \
                                      CEGUIIrrlichtRenderTarget.cpp \
                                      CEGUIIrrlichtResourceProvider.cpp \
                                      CEGUIIrrlichtTexture.cpp \
                                      CEGUIIrrlichtTextureTarget.cpp \
                                      CEGUIIrrlichtWindowTarget.cpp
