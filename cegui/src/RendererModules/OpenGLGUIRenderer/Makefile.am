INCLUDES = -I$(top_srcdir)/cegui/include \
           -I$(top_srcdir)/cegui/include/RendererModules/OpenGLGUIRenderer \
           $(OpenGL_CFLAGS) 

lib_LTLIBRARIES=libCEGUIOpenGLRenderer.la

libCEGUIOpenGLRenderer_la_SOURCES = CEGUIOpenGLFBOTextureTarget.cpp \
                                    CEGUIOpenGLRenderTarget.cpp \
                                    CEGUIOpenGLTextureTarget.cpp \
                                    CEGUIOpenGLViewportTarget.cpp \
                                    openglrenderer.cpp \
                                    opengltexture.cpp

libCEGUIOpenGLRenderer_la_LDFLAGS =
libCEGUIOpenGLRenderer_la_LIBADD =

if MINGW32_BUILD
libCEGUIOpenGLRenderer_la_CPPFLAGS = -DOPENGL_GUIRENDERER_EXPORTS
libCEGUIOpenGLRenderer_la_LDFLAGS += -no-undefined
libCEGUIOpenGLRenderer_la_LIBADD += -L$(top_builddir)/src -lCEGUIBase
libCEGUIOpenGLRenderer_la_SOURCES += CEGUIOpenGLWGLPBTextureTarget.cpp
else
libCEGUIOpenGLRenderer_la_SOURCES += CEGUIOpenGLGLXPBTextureTarget.cpp
endif

libCEGUIOpenGLRenderer_la_LIBADD += $(OpenGL_LIBS)
libCEGUIOpenGLRenderer_la_LDFLAGS += -version-info @CEGUI_OPENGL_VERSION_INFO@
