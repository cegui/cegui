INCLUDES = -I$(top_srcdir)/include -I$(top_srcdir) $(OpenGL_CFLAGS) 
pkgincludedir=$(prefix)/include/CEGUI/RendererModules/OpenGLGUIRenderer
pkginclude_HEADERS = openglrenderer.h opengltexture.h
lib_LTLIBRARIES=libCEGUIOpenGLRenderer.la
libCEGUIOpenGLRenderer_la_LDFLAGS = -version-info @CEGUI_OPENGL_VERSION_INFO@
libCEGUIOpenGLRenderer_la_SOURCES = openglrenderer.cpp opengltexture.cpp



