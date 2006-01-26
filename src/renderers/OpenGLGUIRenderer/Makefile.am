INCLUDES = -I$(top_srcdir)/include $(OpenGL_CFLAGS) $(DevIL_CFLAGS)
lib_LTLIBRARIES=libCEGUIOpenGLRenderer.la

libCEGUIOpenGLRenderer_la_LDFLAGS = -version-info @CEGUI_OPENGL_VERSION_INFO@

libCEGUIOpenGLRenderer_la_SOURCES = openglrenderer.cpp \
                                    opengltexture.cpp

libCEGUIOpenGLRenderer_la_LIBADD = $(OpenGL_LIBS)
