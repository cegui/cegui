INCLUDES = -I$(top_srcdir)/include -I$(top_srcdir) $(OpenGL_CFLAGS) $(DevIL_CFLAGS) $(Corona_CFLAGS) $(SILLY_CFLAGS)
pkgincludedir=$(prefix)/include/CEGUI/renderers/OpenGLGUIRenderer
pkginclude_HEADERS = openglrenderer.h \
					 opengltexture.h

lib_LTLIBRARIES=libCEGUIOpenGLRenderer.la

libCEGUIOpenGLRenderer_la_LDFLAGS = -version-info @CEGUI_OPENGL_VERSION_INFO@

libCEGUIOpenGLRenderer_la_SOURCES = openglrenderer.cpp \
                                    opengltexture.cpp

libCEGUIOpenGLRenderer_la_LIBADD = \
	$(OpenGL_LIBS) \
	$(top_builddir)/ImageCodecModules/TGAImageCodec/libCEGUITGAImageCodec.la

if CEGUI_BUILD_CORONA_IMAGE_CODEC
libCEGUIOpenGLRenderer_la_LIBADD += \
	$(top_builddir)/ImageCodecModules/CoronaImageCodec/libCEGUICoronaImageCodec.la
endif

if CEGUI_BUILD_DEVIL_IMAGE_CODEC 
libCEGUIOpenGLRenderer_la_LIBADD += \
	$(top_builddir)/ImageCodecModules/DevILImageCodec/libCEGUIDevILImageCodec.la
endif 

if CEGUI_BUILD_SILLY_IMAGE_CODEC 
libCEGUIOpenGLRenderer_la_LIBADD += \
    $(top_builddir)/ImageCodecModules/SILLYImageCodec/libCEGUISILLYImageCodec.la
endif

