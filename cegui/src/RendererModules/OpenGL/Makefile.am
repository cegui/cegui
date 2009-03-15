INCLUDES =

if BUILD_USING_INTERNAL_GLEW
INCLUDES += -I$(top_srcdir)/cegui/src/RendererModules/OpenGL/GLEW
endif

INCLUDES += -I$(top_srcdir)/cegui/include \
            -I$(top_srcdir)/cegui/include/RendererModules/OpenGL \
            $(OpenGL_CFLAGS)

libCEGUIOpenGLRenderer_la_LDFLAGS =

if MINGW32_BUILD
libCEGUIOpenGLRenderer_la_CPPFLAGS = -DOPENGL_GUIRENDERER_EXPORTS
libCEGUIOpenGLRenderer_la_LDFLAGS += -no-undefined

if BUILD_USING_INTERNAL_GLEW
libCEGUIOpenGLRenderer_la_CPPFLAGS += -DGLEW_STATIC
endif

endif

libCEGUIOpenGLRenderer_la_LIBADD = $(OpenGL_LIBS) \
                                   $(top_builddir)/cegui/src/libCEGUIBase.la

libCEGUIOpenGLRenderer_la_LDFLAGS += -release @PACKAGE_VERSION@

lib_LTLIBRARIES=libCEGUIOpenGLRenderer.la
libCEGUIOpenGLRenderer_la_SOURCES = CEGUIOpenGLFBOTextureTarget.cpp \
                                    CEGUIOpenGLGLXPBTextureTarget.cpp \
                                    CEGUIOpenGLGeometryBuffer.cpp \
                                    CEGUIOpenGLRenderer.cpp \
                                    CEGUIOpenGLRenderTarget.cpp \
                                    CEGUIOpenGLTexture.cpp \
                                    CEGUIOpenGLViewportTarget.cpp

if BUILD_USING_INTERNAL_GLEW
libCEGUIOpenGLRenderer_la_SOURCES += GLEW/src/glew.c
endif

noinst_HEADERS= GLEW/GL/glew.h \
                GLEW/GL/glxew.h \
                GLEW/GL/wglew.h

EXTRA_DIST = GLEW/GLEW-LICENSE \
             CEGUIOpenGLApplePBTextureTarget.cpp \
             CEGUIOpenGLApplePBTextureTarget.h \
             CEGUIOpenGLWGLPBTextureTarget.cpp \
             CEGUIOpenGLWGLPBTextureTarget.h
