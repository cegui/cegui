INCLUDES = -I$(top_srcdir)/cegui/include \
           -I$(top_srcdir)/cegui/include/RendererModules/directfbRenderer \
           $(directfb_CFLAGS)

libCEGUIdirectfbRenderer_la_LDFLAGS =

libCEGUIdirectfbRenderer_la_LDFLAGS += -release @PACKAGE_VERSION@
libCEGUIdirectfbRenderer_la_LIBADD = $(directfb_LIBS) \
                                     $(top_builddir)/cegui/src/libCEGUIBase.la

lib_LTLIBRARIES=libCEGUIdirectfbRenderer.la

libCEGUIdirectfbRenderer_la_SOURCES = directfb-texture.cpp \
                                      directfb-renderer.cpp
