INCLUDES = -I$(top_srcdir)/cegui/include \
           -I$(top_srcdir)/cegui/include/RendererModules/Ogre \
            $(Ogre_CFLAGS)

lib_LTLIBRARIES=libCEGUIOgreRenderer@cegui_bsfx@.la
libCEGUIOgreRenderer@cegui_bsfx@_la_LDFLAGS = @CEGUI_LIB_LINK_FLAGS@
libCEGUIOgreRenderer@cegui_bsfx@_la_LIBADD = $(Ogre_LIBS) $(top_builddir)/cegui/src/libCEGUIBase@cegui_bsfx@.la

if CEGUI_BUILD_STATIC
libCEGUIOgreRenderer@cegui_bsfx@_la_CPPFLAGS = -DCEGUI_STATIC
else
if MINGW32_BUILD
libCEGUIOgreRenderer@cegui_bsfx@_la_CPPFLAGS = -DOGRE_GUIRENDERER_EXPORTS
endif
endif

libCEGUIOgreRenderer@cegui_bsfx@_la_SOURCES = CEGUIOgreGeometryBuffer.cpp \
                                  CEGUIOgreImageCodec.cpp \
                                  CEGUIOgreRenderer.cpp \
                                  CEGUIOgreRenderTarget.cpp \
                                  CEGUIOgreResourceProvider.cpp \
                                  CEGUIOgreTexture.cpp \
                                  CEGUIOgreTextureTarget.cpp \
                                  CEGUIOgreWindowTarget.cpp
