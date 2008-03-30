pkgincludedir=$(includedir)/CEGUI/RendererModules/OpenGLGUIRenderer

pkginclude_HEADERS = openglrenderer.h \
                     opengltexture.h \
                     CEGUIOpenGLFBOTextureTarget.h \
                     CEGUIOpenGLRenderTarget.h \
                     CEGUIOpenGLTextureTarget.h \
                     CEGUIOpenGLViewportTarget.h \
                     glext.h \
                     glxext.h \
                     wglext.h

if MINGW32_BUILD
pkginclude_HEADERS += CEGUIOpenGLWGLPBTextureTarget.h
else
pkginclude_HEADERS += CEGUIOpenGLGLXPBTextureTarget.h
endif
