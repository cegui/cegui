AC_DEFUN([CEGUI_ENABLE_OPENGL_RENDERER], [
    AC_ARG_ENABLE([opengl-renderer], AC_HELP_STRING([--disable-opengl-renderer], [Disable the OpenGL renderer]),
        [cegui_enable_opengl=$enableval], [cegui_enable_opengl=yes])
    AC_ARG_WITH([devil], AC_HELP_STRING([--without-devil], [Disables image loading via DevIL by OpenGL renderer]),
        [cegui_with_devil=$withval], [cegui_with_devil=yes])
    CEGUI_CHECK_OPENGL([cegui_found_opengl=yes], [cegui_found_opengl=no])
    AC_CHECK_LIB(IL, ilInit, [cegui_found_devil=yes], [cegui_found_devil=no])

    if test x$cegui_enable_opengl = xyes && test x$cegui_found_opengl = xyes; then
        AC_MSG_RESULT([OpenGL renderer enabled])

        if test x$cegui_with_devil = xyes && test x$cegui_found_devil = xyes; then
            AC_CHECK_LIB(ILU, iluFlipImage,,AC_MSG_ERROR([Library ilu required for DevIL support is missing]),-lIL)
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
    AM_CONDITIONAL([BUILD_OPENGL_RENDERER], [test x$cegui_enable_opengl = xyes && test x$cegui_found_opengl = xyes])
])

AC_DEFUN([CEGUI_CHECK_OPENGL], [
    AC_PATH_XTRA
    GLSAVEDLIBS="$LIBS"
    LIBS="$X_PRE_LIBS $X_LIBS $X_EXTRA_LIBS"
    AC_SEARCH_LIBS(glInterleavedArrays, MesaGL GL, found_lib_GL=yes, found_lib_GL=no)
    AC_SEARCH_LIBS(gluOrtho2D, MesaGLU GLU, found_lib_GLU=yes,  found_lib_GLU=no)
    OpenGL_LIBS=$LIBS
    LIBS="$GLSAVEDLIBS"
    if test x$found_lib_GL = xyes && test x$found_lib_GLU = xyes; then
        OpenGL_CFLAGS="$X_CFLAGS"
        $1
    else
        $2
    fi
])
