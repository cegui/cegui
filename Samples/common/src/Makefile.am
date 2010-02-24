INCLUDES = -I$(top_srcdir)/Samples/common/include \
           -I$(top_srcdir)/cegui/include \
           $(CEGUI_SAMPLES_CFLAGS)

lib_LTLIBRARIES=libCEGUISampleHelper@cegui_bsfx@.la

libCEGUISampleHelper@cegui_bsfx@_la_LDFLAGS = @CEGUI_LIB_LINK_FLAGS@

libCEGUISampleHelper@cegui_bsfx@_la_SOURCES = CEGuiBaseApplication.cpp \
                                  CEGuiRendererSelector.cpp \
                                  CEGuiSample.cpp \
                                  CLICEGuiRendererSelector.cpp

libCEGUISampleHelper@cegui_bsfx@_la_LIBADD = $(top_builddir)/cegui/src/libCEGUIBase@cegui_bsfx@.la

if CEGUI_BUILD_STATIC
libCEGUISampleHelper@cegui_bsfx@_la_CPPFLAGS = -DCEGUI_STATIC
else
if MINGW32_BUILD
libCEGUISampleHelper@cegui_bsfx@_la_CPPFLAGS = -DCEGUISAMPLE_EXPORTS
endif
endif

if MINGW32_BUILD
libCEGUISampleHelper@cegui_bsfx@_la_SOURCES  += Win32AppHelper.cpp Win32CEGuiRendererSelector.cpp
endif

if CEGUI_USING_GTK2
    INCLUDES += $(GTK_CFLAGS)
    libCEGUISampleHelper@cegui_bsfx@_la_LIBADD += $(GTK_LIBS)
    libCEGUISampleHelper@cegui_bsfx@_la_SOURCES += GTK2CEGuiRendererSelector.cpp
endif
if CEGUI_SAMPLES_USE_OPENGL
    libCEGUISampleHelper@cegui_bsfx@_la_SOURCES += CEGuiOpenGLBaseApplication.cpp
    libCEGUISampleHelper@cegui_bsfx@_la_LIBADD += $(top_builddir)/cegui/src/RendererModules/OpenGL/libCEGUIOpenGLRenderer@cegui_bsfx@.la \
                                      $(OpenGL_LIBS)
endif
if CEGUI_SAMPLES_USE_OGRE
    INCLUDES += $(Ogre_CFLAGS) $(OIS_CFLAGS)
    libCEGUISampleHelper@cegui_bsfx@_la_SOURCES += CEGuiOgreBaseApplication.cpp
    libCEGUISampleHelper@cegui_bsfx@_la_LIBADD += $(top_builddir)/cegui/src/RendererModules/Ogre/libCEGUIOgreRenderer@cegui_bsfx@.la \
                                      $(Ogre_LIBS) $(OIS_LIBS)
endif
if CEGUI_SAMPLES_USE_IRRLICHT
    INCLUDES += $(Irrlicht_CFLAGS)
    libCEGUISampleHelper@cegui_bsfx@_la_SOURCES += CEGuiIrrlichtBaseApplication.cpp
    libCEGUISampleHelper@cegui_bsfx@_la_LIBADD += $(top_builddir)/cegui/src/RendererModules/Irrlicht/libCEGUIIrrlichtRenderer@cegui_bsfx@.la \
                                      $(Irrlicht_LIBS)
endif
if CEGUI_SAMPLES_USE_DIRECTFB
    INCLUDES += $(directfb_CFLAGS)
    libCEGUISampleHelper@cegui_bsfx@_la_SOURCES += CEGuiDirectFBBaseApplication.cpp
    libCEGUISampleHelper@cegui_bsfx@_la_LIBADD += $(top_builddir)/cegui/src/RendererModules/DirectFB/libCEGUIDirectFBRenderer@cegui_bsfx@.la \
                                      $(directfb_LIBS)
endif

EXTRA_DIST = CEGuiD3D10BaseApplication.cpp \
             CEGuiD3D9BaseApplication.cpp \
             DeviceReset_Direct3D10.cpp \
             DeviceReset_Direct3D9.cpp \
             MacCEGuiRendererSelector.cpp \
             Win32AppHelper.cpp \
             Win32CEGuiRendererSelector.cpp
