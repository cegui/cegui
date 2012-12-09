INCLUDES = -I$(top_srcdir)/cegui/include \
           -I$(top_srcdir)/cegui/include/RendererModules/Null \
            $(Null_CFLAGS)

lib_LTLIBRARIES=libCEGUINullRenderer@cegui_bsfx@.la
libCEGUINullRenderer@cegui_bsfx@_la_LDFLAGS = @CEGUI_LIB_LINK_FLAGS@
libCEGUINullRenderer@cegui_bsfx@_la_LIBADD = $(top_builddir)/cegui/src/libCEGUIBase@cegui_bsfx@.la

if CEGUI_BUILD_STATIC
libCEGUINullRenderer@cegui_bsfx@_la_CPPFLAGS = -DCEGUI_STATIC
else
if MINGW32_BUILD
libCEGUINullRenderer@cegui_bsfx@_la_CPPFLAGS = -DNULL_GUIRENDERER_EXPORTS
endif
endif

libCEGUINullRenderer@cegui_bsfx@_la_SOURCES = CEGUINullGeometryBuffer.cpp \
                                  CEGUINullRenderer.cpp \
                                  CEGUINullRenderTarget.cpp \
                                  CEGUINullTexture.cpp \
                                  CEGUINullTextureTarget.cpp 
