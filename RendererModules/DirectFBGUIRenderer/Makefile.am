INCLUDES = -I$(top_srcdir)/include \
           -I$(top_srcdir)/RendererModules/DirectFBGUIRenderer \
           $(directfb_CFLAGS)

pkgincludedir=$(includedir)/CEGUI/RendererModules/DirectFBGUIRenderer
pkginclude_HEADERS = CEGUIDirectFBGeometryBuffer.h \
                     CEGUIDirectFBRenderer.h \
                     CEGUIDirectFBRenderTarget.h \
                     CEGUIDirectFBTexture.h

libCEGUIDirectFBRenderer_la_LDFLAGS =

libCEGUIDirectFBRenderer_la_LDFLAGS += -version-info @CEGUI_DIRECTFB_VERSION_INFO@
libCEGUIDirectFBRenderer_la_LIBADD = $(directfb_LIBS) \
                                     $(top_builddir)/src/libCEGUIBase.la

lib_LTLIBRARIES=libCEGUIDirectFBRenderer.la

libCEGUIDirectFBRenderer_la_SOURCES = CEGUIDirectFBGeometryBuffer.cpp \
                                      CEGUIDirectFBRenderer.cpp \
                                      CEGUIDirectFBRenderTarget.cpp \
                                      CEGUIDirectFBTexture.cpp
