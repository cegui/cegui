AC_DEFUN([CEGUI_ENABLE_OGRE_RENDERER], [
    PKG_CHECK_MODULES(CEGUIOGRE, CEGUI-OGRE >= 1.0.0, [cegui_found_ogre_renderer=yes], [cegui_found_ogre_renderer=no])
    PKG_CHECK_MODULES(CEGUI_NULL, CEGUI, [cegui_found_cegui=yes], [cegui_found_cegui=no])    
    AC_ARG_WITH([ogre-renderer], AC_HELP_STRING([--without-ogre-renderer], [Disables the use of the Ogre3D renderer, when available, in samples]),
                [cegui_with_ogre=$withval], [cegui_with_ogre=yes])

    if test x$cegui_found_ogre_renderer = xyes && test x$cegui_found_cegui = xyes && test x$cegui_with_ogre = xyes; then
        AC_DEFINE(CEGUI_SAMPLES_USE_OGRE, [], [Define to have the Ogre3D CEGUI renderer available in the samples])
        AC_MSG_NOTICE([Use of Ogre3D in Samples is enabled])
    else        
        AC_MSG_NOTICE([Use of Ogre3D in Samples is disabled])
    fi

    AM_CONDITIONAL([CEGUI_SAMPLES_USE_OGRE], [test x$cegui_found_ogre_renderer = xyes && test x$cegui_found_cegui = xyes && test x$cegui_with_ogre = xyes])
    AC_SUBST(CEGUIOGRE_CFLAGS)
    AC_SUBST(CEGUIOGRE_LIBS)
])

AC_DEFUN([CEGUI_ENABLE_IRRLICHT_RENDERER], [
    CEGUI_CHECK_IRRLICHT(Irrlicht, [cegui_found_irr=yes], [cegui_found_irr=no])
    AC_CHECK_LIB([Xxf86vm], XF86VidModeQueryVersion, [cegui_found_xf86vm=yes], [cegui_found_xf86vm=no])
    AC_ARG_ENABLE([irrlicht-renderer], AC_HELP_STRING([--disable-irrlicht-renderer], [Disable the Irrlicht renderer]),
        [cegui_enable_irr=$enableval],[cegui_enable_irr=yes])
    
    if test x$cegui_enable_irr = xyes && test x$cegui_found_irr = xyes; then
        AC_MSG_NOTICE([Irrlicht renderer enabled])
        
        if test x$cegui_found_xf86vm = xyes; then
            Irrlicht_LIBS="-lXxf86vm $Irrlicht_LIBS"
            AC_DEFINE(CEGUI_SAMPLES_USE_IRRLICHT, [], [Define to have the Irrlicht CEGUI renderer available in the samples])
            AC_MSG_NOTICE([Use of Irrlicht in Samples is enabled])
        else
            AC_MSG_NOTICE([Use of Irrlicht in Samples is disabled])
        fi
    else
        AC_MSG_NOTICE([Irrlicht renderer disabled])
    fi
    
    AM_CONDITIONAL([BUILD_IRRLICHT_RENDERER], [test x$cegui_found_irr = xyes && test x$cegui_enable_irr = xyes])
    AM_CONDITIONAL([CEGUI_SAMPLES_USE_IRRLICHT], [test x$cegui_found_irr = xyes && test x$cegui_enable_irr = xyes && test x$cegui_found_xf86vm = xyes])
    AC_SUBST(Irrlicht_CFLAGS)
    AC_SUBST(Irrlicht_LIBS)
])

# CEGUI_CHECK_IRRLICHT(variable, [action-if-found], [action-if-not-found])
# checks for Irrlicht headers and libs in some common places.
AC_DEFUN([CEGUI_CHECK_IRRLICHT],[
    AC_ARG_WITH([irrlicht-incdir], AC_HELP_STRING([--with-irrlicht-incdir=DIR], [Optionally specifies location of the Irrlicht includes]),
        [cegui_irr_incdir=$withval],[cegui_irr_incdir=[.]])
    AC_ARG_WITH([irrlicht-libdir], AC_HELP_STRING([--with-irrlicht-libdir=DIR], [Optionally specifies location of the Irrlicht libraries]),
        [cegui_irr_libdir=$withval],[cegui_irr_libdir=[.]])
    cegui_lib_paths="$cegui_irr_libdir"
    cegui_inc_paths="$cegui_irr_incdir /usr/local/include/irrlicht /usr/include/irrlicht /usr/local/include /usr/include"
    
    $1_CFLAGS=
    $1_LIBS=
    
    cegui_saved_CFLAGS="$CPPFLAGS"
    cegui_saved_LIBS="$LIBS"
    
    for cegui_path in $cegui_inc_paths; do
        ifelse($cegui_path, [.], CPPFLAGS="$cegui_saved_CFLAGS", CPPFLAGS="-I$cegui_path $cegui_saved_CFLAGS")
        AC_PREPROC_IFELSE([#include <irrlicht.h>], [cegui_irr_h_found=yes; cegui_irr_flags="$cegui_path"; break],[cegui_irr_h_found=no])
    done

    for cegui_path in $cegui_lib_paths; do
        ifelse($cegui_path, [.], LIBS="$cegui_saved_LIBS", LIBS="-L$cegui_path $saved_LIBS")
        AC_CHECK_LIB(Irrlicht, main, [cegui_irr_l_found=yes; cegui_irr_libs="$cegui_path"; break],[cegui_irr_l_found=no; unset ac_cv_lib_Irrlicht_main])
    done

    CPPFLAGS="$cegui_saved_CFLAGS"
    LIBS="$cegui_saved_LIBS"

    if test x$cegui_irr_h_found = xyes && test x$cegui_irr_l_found = xyes; then
        if test x$cegui_irr_flags != x.; then
            $1_CFLAGS="-I$cegui_irr_flags"
        fi
        if test x$cegui_irr_libs = x.; then
            $1_LIBS="-lIrrlicht"
        else
            $1_LIBS="-L$cegui_irr_libs -lIrrlicht"
        fi

        ifelse([$2], [], :, [$2])
    else
        ifelse([$3], [], :, [$3])
    fi    
])

AC_DEFUN([CEGUI_ENABLE_OPENGL_RENDERER], [
    AC_ARG_ENABLE([opengl-renderer], AC_HELP_STRING([--disable-opengl-renderer], [Disable the OpenGL renderer]),
        [cegui_enable_opengl=$enableval], [cegui_enable_opengl=yes])
    AC_ARG_WITH([devil], AC_HELP_STRING([--without-devil], [Disables image loading via DevIL by OpenGL renderer]),
        [cegui_with_devil=$withval], [cegui_with_devil=yes])

    AC_PATH_XTRA
    cegui_saved_LIBS="$LIBS"
    
    LIBS="$X_PRE_LIBS $X_LIBS $X_EXTRA_LIBS"
    
    AC_SEARCH_LIBS(glInterleavedArrays, MesaGL GL, cegui_found_lib_GL=yes, cegui_found_lib_GL=no)
    AC_SEARCH_LIBS(gluOrtho2D, MesaGLU GLU, cegui_found_lib_GLU=yes,  cegui_found_lib_GLU=no)
    AC_SEARCH_LIBS(glutInit, glut, cegui_found_lib_glut=yes, cegui_found_lib_glut=no)
    OpenGL_CFLAGS="$X_CFLAGS"
    OpenGL_LIBS=$LIBS
    LIBS="$cegui_saved_LIBS"
    
    if test x$cegui_enable_opengl = xyes && test x$cegui_found_lib_GL = xyes && test x$cegui_found_lib_GLU = xyes; then
        AC_MSG_NOTICE([OpenGL renderer enabled])

        if test x$cegui_with_devil = xyes; then
            AC_CHECK_LIB(ILU, iluFlipImage,,AC_MSG_ERROR([Library ilu required for DevIL support is missing]),-lIL)
            DevIL_CFLAGS=-DUSE_DEVIL_LIBRARY
            AC_MSG_NOTICE([Image loading via DevIL by OpenGL renderer enabled])
        else            
            AC_MSG_NOTICE([Image loading via DevIL by OpenGL renderer disabled])
        fi
        if test x$cegui_found_lib_glut = xyes; then
            AC_DEFINE(CEGUI_SAMPLES_USE_OPENGL, [], [Define to have the OpenGL CEGUI renderer available in the samples])
            AC_MSG_NOTICE([Use of OpenGL in Samples is enabled])
        else
            AC_MSG_NOTICE([Use of OpenGL in Samples is disabled])
        fi
    else
        AC_MSG_NOTICE([OpenGL renderer disabled])        
    fi

    AM_CONDITIONAL([BUILD_OPENGL_RENDERER], [test x$cegui_enable_opengl = xyes && test x$cegui_found_lib_GL = xyes && test x$cegui_found_lib_GLU = xyes])
    AM_CONDITIONAL([CEGUI_SAMPLES_USE_OPENGL], [test x$cegui_enable_opengl = xyes && test x$cegui_found_lib_GL = xyes && test x$cegui_found_lib_GLU = xyes && test x$cegui_found_lib_glut = xyes])
    AC_SUBST(OpenGL_CFLAGS)
    AC_SUBST(OpenGL_LIBS)
    AC_SUBST(DevIL_CFLAGS)
])
