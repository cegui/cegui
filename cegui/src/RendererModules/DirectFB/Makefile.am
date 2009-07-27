INCLUDES = -I$(top_srcdir)/cegui/include \
           -I$(top_srcdir)/cegui/include/RendererModules/DirectFB \
            $(directfb_CFLAGS)

libCEGUIDirectFBRenderer@cegui_bsfx@_la_LDFLAGS = @CEGUI_LIB_LINK_FLAGS@
libCEGUIDirectFBRenderer@cegui_bsfx@_la_LIBADD = $(directfb_LIBS) \
                                     $(top_builddir)/cegui/src/libCEGUIBase@cegui_bsfx@.la

lib_LTLIBRARIES=libCEGUIDirectFBRenderer@cegui_bsfx@.la

libCEGUIDirectFBRenderer@cegui_bsfx@_la_SOURCES = CEGUIDirectFBGeometryBuffer.cpp \
                                      CEGUIDirectFBRenderer.cpp \
                                      CEGUIDirectFBRenderTarget.cpp \
                                      CEGUIDirectFBTexture.cpp
