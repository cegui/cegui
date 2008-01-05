AC_DEFUN([CEGUI_CODE_OPTIONS], [
    AC_ARG_ENABLE([debug], AC_HELP_STRING([--enable-debug], [Enable building CEGUI in debugging mode.]), 
        [cegui_enable_debug=$enableval], [cegui_enable_debug=no])
    dnl Code options debug
    if test x$cegui_enable_debug = xyes ; then 
        CPPFLAGS="$CPPFLAGS -DDEBUG"
    fi
])

AC_DEFUN([CEGUI_CHECK_WANTS_SAMPLES],[
    AC_ARG_ENABLE([samples], AC_HELP_STRING([--disable-samples], [Disable building of samples framework and applications.]),
        [cegui_enable_samples=$enableval],[cegui_enable_samples=yes])

    if test x$cegui_enable_samples = xyes; then
        if test x$cegui_samples_use_ogre = xyes || test x$cegui_samples_use_irrlicht = xyes || test x$cegui_samples_use_opengl = xyes; then
            cegui_build_samples=yes
            AC_MSG_NOTICE([Samples framework and applications are enabled.])
            CEGUI_SAMPLES_CFLAGS='-DCEGUI_SAMPLE_DATAPATH="\"$(datadir)/$(PACKAGE)"\"'
        else
            cegui_build_samples=no
            AC_MSG_NOTICE([No renderers available.  Building of samples framework and applications has been disabled.])
        fi
    else
        cegui_build_samples=no
        AC_MSG_NOTICE([Samples framework and applications are disabled.])
    fi

    AM_CONDITIONAL([CEGUI_BUILD_SAMPLES], [test x$cegui_build_samples = xyes])
    AC_SUBST(CEGUI_SAMPLES_CFLAGS)
])

AC_DEFUN([CEGUI_CHECK_GTK_FOR_SAMPLES],[
    PKG_CHECK_MODULES(GTK, gtk+-2.0 >= 2.4, [cegui_found_gtk=yes], [cegui_found_gtk=no])
    AC_ARG_WITH([gtk2], AC_HELP_STRING([--with-gtk2], [Enables the use of a GTK2 based dialog to select a renderer in the samples]),
                [cegui_with_gtk=$withval], [cegui_with_gtk=no])

    if test x$cegui_found_gtk = xyes && test x$cegui_with_gtk = xyes; then
        cegui_with_gtk=yes
    else
        cegui_with_gtk=no
    fi

    if test x$cegui_with_gtk = xyes; then
        AC_DEFINE(CEGUI_SAMPLES_USE_GTK2, [], [Define to have a GTK2 based dialog used for renderer selection in the samples])
        AC_MSG_NOTICE([GTK2 renderer selection dialog in samples is enabled])
    else
        AC_MSG_NOTICE([GTK2 renderer selection dialog in samples is disabled])
    fi

    AM_CONDITIONAL([CEGUI_USING_GTK2], [test x$cegui_with_gtk = xyes])
    AC_SUBST(GTK_CFLAGS)
    AC_SUBST(GTK_LIBS)
])

AC_DEFUN([CEGUI_CHECK_XML_PARSERS],[
    PKG_CHECK_MODULES(libxml, libxml-2.0 >= 2.6, [cegui_found_libxml=yes], [cegui_found_libxml=no])
    CEGUI_CHECK_XERCES(xerces, [cegui_found_xerces=yes], [cegui_found_xerces=no])
    CEGUI_CHECK_EXPAT(expat, [cegui_found_expat=yes], [cegui_found_expat=no])

    dnl save lots of linker aggro ;)
    LIBS="$xerces_LIBS $LIBS"

    dnl Find out what user disabled as far as parsers go
    AC_ARG_ENABLE([xerces-c], AC_HELP_STRING([--disable-xerces-c], [Disables building of the Xerces-C++ XML parser module.]),
                [cegui_with_xerces=$enableval], [cegui_with_xerces=yes])

    AC_ARG_ENABLE([libxml], AC_HELP_STRING([--disable-libxml], [Disables building of the libxml XML parser module.]),
                [cegui_with_libxml=$enableval], [cegui_with_libxml=yes])

    AC_ARG_ENABLE([expat], AC_HELP_STRING([--disable-expat], [Disables building of the expat XML parser module.]),
                [cegui_with_expat=$enableval], [cegui_with_expat=yes])

    AC_ARG_ENABLE([tinyxml], AC_HELP_STRING([--disable-tinyxml], [Disables building of the tinyXML parser module.]),
                [cegui_with_tinyxml=$enableval], [cegui_with_tinyxml=yes])

    dnl Find out which paser user wants as a default
    AC_ARG_WITH([default-xml-parser], AC_HELP_STRING([--with-default-xml-parser[=PARSER]], [Sets the default XML parser module.
Typically this will be one of XercesParser, ExpatParser, LibxmlParser or TinyXMLParser, though you can set it to anything to
load a custom made parser module as the default.]),
                [cegui_default_parser=$withval], [cegui_default_parser=none])

    dnl Determine what we will actually be building based on user preference and what's available on the system.
    if test x$cegui_found_xerces = xyes && test x$cegui_with_xerces = xyes; then
        cegui_with_xerces=yes
    else
        cegui_with_xerces=no
    fi
    if test x$cegui_found_libxml = xyes && test x$cegui_with_libxml = xyes; then
        cegui_with_libxml=yes
    else
        cegui_with_libxml=no
    fi
    if test x$cegui_found_expat = xyes && test x$cegui_with_expat = xyes; then
        cegui_with_expat=yes
    else
        cegui_with_expat=no
    fi

    dnl reset default parser to 'none' if current selection will not be built
    if test x$cegui_default_parser = xXercesParser && test x$cegui_with_xerces = xno; then
        cegui_default_parser=none
    fi
    if test x$cegui_default_parser = xExpatParser && test x$cegui_with_expat = xno; then
        cegui_default_parser=none
    fi
    if test x$cegui_default_parser = xLibxmlParser && test x$cegui_with_libxml = xno; then
        cegui_default_parser=none
    fi
    if test x$cegui_default_parser = xTinyXMLParser && test x$cegui_with_tinyxml = xno; then
        cegui_default_parser=none
    fi

    dnl if no parser is selected as default, choose one that will be available
    if test x$cegui_default_parser = xnone; then
        if test x$cegui_with_xerces = xyes; then
            cegui_default_parser=XercesParser
        else
            if test x$cegui_with_expat = xyes; then
                cegui_default_parser=ExpatParser
            else
                if test x$cegui_with_libxml = xyes; then
                    cegui_default_parser=LibxmlParser
                else
                    if test x$cegui_with_tinyxml = xyes; then
                        cegui_default_parser=TinyXMLParser
                    else
                        AC_MSG_ERROR([None of the XMLParsers are going to be built - unable to continue.  Either enable a parser or set a custom default.])
                    fi
                fi
            fi
        fi
    fi

    dnl define macro for the class of the default xml parser to be used
    AC_DEFINE_UNQUOTED(CEGUI_DEFAULT_XMLPARSER, $cegui_default_parser, [Set this to the default XMLParser to be used (XercesParser, ExpatParser, LibxmlParser or TinyXMLParser).])
    AC_MSG_NOTICE([Default XML Parser will be: $cegui_default_parser])

    dnl automake conditionals
    AM_CONDITIONAL([BUILD_XERCES_PARSER], [test x$cegui_with_xerces = xyes])
    AM_CONDITIONAL([BUILD_LIBXML_PARSER], [test x$cegui_with_libxml = xyes])
    AM_CONDITIONAL([BUILD_EXPAT_PARSER], [test x$cegui_with_expat = xyes])
    AM_CONDITIONAL([BUILD_TINYXML_PARSER], [test x$cegui_with_tinyxml = xyes])

    AC_SUBST(xerces_CFLAGS)
    AC_SUBST(xerces_LIBS)
    AC_SUBST(expat_CFLAGS)
    AC_SUBST(expat_LIBS)
])

AC_DEFUN([CEGUI_ENABLE_OGRE_RENDERER], [
    PKG_CHECK_MODULES(CEGUIOGRE, CEGUI-OGRE >= 1.0.0, [cegui_found_ogre_renderer=yes], [cegui_found_ogre_renderer=no])
    PKG_CHECK_MODULES(CEGUI_NULL, CEGUI, [cegui_found_cegui=yes], [cegui_found_cegui=no])    
	PKG_CHECK_MODULES(OIS, OIS >= 1.0.0, [ois_found=yes],[ois_found=no])

    AC_ARG_WITH([ogre-renderer], AC_HELP_STRING([--without-ogre-renderer], [Disables the use of the Ogre3D renderer, when available, in samples]),
                [cegui_with_ogre=$withval], [cegui_with_ogre=yes])


	if test "x$ois_found" = "xno" ; then
		cegui_samples_use_ogre=no
		AC_MSG_NOTICE([
****************************************************************
* You do not have OIS installed.  This is required to build    *
* Ogre CEGUI demos. You may find it at:                        *
* http://www.sourceforge.net/projects/wgois.                   *
* If you do not want to build the demos, you can safely ignore *
* this.                                                        *
****************************************************************])
	fi

    if test x$cegui_found_ogre_renderer = xyes && test x$cegui_found_cegui = xyes && test x$cegui_with_ogre = xyes && test x$ois_found = xyes; then
        cegui_samples_use_ogre=yes
        AC_DEFINE(CEGUI_SAMPLES_USE_OGRE, [], [Define to have the Ogre3D CEGUI renderer available in the samples])
        AC_MSG_NOTICE([Use of Ogre3D in Samples is enabled])
    else        
        cegui_samples_use_ogre=no
        AC_MSG_NOTICE([Use of Ogre3D in Samples is disabled])
    fi

    AM_CONDITIONAL([CEGUI_SAMPLES_USE_OGRE], [test x$cegui_samples_use_ogre = xyes])
	AC_SUBST(OIS_CFLAGS)
	AC_SUBST(OIS_LIBS)
    AC_SUBST(CEGUIOGRE_CFLAGS)
    AC_SUBST(CEGUIOGRE_LIBS)
])

AC_DEFUN([CEGUI_ENABLE_IRRLICHT_RENDERER], [
    CEGUI_CHECK_IRRLICHT(Irrlicht, [cegui_found_irr=yes], [cegui_found_irr=no])
    AC_CHECK_LIB([Xxf86vm], XF86VidModeQueryVersion, [cegui_found_xf86vm=yes], [cegui_found_xf86vm=no])
    AC_ARG_ENABLE([irrlicht-renderer], AC_HELP_STRING([--disable-irrlicht-renderer], [Disable the Irrlicht renderer]),
        [cegui_enable_irr=$enableval],[cegui_enable_irr=yes])

    dnl decide if we will actually build the Irrlicht Renderer
    if test x$cegui_enable_irr = xyes && test x$cegui_found_irr = xyes; then
        cegui_enable_irrlicht=yes
    else
        cegui_enable_irrlicht=no
    fi

    if test x$cegui_enable_irrlicht = xyes; then
        AC_MSG_NOTICE([Irrlicht renderer enabled])

        if test x$cegui_found_xf86vm = xyes; then
            Irrlicht_LIBS="-lXxf86vm $Irrlicht_LIBS"
            cegui_samples_use_irrlicht=yes
            AC_DEFINE(CEGUI_SAMPLES_USE_IRRLICHT, [], [Define to have the Irrlicht CEGUI renderer available in the samples])
            AC_MSG_NOTICE([Use of Irrlicht in Samples is enabled])
        else
            cegui_samples_use_irrlicht=no
            AC_MSG_NOTICE([Use of Irrlicht in Samples is disabled])
        fi
    else
        cegui_samples_use_irrlicht=no
        AC_MSG_NOTICE([Irrlicht renderer disabled])
    fi
    
    AM_CONDITIONAL([BUILD_IRRLICHT_RENDERER], [test x$cegui_enable_irrlicht = xyes])
    AM_CONDITIONAL([CEGUI_SAMPLES_USE_IRRLICHT], [test x$cegui_samples_use_irrlicht = xyes])
    AC_SUBST(Irrlicht_CFLAGS)
    AC_SUBST(Irrlicht_LIBS)
])

# CEGUI_CHECK_IRRLICHT(variable, [action-if-found], [action-if-not-found])
# checks for Irrlicht headers and libs in some common places.
AC_DEFUN([CEGUI_CHECK_IRRLICHT],[
    AC_LANG_PUSH([C++])
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
        ifelse($cegui_path, [.], LIBS="$cegui_saved_LIBS", LIBS="-L$cegui_path $cegui_saved_LIBS")
        AC_CHECK_LIB(Irrlicht, main, [cegui_irr_l_found=yes; cegui_irr_libs="$cegui_path"; break],[cegui_irr_l_found=no; unset ac_cv_lib_Irrlicht_main])
    done

    dnl The following is all to set a 'usable' macro for the Irrlicht version, as opposed
    dnl to the IRRLICHT_SDK_VERSION macro in irrlicht which is limited for our purpose.
    dnl if anyone knows a better way - let us know :)
    gotirrsdkver=no
    if test x$cegui_irr_h_found = xyes && test x$cegui_irr_l_found = xyes; then
        AC_MSG_NOTICE([Trying to determine Irrlicht SDK version])
        AC_COMPILE_IFELSE(
        [
            #include <irrlicht.h>
            class test : public irr::io::IReadFile
            {
            public:
                bool seek(irr::s32 finalPos, bool relativeMovement = false) {return false; }
                irr::s32 read(void* buffer, irr::s32 sizeToRead) {return 0;}
                irr::s32 getSize() {return 0;}
                irr::s32 getPos() {return 0;}
                const irr::c8* getFileName() {return 0;}
            };
            int main(int argc, char** argv) { test x; return 0; }
        ],
        [AC_DEFINE([CEGUI_IRR_SDK_VERSION],[12],[Defines irrlicht SDK version.  12 is 1.2 or below. 13 is 1.3.x and 14 is 1.4 or later.])
        gotirrsdkver=yes])
    
        if test $gotirrsdkver = no; then
        AC_COMPILE_IFELSE(
        [
            #include <irrlicht.h>
            class test : public irr::io::IReadFile
            {
            public:
                bool seek(irr::s32 finalPos, bool relativeMovement = false) {return false; }
                irr::s32 read(void* buffer, irr::u32 sizeToRead) {return 0;}
                irr::s32 getSize() {return 0;}
                irr::s32 getPos() {return 0;}
                const irr::c8* getFileName() {return 0;}
            };
            int main(int argc, char** argv) { test x; return 0; }
        ],
        [AC_DEFINE([CEGUI_IRR_SDK_VERSION],[13],[Defines irrlicht SDK version.  12 is 1.2 or below. 13 is 1.3.x and 14 is 1.4 or later.])
        gotirrsdkver=yes])
        fi
    
        if test $gotirrsdkver = no; then
        AC_COMPILE_IFELSE(
        [
            #include <irrlicht.h>
            class test : public irr::io::IReadFile
            {
            public:
                bool seek(long finalPos, bool relativeMovement = false) {return false; }
                irr::s32 read(void* buffer, irr::u32 sizeToRead) {return 0;}
                long getSize() const {return 0;}
                long getPos() const {return 0;}
                const irr::c8* getFileName() const {return 0;}
            };
            int main(int argc, char** argv) { test x; return 0; }
        ],
        [AC_DEFINE([CEGUI_IRR_SDK_VERSION],[14],[Defines irrlicht SDK version.  12 is 1.2 or below. 13 is 1.3.x and 14 is 1.4 or later.])
        gotirrsdkver=yes],
        [AC_MSG_NOTICE([Unable to determine Irrlicht sdk version.])
        ])
        fi
    fi

    CPPFLAGS="$cegui_saved_CFLAGS"
    LIBS="$cegui_saved_LIBS"

    if test x$cegui_irr_h_found = xyes && test x$cegui_irr_l_found = xyes && test x$gotirrsdkver = xyes; then
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
    AC_LANG_POP([C++])
])

AC_DEFUN([CEGUI_ENABLE_OPENGL_RENDERER], [
    AC_ARG_ENABLE([opengl-renderer], AC_HELP_STRING([--disable-opengl-renderer], [Disable the OpenGL renderer]),
        [cegui_enable_opengl=$enableval], [cegui_enable_opengl=yes])
    AC_ARG_ENABLE([corona], AC_HELP_STRING([--enable-corona], [Enable image loading via Corona image codec by OpenGL renderer (auto)]), 
        [cegui_with_corona=$enableval], [cegui_with_corona=yes])
    AC_ARG_WITH(corona-prefix, AC_HELP_STRING([--with-corona-prefix], [Prefix where corona is installed (optional)]), 
        [cegui_corona_prefix="$withval"], [cegui_corona_prefix=""])
    AC_ARG_ENABLE([devil], AC_HELP_STRING([--enable-devil], [Enable image loading via DevIL image codec by OpenGL renderer (auto)]),
        [cegui_with_devil=$enableval], [cegui_with_devil=yes])
    AC_ARG_ENABLE([freeimage], AC_HELP_STRING([--enable-freeimage], [Disabled image loading via FreeImage image codec by OpenGL renderer (auto)]), 
        [cegui_with_freeimage=$enableval], [cegui_with_freeimage=yes])
    AC_ARG_ENABLE([silly], AC_HELP_STRING([--enable-silly], [Enable image loading via SILLY image codec by OpenGL renderer (auto)]), 
        [cegui_with_silly=$enableval], [cegui_with_silly=yes])
    AC_ARG_ENABLE([tga], AC_HELP_STRING([--enable-tga], [Enable image loading via TGA image codec by OpenGL renderer (auto)]), 
        [cegui_with_tga=$enableval], [cegui_with_tga=yes])
    AC_ARG_WITH([default-image-codec], AC_HELP_STRING([--with-default-image-codec[=PARSER]], [Sets the default image codec used by the OpenGL renderer. 
Tipically this will be one of TGAImageCodec, SILLYImageCodec, CoronaImageCodec, FreeImageImageCodec, DevILImageCodec, though you can set it to anything 
to load a custom made image codec module as the default.]), 
    [cegui_default_image_codec=$withval], [cegui_default_image_codec=none])
    AC_PATH_XTRA
    cegui_saved_LIBS="$LIBS"
    LIBS="$X_PRE_LIBS $X_LIBS $X_EXTRA_LIBS"

    AC_SEARCH_LIBS(glInterleavedArrays, MesaGL GL, cegui_found_lib_GL=yes, cegui_found_lib_GL=no)
    AC_SEARCH_LIBS(gluOrtho2D, MesaGLU GLU, cegui_found_lib_GLU=yes,  cegui_found_lib_GLU=no)
    AC_SEARCH_LIBS(glutInit, glut, cegui_found_lib_glut=yes, cegui_found_lib_glut=no)
    OpenGL_CFLAGS="$X_CFLAGS"
    OpenGL_LIBS=$LIBS
    LIBS="$cegui_saved_LIBS"

    dnl decide whether to really build the OpenGL renderer
    if test x$cegui_enable_opengl = xyes && test x$cegui_found_lib_GL = xyes && test x$cegui_found_lib_GLU = xyes; then
        cegui_enable_opengl=yes
    else
        cegui_enable_opengl=no
    fi

    if test x$cegui_enable_opengl = xyes; then
        AC_MSG_NOTICE([OpenGL renderer enabled])

        dnl DevIL 
        if test x$cegui_with_devil = xyes; then
            AC_CHECK_LIB(IL, ilLoadL, [cegui_with_il_lib=yes], [cegui_with_il_lib=no], [])
            AC_CHECK_LIB(ILU, iluFlipImage, [cegui_with_ilu_lib=yes],[cegui_with_ilu_lib=no], [-lIL])
            if test x$cegui_with_il_lib = xyes -a x$cegui_with_ilu_lib = xyes ; then 
                AC_MSG_NOTICE([Image loading via DevIL by OpenGL renderer enabled])
                DevIL_CFLAGS="-DUSE_DEVIL_LIBRARY"
                DevIL_LIBS="-LIL -lILU"
                AC_SUBST(DevIL_CFLAGS)
                AC_SUBST(DevIL_LIBS)
                cegui_with_devil=yes
            else
                AC_MSG_NOTICE([Image loading via DevIL by OpenGL renderer disabled])
                cegui_with_devil=no
            fi
        else            
            AC_MSG_NOTICE([Image loading via DevIL by OpenGL renderer disabled])
            cegui_with_devil=no
        fi

        dnl FreeImage 
        if test x$cegui_with_freeimage = xyes ; then 
            AC_LANG_PUSH(C++)
            AC_SEARCH_LIBS(FreeImage_GetVersion, freeimage, [cegui_with_freeimage_lib=yes], [cegui_with_freeimage_lib=no])
            AC_LANG_POP(C++)
            AC_CHECK_HEADER(FreeImage.h, [cegui_with_freeimage_header=yes], [cegui_with_freeimage_header=no], [])
            if test x$cegui_with_freeimage_lib = xyes -a x$cegui_with_freeimage_header = xyes ; then
                AC_MSG_NOTICE([Image loading via FreeImage by OpenGL renderer enabled])
                FreeImage_CFLAGS="-DUSE_FREEIMAGE_LIBRARY"
                FreeImage_LIBS="-lfreeimage" 
                AC_SUBST(FreeImage_CFLAGS)
                AC_SUBST(FreeImage_LIBS)
                cegui_with_freeimage=yes
             else 
                AC_MSG_NOTICE([Image loading via FreeImage by OpenGL renderer disabled])
                cegui_with_freeimage=no
             fi
        else 
            AC_MSG_NOTICE([Image loading via FreeImage by OpenGL renderer disabled])
        fi 
        dnl Silly 
        if test x$cegui_with_silly = xyes ; then 
            PKG_CHECK_MODULES([SILLY], [SILLY >= 0.1.0], [cegui_with_silly=yes], [cegui_with_silly=no])
            if test x$cegui_with_silly = xyes ; then
               SILLY_CFLAGS="-DUSE_SILLY_LIBRARY $SILLY_CFLAGS"
               AC_SUBST(SILLY_CFLAGS)
               AC_SUBST(SILLY_LIBS)
               AC_MSG_NOTICE([Image loading via SILLY by OpenGL renderer enabled])
            else 
               AC_MSG_NOTICE([Image loading via SILLY by OpenGL renderer disabled])
            fi
        else 
            AC_MSG_NOTICE([Image loading via SILLY by OpenGL renderer disabled])
        fi 

        dnl Corona
        if test x$cegui_with_corona = xyes; then
            if test "x$cegui_corona_prefix" != "x" ; then
                cegui_corona_config=$cegui_corona_prefix/bin/corona-config
            fi 
            if test "x$prefix" != xNONE ; then 
                PATH="$prefix/bin:$prefix/usr/bin:$PATH"
            fi
            AC_PATH_PROG(cegui_corona_config, corona-config, no, [PATH])
            AC_MSG_CHECKING([for corona - version >= 1.2.0])
            cegui_with_corona=no
            if test "$cegui_corona_config" = "no" ; then
                cegui_with_corona=no
                AC_MSG_RESULT([no])
            else 
                CORONA_CFLAGS="`$cegui_corona_config --cflags`"
                CORONA_LIBS="`$cegui_corona_config --libs`"
                CORONA_VERSION="`$cegui_corona_config --version`"
                dnl if test "$CORONA_VERSION" >= "1.2.0" ; then 
                    AC_MSG_RESULT([yes])
                    cegui_with_corona=yes
                    Corona_CFLAGS="$CORONA_CFLAGS -DUSE_CORONA_LIBRARY"
                    Corona_LIBS="$CORONA_LIBS" 
                    AC_SUBST(Corona_CFLAGS)
                    AC_SUBST(Corona_LIBS)
                    AC_MSG_NOTICE([Image loading via Corona by OpenGL renderer enabled])
                dnl else 
                dnl    AC_MSG_RESULT([no])
                dnl     cegui_with_corona=no
                dnl     AC_MSG_NOTICE([Image loading via Corona by OpenGL renderer disabled])
                dnl fi
            fi
        else 
            AC_MSG_NOTICE([Image loading via Corona by OpenGL renderer disabled])
        fi 

        if test x$cegui_found_lib_glut = xyes; then
            cegui_samples_use_opengl=yes
            AC_DEFINE(CEGUI_SAMPLES_USE_OPENGL, [], [Define to have the OpenGL CEGUI renderer available in the samples (requires glut)])
            AC_MSG_NOTICE([Use of OpenGL in Samples is enabled])
        else
            cegui_samples_use_opengl=no
            AC_MSG_NOTICE([Use of OpenGL in Samples is disabled - Did not find GLUT library.])
        fi

    else
        cegui_with_freeimage=no
        cegui_with_devil=no
        cegui_with_corona=no
        cegui_with_tga=no
        cegui_with_silly=no
        AC_MSG_NOTICE([OpenGL renderer disabled])
    fi

    if test x$cegui_enable_opengl = xyes; then
        if test x$cegui_default_image_codec = xnone ; then 
            if test x$cegui_with_devil = xyes ; then
                cegui_default_image_codec=DevILImageCodec
            else 
                if test x$cegui_with_freeimage = xyes ; then 
                    cegui_default_image_codec=FreeImageImageCodec
                else 
                    if test x$cegui_with_silly = xyes ; then 
                        cegui_default_image_codec=SILLYImageCodec
                    else 
                        if test x$cegui_with_corona = xyes ; then
                            cegui_default_image_codec=CoronaImageCodec
                        else 
                            if test x$cegui_with_tga = xyes ; then 
                                cegui_default_image_codec=TGAImageCodec
                            else 
                                AC_MSG_ERROR([None of the ImageCodec are going to be build - unable to continue. Either enable an image codec or set a custom default.])
                            fi 
                        fi
                    fi
                fi
            fi
        fi
    fi
    dnl define macro for the class of the default image codec  to be used
    AC_DEFINE_UNQUOTED(CEGUI_DEFAULT_IMAGE_CODEC, $cegui_default_image_codec, [Set this to the default ImageCodec to be used (CoronaImageCodec, DevILImageCodec FreeImageImageCode, SILLYImageCodec, TGAImageCodec).])
    AC_MSG_NOTICE([Default ImageCodec will be: $cegui_default_image_codec])
        

    AM_CONDITIONAL([BUILD_OPENGL_RENDERER], [test x$cegui_enable_opengl = xyes])
    AM_CONDITIONAL([CEGUI_SAMPLES_USE_OPENGL], [test x$cegui_samples_use_opengl = xyes])
    AM_CONDITIONAL([CEGUI_BUILD_DEVIL_IMAGE_CODEC], [test x$cegui_with_devil = xyes])
    AM_CONDITIONAL([CEGUI_BUILD_CORONA_IMAGE_CODEC], [test x$cegui_with_corona = xyes])
    AM_CONDITIONAL([CEGUI_BUILD_SILLY_IMAGE_CODEC], [test x$cegui_with_silly = xyes])
    AM_CONDITIONAL([CEGUI_BUILD_FREE_IMAGE_IMAGE_CODEC], [test x$cegui_with_freeimage = xyes])
    AM_CONDITIONAL([CEGUI_BUILD_TGA_IMAGE_CODEC], [test x$cegui_with_tga = xyes])
    AC_SUBST(OpenGL_CFLAGS)
    AC_SUBST(OpenGL_LIBS)
])

# CEGUI_CHECK_XERCES(variable, [action-if-found], [action-if-not-found])
# checks for xerces-c++ headers and libs in some common places.
AC_DEFUN([CEGUI_CHECK_XERCES],[
    AC_ARG_WITH([xerces-incdir], AC_HELP_STRING([--with-xerces-incdir=DIR], [Optionally specifies location of the xerces-c++ includes]),
        [cegui_xerces_incdir=$withval],[cegui_xerces_incdir=[.]])
    AC_ARG_WITH([xerces-libdir], AC_HELP_STRING([--with-xerces-libdir=DIR], [Optionally specifies location of the xerces-c++ library]),
        [cegui_xerces_libdir=$withval],[cegui_xerces_libdir=[.]])
    cegui_lib_paths="$cegui_xerces_libdir"
    cegui_inc_paths="$cegui_xerces_incdir /usr/local/include/xerces /usr/include/xerces /usr/local/include /usr/include"

    $1_CFLAGS=
    $1_LIBS=

    cegui_saved_CFLAGS="$CPPFLAGS"
    cegui_saved_LIBS="$LIBS"

    for cegui_path in $cegui_inc_paths; do
        ifelse($cegui_path, [.], CPPFLAGS="$cegui_saved_CFLAGS", CPPFLAGS="-I$cegui_path $cegui_saved_CFLAGS")
        AC_PREPROC_IFELSE(
            [#include <xercesc/sax2/DefaultHandler.hpp>],
            [cegui_xerces_h_found=yes; cegui_xerces_flags="$cegui_path"; break],
            [cegui_xerces_h_found=no])
    done

    for cegui_path in $cegui_lib_paths; do
        ifelse($cegui_path, [.], LIBS="$cegui_saved_LIBS", LIBS="-L$cegui_path $cegui_saved_LIBS")
        AC_CHECK_LIB([xerces-c], [main],
            [cegui_xerces_l_found=yes; cegui_xerces_libs="$cegui_path"; break],
            [cegui_xerces_l_found=no; unset ac_cv_lib_xerces_c_main], -lpthread)
    done

    CPPFLAGS="$cegui_saved_CFLAGS"
    LIBS="$cegui_saved_LIBS"

    if test x$cegui_xerces_h_found = xyes && test x$cegui_xerces_l_found = xyes; then
        if test x$cegui_xerces_flags != x.; then
            $1_CFLAGS="-I$cegui_xerces_flags"
        fi
        if test x$cegui_xerces_libs = x.; then
            $1_LIBS="-lxerces-c"
        else
            $1_LIBS="-L$cegui_xerces_libs -lxerces-c"
        fi

        ifelse([$2], [], :, [$2])
    else
        ifelse([$3], [], :, [$3])
    fi
])

# CEGUI_CHECK_EXPAT(variable, [action-if-found], [action-if-not-found])
# checks for expat headers and libs in some common places.
AC_DEFUN([CEGUI_CHECK_EXPAT],[
    AC_ARG_WITH([expat-incdir], AC_HELP_STRING([--with-expat-incdir=DIR], [Optionally specifies location of the expat-c++ includes]),
        [cegui_expat_incdir=$withval],[cegui_expat_incdir=[.]])
    AC_ARG_WITH([expat-libdir], AC_HELP_STRING([--with-expat-libdir=DIR], [Optionally specifies location of the expat-c++ library]),
        [cegui_expat_libdir=$withval],[cegui_expat_libdir=[.]])
    cegui_lib_paths="$cegui_expat_libdir"
    cegui_inc_paths="$cegui_expat_incdir /usr/local/include/expat /usr/include/expat /usr/local/include /usr/include"

    $1_CFLAGS=
    $1_LIBS=

    cegui_saved_CFLAGS="$CPPFLAGS"
    cegui_saved_LIBS="$LIBS"

    for cegui_path in $cegui_inc_paths; do
        ifelse($cegui_path, [.], CPPFLAGS="$cegui_saved_CFLAGS", CPPFLAGS="-I$cegui_path $cegui_saved_CFLAGS")
        AC_PREPROC_IFELSE(
            [#include <expat.h>],
            [cegui_expat_h_found=yes; cegui_expat_flags="$cegui_path"; break],
            [cegui_expat_h_found=no])
    done

    for cegui_path in $cegui_lib_paths; do
        ifelse($cegui_path, [.], LIBS="$cegui_saved_LIBS", LIBS="-L$cegui_path $cegui_saved_LIBS")
        AC_CHECK_LIB([expat], [XML_ParserCreate],
            [cegui_expat_l_found=yes; cegui_expat_libs="$cegui_path"; break],
            [cegui_expat_l_found=no; unset ac_cv_lib_expat-c_main])
    done

    CPPFLAGS="$cegui_saved_CFLAGS"
    LIBS="$cegui_saved_LIBS"

    if test x$cegui_expat_h_found = xyes && test x$cegui_expat_l_found = xyes; then
        if test x$cegui_expat_flags != x.; then
            $1_CFLAGS="-I$cegui_expat_flags"
        fi
        if test x$cegui_expat_libs = x.; then
            $1_LIBS="-lexpat"
        else
            $1_LIBS="-L$cegui_expat_libs -lexpat"
        fi

        ifelse([$2], [], :, [$2])
    else
        ifelse([$3], [], :, [$3])
    fi
])

AC_DEFUN([CEGUI_CHECK_LUA],[
    PKG_CHECK_MODULES(Lua, lua >= 5.0 lua < 5.1, [cegui_found_lua=yes], [cegui_found_lua=no])

    AC_ARG_ENABLE([lua-module], AC_HELP_STRING([--disable-lua-module], [Disables building of the Lua scripting module.]),
                [cegui_with_lua=$enableval], [cegui_with_lua=yes])
    AC_ARG_ENABLE([toluacegui], AC_HELP_STRING([--enable-toluacegui], [Enables building of our custom tolua++ based binding generator tool, tolua++cegui.]),
                [cegui_build_toluatool=$enableval], [cegui_build_toluatool=no])

    if test x$cegui_found_lua = xyes && test x$cegui_with_lua = xyes; then
        cegui_with_lua=yes
    else
        cegui_with_lua=no
    fi

    if test x$cegui_with_lua = xyes; then
        AC_MSG_NOTICE([Building of Lua scripting module is enabled])
    else
        AC_MSG_NOTICE([Building of Lua scripting module is disabled])
    fi

    if test x$cegui_build_toluatool = xyes; then
        AC_MSG_NOTICE([Building of tolua++cegui generator tool is enabled.])
    else
        AC_MSG_NOTICE([Building of tolua++cegui generator tool is disabled.])
    fi

    AM_CONDITIONAL([CEGUI_BUILD_LUA_MODULE], [test x$cegui_with_lua = xyes])
    AM_CONDITIONAL([CEGUI_BUILD_TOLUACEGUI], [test x$cegui_build_toluatool = xyes])
    AC_SUBST(Lua_CFLAGS)
    AC_SUBST(Lua_LIBS)
])
