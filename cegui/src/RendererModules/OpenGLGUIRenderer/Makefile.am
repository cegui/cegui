INCLUDES = -I$(top_srcdir)/cegui/include \
           -I$(top_srcdir)/cegui/include/RendererModules/OpenGLGUIRenderer \
           $(OpenGL_CFLAGS)

libCEGUIOpenGLRenderer_la_LDFLAGS =

if MINGW32_BUILD
libCEGUIOpenGLRenderer_la_CPPFLAGS = -DOPENGL_GUIRENDERER_EXPORTS
libCEGUIOpenGLRenderer_la_LDFLAGS += -no-undefined
endif

libCEGUIOpenGLRenderer_la_LIBADD = $(OpenGL_LIBS) \
                                   $(top_builddir)/cegui/src/libCEGUIBase.la

libCEGUIOpenGLRenderer_la_LDFLAGS += -version-info @CEGUI_OPENGL_VERSION_INFO@

lib_LTLIBRARIES=libCEGUIOpenGLRenderer.la
libCEGUIOpenGLRenderer_la_SOURCES = openglrenderer.cpp opengltexture.cpp



