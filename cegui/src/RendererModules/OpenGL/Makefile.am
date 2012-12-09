INCLUDES =

if BUILD_USING_INTERNAL_GLEW
INCLUDES += -I$(top_srcdir)/cegui/src/RendererModules/OpenGL/GLEW
endif

INCLUDES += -I$(top_srcdir)/cegui/include \
            -I$(top_srcdir)/cegui/include/RendererModules/OpenGL \
            $(OpenGL_CFLAGS)

libCEGUIOpenGLRenderer@cegui_bsfx@_la_LDFLAGS = @CEGUI_LIB_LINK_FLAGS@

if CEGUI_BUILD_STATIC
libCEGUIOpenGLRenderer@cegui_bsfx@_la_CPPFLAGS = -DCEGUI_STATIC
endif

if MINGW32_BUILD
if !CEGUI_BUILD_STATIC
libCEGUIOpenGLRenderer@cegui_bsfx@_la_CPPFLAGS = -DOPENGL_GUIRENDERER_EXPORTS
endif

if BUILD_USING_INTERNAL_GLEW
libCEGUIOpenGLRenderer@cegui_bsfx@_la_CPPFLAGS += -DGLEW_STATIC
endif

EXTRAPB_SOURCES = CEGUIOpenGLWGLPBTextureTarget.cpp
else

if MACOSX_BUILD
EXTRAPB_SOURCES = CEGUIOpenGLApplePBTextureTarget.cpp

else
EXTRAPB_SOURCES = CEGUIOpenGLGLXPBTextureTarget.cpp
endif
endif

libCEGUIOpenGLRenderer@cegui_bsfx@_la_LIBADD = $(OpenGL_LIBS) \
                                   $(top_builddir)/cegui/src/libCEGUIBase@cegui_bsfx@.la

lib_LTLIBRARIES=libCEGUIOpenGLRenderer@cegui_bsfx@.la
libCEGUIOpenGLRenderer@cegui_bsfx@_la_SOURCES = CEGUIOpenGLFBOTextureTarget.cpp \
                                    $(EXTRAPB_SOURCES) \
                                    CEGUIOpenGLGeometryBuffer.cpp \
                                    CEGUIOpenGLRenderer.cpp \
                                    CEGUIOpenGLRenderTarget.cpp \
                                    CEGUIOpenGLTexture.cpp \
                                    CEGUIOpenGLTextureTarget.cpp \
                                    CEGUIOpenGLViewportTarget.cpp

if BUILD_USING_INTERNAL_GLEW
libCEGUIOpenGLRenderer@cegui_bsfx@_la_SOURCES += GLEW/src/glew.c
endif

noinst_HEADERS= GLEW/GL/glew.h \
                GLEW/GL/glxew.h \
                GLEW/GL/wglew.h
