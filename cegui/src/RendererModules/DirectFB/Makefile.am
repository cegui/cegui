INCLUDES = -I$(top_srcdir)/cegui/include \
           -I$(top_srcdir)/cegui/include/RendererModules/DirectFB \
            $(directfb_CFLAGS)

libCEGUIDirectFBRenderer_la_LDFLAGS = @CEGUI_LIB_LINK_FLAGS@
libCEGUIDirectFBRenderer_la_LIBADD = $(directfb_LIBS) \
                                     $(top_builddir)/cegui/src/libCEGUIBase.la

lib_LTLIBRARIES=libCEGUIDirectFBRenderer.la

libCEGUIDirectFBRenderer_la_SOURCES = CEGUIDirectFBGeometryBuffer.cpp \
                                      CEGUIDirectFBRenderer.cpp \
                                      CEGUIDirectFBRenderTarget.cpp \
                                      CEGUIDirectFBTexture.cpp
