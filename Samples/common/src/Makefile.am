INCLUDES = -I$(top_srcdir)/Samples/common/include -I$(top_srcdir)/include -I$(top_srcdir) $(CEGUI_SAMPLES_CFLAGS)

lib_LTLIBRARIES=libCEGUISampleHelper.la

libCEGUISampleHelper_la_LDFLAGS = -version-info @CEGUI_VERSION_INFO@

libCEGUISampleHelper_la_SOURCES = CEGuiRendererSelector.cpp \
                                  CEGuiSample.cpp \
                                  CLICEGuiRendererSelector.cpp

libCEGUISampleHelper_la_LIBADD =


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
    libCEGUISampleHelper_la_LDFLAGS += -L$(top_builddir)/RendererModules/OpenGLGUIRenderer
    libCEGUISampleHelper_la_LIBADD += -lCEGUIOpenGLRenderer $(OpenGL_LIBS)
endif
if CEGUI_SAMPLES_USE_IRRLICHT
    INCLUDES += $(Irrlicht_CFLAGS)
    libCEGUISampleHelper_la_SOURCES += CEGuiIrrlichtBaseApplication.cpp
    libCEGUISampleHelper_la_LDFLAGS += -L$(top_builddir)/RendererModules/IrrlichtRenderer
    libCEGUISampleHelper_la_LIBADD += -lCEGUIIrrlichtRenderer $(Irrlicht_LIBS)
endif

EXTRA_DIST = CEGuiD3D81BaseApplication.cpp \
             CEGuiD3D9BaseApplication.cpp \
             MacCEGuiRendererSelector.cpp \
             Win32AppHelper.cpp \
             Win32CEGuiRendererSelector.cpp
