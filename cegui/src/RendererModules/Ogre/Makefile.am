INCLUDES = -I$(top_srcdir)/cegui/include \
           -I$(top_srcdir)/cegui/include/RendererModules/Ogre \
            $(Ogre_CFLAGS)

lib_LTLIBRARIES=libCEGUIOgreRenderer.la
libCEGUIOgreRenderer_la_LDFLAGS = -release @PACKAGE_VERSION@
libCEGUIOgreRenderer_la_LIBADD = $(Ogre_LIBS) $(top_builddir)/cegui/src/libCEGUIBase.la

if MINGW32_BUILD
libCEGUIOgreRenderer_la_CPPFLAGS = -DOGRE_GUIRENDERER_EXPORTS
libCEGUIOgreRenderer_la_LDFLAGS += -no-undefined
endif

libCEGUIOgreRenderer_la_SOURCES = CEGUIOgreGeometryBuffer.cpp \
                                  CEGUIOgreRenderer.cpp \
                                  CEGUIOgreRenderTarget.cpp \
                                  CEGUIOgreResourceProvider.cpp \
                                  CEGUIOgreTexture.cpp \
                                  CEGUIOgreTextureTarget.cpp \
                                  CEGUIOgreWindowTarget.cpp
