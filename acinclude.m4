AC_DEFUN([CEGUI_ENABLE_OGRE_RENDERER], [
    PKG_CHECK_MODULES(OGRE, OGRE >= 1.0.0, [
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

AC_DEFUN([CEGUI_ENABLE_OPENGL_RENDERER], [
    CEGUI_CHECK_OPENGL([
        AC_ARG_ENABLE([opengl-renderer],
            AC_HELP_STRING([--disable-opengl-renderer],
            [Disable the OpenGL renderer]),
            [build_opengl_renderer=false], [build_opengl_renderer=true])
        AC_CHECK_LIB(IL, ilInit, [
            AC_ARG_WITH([devil],
                AC_HELP_STRING([--without-devil],
                [Disables image loading via DevIL by OpenGL renderer]),
                [use_devil_library=$withval], [use_devil_library=yes])
        ], [use_devil_library=no])
    ], [
        [build_opengl_renderer=false]
        [use_devil_library=no]
    ])
    if test x$build_opengl_renderer = xtrue; then
        AC_CONFIG_FILES([include/renderers/OpenGLGUIRenderer/Makefile \
                         src/renderers/OpenGLGUIRenderer/Makefile \
                         CEGUI-OPENGL.pc])
        AC_MSG_RESULT([OpenGL renderer enabled])
        if test x$use_devil_library = xyes; then
            AC_CHECK_LIB(ILU, iluFlipImage,,AC_MSG_ERROR([Library ilu required for DevIL support is missing]))
            DevIL_CFLAGS=-DUSE_DEVIL_LIBRARY
            AC_MSG_RESULT([Image loading via DevIL by OpenGL renderer enabled])
        else
            AC_MSG_RESULT([Image loading via DevIL by OpenGL renderer disabled])
        fi
    else
        AC_MSG_RESULT([OpenGL renderer disabled])
    fi
    AC_SUBST(OpenGL_CFLAGS)
    AC_SUBST(OpenGL_LIBS)
    AC_SUBST(DevIL_CFLAGS)
    AM_CONDITIONAL([BUILD_OPENGL_RENDERER], [test x$build_opengl_renderer = xtrue])
])

AC_DEFUN([CEGUI_CHECK_OPENGL], [
    AC_PATH_XTRA
    GLSAVEDLIBS="$LIBS"
    LIBS="$X_PRE_LIBS $X_LIBS $X_EXTRA_LIBS"
    AC_SEARCH_LIBS(glInterleavedArrays, MesaGL GL, found_lib_GL=true, found_lib_GL=false)
    AC_SEARCH_LIBS(gluOrtho2D, MesaGLU GLU, found_lib_GLU=true,  found_lib_GLU=false)
    OpenGL_LIBS=$LIBS
    LIBS="$GLSAVEDLIBS"
    if test x$found_lib_GL = xtrue && test x$found_lib_GLU = xtrue; then
        OpenGL_CFLAGS="$X_CFLAGS"
        $1
    else
        $2
    fi
])
