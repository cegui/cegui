INCLUDES = -I$(top_srcdir)/cegui/include \
           -I$(top_srcdir)/cegui/include/RendererModules/OgreGUIRenderer \
           $(OGRE_CFLAGS)

lib_LTLIBRARIES = libCEGUIOgreRenderer.la

libCEGUIOgreRenderer_la_LIBADD  = $(OGRE_LIBS)
libCEGUIOgreRenderer_la_LDFLAGS =

libCEGUIOgreRenderer_la_SOURCES = OgreCEGUIRenderer.cpp \
                                  OgreCEGUITexture.cpp \
                                  OgreCEGUIResourceProvider.cpp \
                                  OgreCEGUIRenderQueueListener.cpp \
                                  OgreCEGUIRenderTarget.cpp \
                                  OgreCEGUIOgreRenderTargetTarget.cpp \
                                  OgreCEGUITextureTarget.cpp
