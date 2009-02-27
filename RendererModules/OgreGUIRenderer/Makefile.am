INCLUDES = -I$(top_srcdir)/include -I$(top_srcdir) $(Ogre_CFLAGS)

pkgincludedir=$(prefix)/include/CEGUI/RendererModules/OgreGUIRenderer

lib_LTLIBRARIES=libCEGUIOgreRenderer.la
libCEGUIOgreRenderer_la_LIBADD = $(Ogre_LIBS) $(top_builddir)/src/libCEGUIBase.la
libCEGUIOgreRenderer_la_LDFLAGS = -version-info @CEGUI_OGRE_VERSION_INFO@

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

pkginclude_HEADERS = CEGUIOgreGeometryBuffer.h \
                     CEGUIOgreRenderer.h \
                     CEGUIOgreRenderTarget.h \
					 CEGUIOgreResourceProvider.h \
                     CEGUIOgreTexture.h \
                     CEGUIOgreTextureTarget.h \
                     CEGUIOgreWindowTarget.h

