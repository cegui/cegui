INCLUDES =

if BUILD_USING_INTERNAL_GLEW
INCLUDES += -I$(top_srcdir)/cegui/src/RendererModules/OpenGL/GLEW
endif

INCLUDES += -I$(top_srcdir)/cegui/include \
            -I$(top_srcdir)/cegui/include/RendererModules/OpenGL \
            $(OpenGL_CFLAGS)

libCEGUIOpenGLRenderer@cegui_bsfx@_la_LDFLAGS =

if MINGW32_BUILD
libCEGUIOpenGLRenderer@cegui_bsfx@_la_CPPFLAGS = -DOPENGL_GUIRENDERER_EXPORTS
libCEGUIOpenGLRenderer@cegui_bsfx@_la_LDFLAGS += -no-undefined

if BUILD_USING_INTERNAL_GLEW
libCEGUIOpenGLRenderer@cegui_bsfx@_la_CPPFLAGS += -DGLEW_STATIC
endif

endif

libCEGUIOpenGLRenderer@cegui_bsfx@_la_LIBADD = $(OpenGL_LIBS) \
                                   $(top_builddir)/cegui/src/libCEGUIBase@cegui_bsfx@.la

libCEGUIOpenGLRenderer@cegui_bsfx@_la_LDFLAGS += @CEGUI_LIB_LINK_FLAGS@

lib_LTLIBRARIES=libCEGUIOpenGLRenderer@cegui_bsfx@.la
libCEGUIOpenGLRenderer@cegui_bsfx@_la_SOURCES = CEGUIOpenGLFBOTextureTarget.cpp \
                                    CEGUIOpenGLGLXPBTextureTarget.cpp \
                                    CEGUIOpenGLGeometryBuffer.cpp \
                                    CEGUIOpenGLRenderer.cpp \
                                    CEGUIOpenGLRenderTarget.cpp \
                                    CEGUIOpenGLTexture.cpp \
                                    CEGUIOpenGLViewportTarget.cpp

if BUILD_USING_INTERNAL_GLEW
libCEGUIOpenGLRenderer@cegui_bsfx@_la_SOURCES += GLEW/src/glew.c
endif

noinst_HEADERS= GLEW/GL/glew.h \
                GLEW/GL/glxew.h \
                GLEW/GL/wglew.h

EXTRA_DIST = GLEW/GLEW-LICENSE \
             CEGUIOpenGLApplePBTextureTarget.cpp \
             CEGUIOpenGLApplePBTextureTarget.h \
             CEGUIOpenGLWGLPBTextureTarget.cpp \
             CEGUIOpenGLWGLPBTextureTarget.h
