SUBDIRS = . src include

pkgconfigdir = $(libdir)/pkgconfig

if BUILD_OPENGL_RENDERER
    CEGUI_OPENGL_PC = CEGUI-OPENGL.pc
endif
if BUILD_OGRE_RENDERER
    CEGUI_OGRE_PC = CEGUI-OGRE.pc
endif

pkgconfig_DATA = CEGUI.pc $(CEGUI_OPENGL_PC) $(CEGUI_OGRE_PC)
