INCLUDES = -I$(top_srcdir)/Samples/common/include -I$(top_srcdir)/include

lib_LTLIBRARIES=libCEGUISampleHelper.la

libCEGUISampleHelper_la_SOURCES = CEGuiRendererSelector.cpp \
                                  CEGuiSample.cpp \
                                  CLICEGuiRendererSelector.cpp
##                                GTK2CEGuiRendererSelector.cpp

libCEGUISampleHelper_la_LIBADD =
libCEGUISampleHelper_la_LDFLAGS =

if CEGUI_SAMPLES_USE_OGRE
    INCLUDES += $(CEGUIOGRE_CFLAGS)
    libCEGUISampleHelper_la_SOURCES += CEGuiOgreBaseApplication.cpp
    libCEGUISampleHelper_la_LIBADD += $(CEGUIOGRE_LIBS)
endif
if CEGUI_SAMPLES_USE_OPENGL
    libCEGUISampleHelper_la_SOURCES += CEGuiOpenGLBaseApplication.cpp
    libCEGUISampleHelper_la_LDFLAGS += -L$(top_builddir)/src/renderers/OpenGLGUIRenderer
    libCEGUISampleHelper_la_LIBADD += -lCEGUIOpenGLRenderer $(OpenGL_LIBS)
endif
if CEGUI_SAMPLES_USE_IRRLICHT
    INCLUDES += $(Irrlicht_CFLAGS)
    libCEGUISampleHelper_la_SOURCES += CEGuiIrrlichtBaseApplication.cpp
    libCEGUISampleHelper_la_LDFLAGS += -L$(top_builddir)/src/renderers/IrrlichtRenderer
    libCEGUISampleHelper_la_LIBADD += -lCEGUIIrrlichtRenderer $(Irrlicht_LIBS)
endif
