INCLUDES = -I$(top_srcdir)/Samples/common/include \
           -I$(top_srcdir)/cegui/include \
           $(CEGUI_SAMPLES_CFLAGS)

lib_LTLIBRARIES=libCEGUISampleHelper.la

libCEGUISampleHelper_la_LDFLAGS = -version-info @CEGUI_VERSION_INFO@

libCEGUISampleHelper_la_SOURCES = CEGuiBaseApplication.cpp \
                                  CEGuiRendererSelector.cpp \
                                  CEGuiSample.cpp \
                                  CLICEGuiRendererSelector.cpp

libCEGUISampleHelper_la_LIBADD = $(top_builddir)/cegui/src/libCEGUIBase.la

if MINGW32_BUILD
libCEGUISampleHelper_la_CPPFLAGS = -DCEGUISAMPLE_EXPORTS
libCEGUISampleHelper_la_LDFLAGS += -shared -no-undefined
libCEGUISampleHelper_la_SOURCES  += Win32AppHelper.cpp Win32CEGuiRendererSelector.cpp
endif

if CEGUI_USING_GTK2
    INCLUDES += $(GTK_CFLAGS)
    libCEGUISampleHelper_la_LIBADD += $(GTK_LIBS)
    libCEGUISampleHelper_la_SOURCES += GTK2CEGuiRendererSelector.cpp
endif
if CEGUI_SAMPLES_USE_OGRE
    INCLUDES += $(CEGUIOGRE_CFLAGS) $(OIS_CFLAGS)
    libCEGUISampleHelper_la_SOURCES += CEGuiOgreBaseApplication.cpp
    libCEGUISampleHelper_la_LIBADD += $(CEGUIOGRE_LIBS) $(OIS_LIBS)
endif
if CEGUI_SAMPLES_USE_OPENGL
    libCEGUISampleHelper_la_SOURCES += CEGuiOpenGLBaseApplication.cpp
    libCEGUISampleHelper_la_LIBADD += $(top_builddir)/cegui/src/RendererModules/OpenGLGUIRenderer/libCEGUIOpenGLRenderer.la \
                                      $(OpenGL_LIBS)
endif
if CEGUI_SAMPLES_USE_IRRLICHT
    INCLUDES += $(Irrlicht_CFLAGS)
    libCEGUISampleHelper_la_SOURCES += CEGuiIrrlichtBaseApplication.cpp
    libCEGUISampleHelper_la_LIBADD += $(top_builddir)/cegui/src/RendererModules/IrrlichtRenderer/libCEGUIIrrlichtRenderer.la \
                                      $(Irrlicht_LIBS)
endif

EXTRA_DIST = CEGuiD3D10BaseApplication.cpp \
             CEGuiD3D81BaseApplication.cpp \
             CEGuiD3D9BaseApplication.cpp \
             DeviceReset_Direct3D10.cpp \
             DeviceReset_Direct3D81.cpp \
             DeviceReset_Direct3D9.cpp \
             MacCEGuiRendererSelector.cpp \
             Win32AppHelper.cpp \
             Win32CEGuiRendererSelector.cpp
