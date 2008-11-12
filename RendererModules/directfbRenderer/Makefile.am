INCLUDES = -I$(top_srcdir)/include \
           -I$(top_srcdir)/RendererModules/directfbRenderer \
           $(directfb_CFLAGS)

pkgincludedir=$(includedir)/CEGUI/RendererModules/directfbRenderer
pkginclude_HEADERS = directfb-texture.h \
                     directfb-renderer.h

libCEGUIdirectfbRenderer_la_LDFLAGS =

libCEGUIdirectfbRenderer_la_LDFLAGS += -version-info @CEGUI_DIRECTFB_VERSION_INFO@
libCEGUIdirectfbRenderer_la_LIBADD = $(directfb_LIBS) \
                                     $(top_builddir)/src/libCEGUIBase.la

lib_LTLIBRARIES=libCEGUIdirectfbRenderer.la

libCEGUIdirectfbRenderer_la_SOURCES = directfb-texture.cpp \
                                      directfb-renderer.cpp
