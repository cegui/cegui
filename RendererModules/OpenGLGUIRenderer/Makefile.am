INCLUDES =

if BUILD_USING_INTERNAL_GLEW
INCLUDES += -I$(top_srcdir)/RendererModules/OpenGLGUIRenderer/GLEW
endif

INCLUDES += -I$(top_srcdir)/include -I$(top_srcdir) $(OpenGL_CFLAGS)

pkgincludedir=$(prefix)/include/CEGUI/RendererModules/OpenGLGUIRenderer
pkginclude_HEADERS = CEGUIOpenGLRenderer.h CEGUIOpenGLTexture.h \
	CEGUIOpenGLGeometryBatch.h CEGUIOpenGLGeometryBuffer.h \
	CEGUIOpenGLRenderTarget.h CEGUIOpenGLViewportTarget.h \
	CEGUIOpenGLFBOTextureTarget.h CEGUIOpenGL.h

libCEGUIOpenGLRenderer_la_LDFLAGS =

if MINGW32_BUILD
libCEGUIOpenGLRenderer_la_CPPFLAGS = -DOPENGL_GUIRENDERER_EXPORTS
libCEGUIOpenGLRenderer_la_LDFLAGS += -no-undefined

if BUILD_USING_INTERNAL_GLEW
libCEGUIOpenGLRenderer_la_CPPFLAGS += -DGLEW_STATIC
endif

endif

libCEGUIOpenGLRenderer_la_LIBADD = $(OpenGL_LIBS) $(top_builddir)/src/libCEGUIBase.la

libCEGUIOpenGLRenderer_la_LDFLAGS += -version-info @CEGUI_OPENGL_VERSION_INFO@

lib_LTLIBRARIES=libCEGUIOpenGLRenderer.la
libCEGUIOpenGLRenderer_la_SOURCES = CEGUIOpenGLRenderer.cpp \
    CEGUIOpenGLTexture.cpp CEGUIOpenGLGeometryBatch.cpp \
    CEGUIOpenGLGeometryBuffer.cpp CEGUIOpenGLRenderTarget.cpp \
    CEGUIOpenGLViewportTarget.cpp CEGUIOpenGLFBOTextureTarget.cpp

if BUILD_USING_INTERNAL_GLEW
libCEGUIOpenGLRenderer_la_SOURCES += GLEW/src/glew.c
endif

noinst_HEADERS= GLEW/GL/glew.h \
                GLEW/GL/glxew.h \
                GLEW/GL/wglew.h

EXTRA_DIST = GLEW/GLEW-LICENSE
