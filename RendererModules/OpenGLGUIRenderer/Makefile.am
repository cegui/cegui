INCLUDES = -I$(top_srcdir)/include -I$(top_srcdir) $(OpenGL_CFLAGS) 
pkgincludedir=$(prefix)/include/CEGUI/RendererModules/OpenGLGUIRenderer
pkginclude_HEADERS = openglrenderer.h opengltexture.h

libCEGUIOpenGLRenderer_la_LDFLAGS =
libCEGUIOpenGLRenderer_la_LIBADD =

if MINGW32_BUILD
libCEGUIOpenGLRenderer_la_CPPFLAGS = -DOPENGL_GUIRENDERER_EXPORTS
libCEGUIOpenGLRenderer_la_LDFLAGS += -no-undefined
libCEGUIOpenGLRenderer_la_LIBADD += -L$(top_builddir)/src -lCEGUIBase
endif

libCEGUIOpenGLRenderer_la_LIBADD += $(OpenGL_LIBS)

libCEGUIOpenGLRenderer_la_LDFLAGS += -version-info @CEGUI_OPENGL_VERSION_INFO@

lib_LTLIBRARIES=libCEGUIOpenGLRenderer.la
libCEGUIOpenGLRenderer_la_SOURCES = openglrenderer.cpp opengltexture.cpp



