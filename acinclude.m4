AC_DEFUN([CEGUI_ENABLE_OGRE_RENDERER], [
    PKG_CHECK_MODULES(OGRE, OGRE >= 0.14.0, [
        AC_ARG_ENABLE([ogre-renderer],
            AC_HELP_STRING([--disable-ogre-renderer], 
            [Disable the ogre renderer]),
            [build_ogre_renderer=false], [build_ogre_renderer=true])
    ], [build_ogre_renderer=false])
    if test x$build_ogre_renderer = xtrue; then
        AC_CONFIG_FILES([include/renderers/OgreGUIRenderer/Makefile \
                         src/renderers/OgreGUIRenderer/Makefile \
                         CEGUI-OGRE.pc])
        AC_SUBST(OGRE_CFLAGS)
        AC_SUBST(OGRE_LIBS)
        AC_MSG_RESULT([OGRE renderer enabled])
    else
        AC_MSG_RESULT([OGRE renderer disabled])
    fi
    AM_CONDITIONAL([BUILD_OGRE_RENDERER], [test x$build_ogre_renderer = xtrue])
])            
