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
        if test x$cegui_samples_use_ogre = xyes || test x$cegui_samples_use_irrlicht = xyes || test x$cegui_samples_use_opengl = xyes || test x$cegui_samples_use_directfb; then
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
    CEGUI_CHECK_RAPIDXML(rapidxml, [cegui_found_rapidxml=yes], [cegui_found_rapidxml=no])

    dnl Make some decisions about how to go about using TinyXML
    AC_ARG_ENABLE([external-tinyxml], AC_HELP_STRING([--disable-external-tinyxml], [Disables the use of any external tinyxml library, forcing the use of the version that accompanies CEGUI.]),
            [cegui_use_external_tinyxml=$enableval], [cegui_use_external_tinyxml=yes])

    if test x$cegui_use_external_tinyxml = xyes; then
        CEGUI_CHECK_TINYXML([tinyxml], [cegui_found_tinyxml=yes], [cegui_found_tinyxml=no])
        dnl Discover what API version we have
        AC_LANG_PUSH([C++])
        AC_COMPILE_IFELSE([AC_LANG_SOURCE([
            [#include <tinyxml.h>
             int main() {
                int i = TiXmlElement::TINYXML_ELEMENT;
                return 0;
             }]])],
            [AC_DEFINE(CEGUI_TINYXML_HAS_2_6_API, 1, [Set this if your tiny xml version is 2.6 or higher])],
            [])
        AC_LANG_POP([C++])
    else
        cegui_found_tinyxml=no
    fi

    dnl Find out what user disabled as far as parsers go
    AC_ARG_ENABLE([xerces-c], AC_HELP_STRING([--disable-xerces-c], [Disables building of the Xerces-C++ XML parser module.]),
                [cegui_with_xerces=$enableval], [cegui_with_xerces=yes])

    AC_ARG_ENABLE([libxml], AC_HELP_STRING([--disable-libxml], [Disables building of the libxml XML parser module.]),
                [cegui_with_libxml=$enableval], [cegui_with_libxml=yes])

    AC_ARG_ENABLE([expat], AC_HELP_STRING([--disable-expat], [Disables building of the expat XML parser module.]),
                [cegui_with_expat=$enableval], [cegui_with_expat=yes])

    AC_ARG_ENABLE([rapidxml], AC_HELP_STRING([--disable-rapidxml], [Disables building of the RapidXML parser module.]),
                [cegui_with_rapidxml=$enableval], [cegui_with_rapidxml=yes])

    AC_ARG_ENABLE([tinyxml], AC_HELP_STRING([--disable-tinyxml], [Disables building of the tinyXML parser module.]),
                [cegui_with_tinyxml=$enableval], [cegui_with_tinyxml=yes])

    dnl Find out which paser user wants as a default
    AC_ARG_WITH([default-xml-parser], AC_HELP_STRING([--with-default-xml-parser[=PARSER]], [Sets the default XML parser module.
Typically this will be one of XercesParser, ExpatParser, LibxmlParser, RapidXMLParser or TinyXMLParser, though you can set it to anything to
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
    if test x$cegui_found_rapidxml = xyes && test x$cegui_with_rapidxml = xyes; then
        cegui_with_rapidxml=yes
    else
        cegui_with_rapidxml=no
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
    if test x$cegui_default_parser = xRapidXMLParser && test x$cegui_with_rapidxml = xno; then
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
                    if test x$cegui_with_rapidxml = xyes; then
                        cegui_default_parser=RapidXMLParser
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
    fi

    dnl define macro for the class of the default xml parser to be used
    AC_DEFINE_UNQUOTED(CEGUI_DEFAULT_XMLPARSER, $cegui_default_parser, [Set this to the default XMLParser to be used (XercesParser, ExpatParser, LibxmlParser or TinyXMLParser).])
    AC_MSG_NOTICE([Default XML Parser will be: $cegui_default_parser])

    dnl automake conditionals
    AM_CONDITIONAL([BUILD_XERCES_PARSER], [test x$cegui_with_xerces = xyes])
    AM_CONDITIONAL([BUILD_LIBXML_PARSER], [test x$cegui_with_libxml = xyes])
    AM_CONDITIONAL([BUILD_EXPAT_PARSER], [test x$cegui_with_expat = xyes])
    AM_CONDITIONAL([BUILD_RAPIDXML_PARSER], [test x$cegui_with_rapidxml = xyes])
    AM_CONDITIONAL([BUILD_TINYXML_PARSER], [test x$cegui_with_tinyxml = xyes])
    AM_CONDITIONAL([BUILD_USING_INTERNAL_TINYXML], [test x$cegui_with_tinyxml = xyes && test x$cegui_found_tinyxml = xno])

    dnl create a define for the main tinyxml header file
    if test x$cegui_with_tinyxml = xyes && test x$cegui_found_tinyxml = xno; then
        AC_DEFINE(CEGUI_TINYXML_H, ["ceguitinyxml/tinyxml.h"], [Set this to the name of the tinyxml.h file])
        AC_DEFINE(CEGUI_TINYXML_NAMESPACE, [CEGUITinyXML], [Set this to the name of the tinyxml namespace])
    else
        AC_DEFINE(CEGUI_TINYXML_H, [<tinyxml.h>], [Set this to the name of the tinyxml.h file])
        AC_DEFINE(CEGUI_TINYXML_NAMESPACE, [], [Set this to the name of the tinyxml namespace])
    fi

    AC_SUBST(xerces_CFLAGS)
    AC_SUBST(xerces_LIBS)
    AC_SUBST(expat_CFLAGS)
    AC_SUBST(expat_LIBS)
    AC_SUBST(tinyxml_CFLAGS)
    AC_SUBST(tinyxml_LIBS)
    AC_SUBST(rapidxml_CFLAGS)
])

AC_DEFUN([CEGUI_CHECK_IMAGE_CODECS], [
    AC_ARG_ENABLE([corona], AC_HELP_STRING([--enable-corona], [Enable Corona based ImageCodec module (auto)]),
        [cegui_with_corona=$enableval], [cegui_with_corona=yes])
    AC_ARG_WITH(corona-prefix, AC_HELP_STRING([--with-corona-prefix], [Prefix where corona is installed (optional)]),
        [cegui_corona_prefix="$withval"], [cegui_corona_prefix=""])
    AC_ARG_ENABLE([devil], AC_HELP_STRING([--enable-devil], [Enable DevIL based ImageCodec module (auto)]),
        [cegui_with_devil=$enableval], [cegui_with_devil=yes])
    AC_ARG_ENABLE([freeimage], AC_HELP_STRING([--enable-freeimage], [Enable FreeImage based ImageCodec module (auto)]),
        [cegui_with_freeimage=$enableval], [cegui_with_freeimage=yes])
    AC_ARG_ENABLE([silly], AC_HELP_STRING([--enable-silly], [Enable SILLY based ImageCodec module (auto)]),
        [cegui_with_silly=$enableval], [cegui_with_silly=yes])
    AC_ARG_ENABLE([tga], AC_HELP_STRING([--enable-tga], [Enable basic TGA loading ImageCodec module (auto)]),
        [cegui_with_tga=$enableval], [cegui_with_tga=yes])
    AC_ARG_ENABLE([stb], AC_HELP_STRING([--enable-stb], [Enable STB based ImageCodec module (auto)]),
        [cegui_with_stb=$enableval], [cegui_with_stb=yes])
    AC_ARG_WITH([default-image-codec], AC_HELP_STRING([--with-default-image-codec[=CODEC]], [Sets the default image codec.
Typically this will be one of TGAImageCodec, SILLYImageCodec, CoronaImageCodec, FreeImageImageCodec, DevILImageCodec or STBImageCodec,
though you can set it to anything to load a custom made image codec module as the default.]),
    [cegui_default_image_codec=$withval], [cegui_default_image_codec=none])

    dnl DevIL
    if test x$cegui_with_devil = xyes; then
        cegui_saved_LIBS="$LIBS"
        cegui_saved_CFLAGS="$CFLAGS"

        AC_CHECK_LIB(IL, ilLoadL, [cegui_with_il_lib=yes], [cegui_with_il_lib=no], [])
        AC_CHECK_LIB(ILU, iluFlipImage, [cegui_with_ilu_lib=yes],[cegui_with_ilu_lib=no], [-lIL])
        if test x$cegui_with_il_lib = xyes -a x$cegui_with_ilu_lib = xyes ; then
            AC_MSG_NOTICE([DevIL based ImageCodec module enabled.])
            DevIL_CFLAGS="-DUSE_DEVIL_LIBRARY"
            DevIL_LIBS="-lIL -lILU"
            AC_SUBST(DevIL_CFLAGS)
            AC_SUBST(DevIL_LIBS)
            cegui_with_devil=yes
        else
            AC_MSG_NOTICE([DevIL based ImageCodec module disabled.])
            cegui_with_devil=no
        fi

        LIBS="$cegui_saved_LIBS"
        CFLAGS="$cegui_saved_CFLAGS"
    else
        AC_MSG_NOTICE([DevIL based ImageCodec module disabled.])
        cegui_with_devil=no
    fi

    dnl FreeImage
    if test x$cegui_with_freeimage = xyes ; then
        cegui_saved_LIBS="$LIBS"
        cegui_saved_CFLAGS="$CFLAGS"

        AC_LANG_PUSH(C++)
        AC_SEARCH_LIBS(FreeImage_GetVersion, freeimage, [cegui_with_freeimage_lib=yes], [cegui_with_freeimage_lib=no])
        AC_LANG_POP(C++)
        AC_CHECK_HEADER(FreeImage.h, [cegui_with_freeimage_header=yes], [cegui_with_freeimage_header=no], [])
        if test x$cegui_with_freeimage_lib = xyes -a x$cegui_with_freeimage_header = xyes ; then
            AC_MSG_NOTICE([FreeImage based ImageCodec module enabled.])
            FreeImage_CFLAGS="-DUSE_FREEIMAGE_LIBRARY"
            FreeImage_LIBS="-lfreeimage"
            AC_SUBST(FreeImage_CFLAGS)
            AC_SUBST(FreeImage_LIBS)
            cegui_with_freeimage=yes
            else
            AC_MSG_NOTICE([FreeImage based ImageCodec module disabled.])
            cegui_with_freeimage=no
        fi

        LIBS="$cegui_saved_LIBS"
        CFLAGS="$cegui_saved_CFLAGS"
    else
        AC_MSG_NOTICE([FreeImage based ImageCodec module disabled.])
    fi

    dnl Silly
    if test x$cegui_with_silly = xyes ; then
        PKG_CHECK_MODULES([SILLY], [SILLY >= 0.1.0], [cegui_with_silly=yes], [cegui_with_silly=no])
        if test x$cegui_with_silly = xyes ; then
            SILLY_CFLAGS="-DUSE_SILLY_LIBRARY $SILLY_CFLAGS"
            AC_SUBST(SILLY_CFLAGS)
            AC_SUBST(SILLY_LIBS)
            AC_MSG_NOTICE([SILLY based ImageCodec module enabled.])
        else
            AC_MSG_NOTICE([SILLY based ImageCodec module disabled.])
        fi
    else
        AC_MSG_NOTICE([SILLY based ImageCodec module disabled.])
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
                AC_MSG_NOTICE([Corona based ImageCodec module enabled.])
            dnl else
            dnl    AC_MSG_RESULT([no])
            dnl     cegui_with_corona=no
            dnl     AC_MSG_NOTICE([Image loading via Corona by OpenGL renderer disabled])
            dnl fi
        fi
    else
        AC_MSG_NOTICE([Corona based ImageCodec module disabled.])
    fi

    dnl Decide which codec is to be the default
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
                            if test x$cegui_with_stb = xyes ; then
                                cegui_default_image_codec=STBImageCodec
                            else
                                AC_MSG_ERROR([None of the ImageCodec are going to be built - unable to continue. Either enable an image codec or set a custom default.])
                            fi
                        fi
                    fi
                fi
            fi
        fi
    fi

    dnl define macro for the class of the default image codec  to be used
    AC_DEFINE_UNQUOTED(CEGUI_DEFAULT_IMAGE_CODEC, $cegui_default_image_codec, [Set this to the default ImageCodec to be used (CoronaImageCodec, DevILImageCodec FreeImageImageCode, SILLYImageCodec, TGAImageCodec, STBImageCodec).])
    AC_MSG_NOTICE([Default ImageCodec will be: $cegui_default_image_codec])

    AM_CONDITIONAL([CEGUI_BUILD_DEVIL_IMAGE_CODEC], [test x$cegui_with_devil = xyes])
    AM_CONDITIONAL([CEGUI_BUILD_CORONA_IMAGE_CODEC], [test x$cegui_with_corona = xyes])
    AM_CONDITIONAL([CEGUI_BUILD_SILLY_IMAGE_CODEC], [test x$cegui_with_silly = xyes])
    AM_CONDITIONAL([CEGUI_BUILD_FREE_IMAGE_IMAGE_CODEC], [test x$cegui_with_freeimage = xyes])
    AM_CONDITIONAL([CEGUI_BUILD_TGA_IMAGE_CODEC], [test x$cegui_with_tga = xyes])
    AM_CONDITIONAL([CEGUI_BUILD_STB_IMAGE_CODEC], [test x$cegui_with_stb = xyes])
])

AC_DEFUN([CEGUI_ENABLE_NULL_RENDERER], [
    AC_ARG_ENABLE([null-renderer], AC_HELP_STRING([--enable-null-renderer],
                  [Enable building the Null renderer module]),
                  [cegui_enable_null=$enableval], [cegui_enable_null=no])

    AM_CONDITIONAL([BUILD_NULL_RENDERER], [test x$cegui_enable_null = xyes])
])

AC_DEFUN([CEGUI_ENABLE_OGRE_RENDERER], [
    AC_ARG_ENABLE([ogre-renderer], AC_HELP_STRING([--disable-ogre-renderer],
                  [Disable building the Ogre renderer module]),
                  [cegui_enable_ogre=$enableval], [cegui_enable_ogre=yes])

    PKG_CHECK_MODULES(Ogre, OGRE >= 1.6.0, [cegui_found_ogre=yes], [cegui_found_ogre=no])
    PKG_CHECK_MODULES(OIS, OIS >= 1.0.0, [cegui_found_ois=yes],[cegui_found_ois=no])

    dnl decide if we will actually build the Ogre Renderer
    if test x$cegui_enable_ogre = xyes && test x$cegui_found_ogre = xyes; then
        cegui_enable_ogre=yes
    else
        cegui_enable_ogre=no
    fi

    dnl see if we should use Ogre in the samples
    if test x$cegui_enable_ogre = xyes; then
        if test x$cegui_found_ois = xno; then
            cegui_samples_use_ogre=no
        else
            cegui_samples_use_ogre=yes
            AC_DEFINE(CEGUI_SAMPLES_USE_OGRE, [],
                      [Define to have the Ogre renderer available in the samples])
            AC_MSG_NOTICE([Use of Ogre3D in Samples is enabled])

            AC_MSG_NOTICE([Trying to determine OIS APIs to use])
            cegui_saved_CFLAGS="$CPPFLAGS"
            CPPFLAGS="$CPPFLAGS $OIS_CFLAGS"
            AC_LANG_PUSH(C++)
            AC_COMPILE_IFELSE([AC_LANG_SOURCE([
            [
                #include <OIS.h>
                OIS::InputManager* im = 0;
                int main(int argc, char** argv) {im->numKeyboards(); return 0;}
            ]])],
            [AC_DEFINE([CEGUI_OLD_OIS_API],[1],[Define if your OIS uses the older numKeyboards like APIs rather than the newer getNumberOfDevices API])
            ])
            AC_LANG_POP(C++)
            CPPFLAGS="$cegui_saved_CFLAGS"
        fi
    else
        cegui_samples_use_ogre=no
        AC_MSG_NOTICE([Ogre renderer disabled])
    fi

    AM_CONDITIONAL([BUILD_OGRE_RENDERER], [test x$cegui_enable_ogre = xyes])
    AM_CONDITIONAL([CEGUI_SAMPLES_USE_OGRE], [test x$cegui_samples_use_ogre = xyes])
    AC_SUBST(Ogre_CFLAGS)
    AC_SUBST(Ogre_LIBS)
    AC_SUBST(OIS_CFLAGS)
    AC_SUBST(OIS_LIBS)
])

AC_DEFUN([CEGUI_ENABLE_DIRECTFB_RENDERER], [
    PKG_CHECK_MODULES(directfb, directfb >= 1.2.0, [cegui_found_directfb=yes], [cegui_found_directfb=no])
    AC_ARG_ENABLE([directfb-renderer],
                  AC_HELP_STRING([--enable-directfb-renderer],
                                 [Enable the DirectFB renderer.  This renderer
                                  is currently broken and incomplete; it should
                                  only be enabled for development and/or testing
                                  purposes.]),
        [cegui_enable_directfb=$enableval],[cegui_enable_directfb=no])

    dnl decide if we will actually build the DirectFB Renderer
    if test x$cegui_enable_directfb = xyes && test x$cegui_found_directfb = xyes; then
        cegui_enable_directfb=yes
    else
        cegui_enable_directfb=no
    fi

    if test x$cegui_enable_directfb = xyes; then
        cegui_samples_use_directfb=yes
        AC_DEFINE(CEGUI_SAMPLES_USE_DIRECTFB, [], [Define to have the DirectFB CEGUI renderer available in the samples])
        AC_MSG_NOTICE([Use of DirectFB in Samples is enabled])
    else
        cegui_samples_use_directfb=no
        AC_MSG_NOTICE([DirectFB renderer disabled])
    fi

    AM_CONDITIONAL([BUILD_DIRECTFB_RENDERER], [test x$cegui_enable_directfb = xyes])
    AM_CONDITIONAL([CEGUI_SAMPLES_USE_DIRECTFB], [test x$cegui_samples_use_directfb = xyes])
    AC_SUBST(directfb_CFLAGS)
    AC_SUBST(directfb_LIBS)
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
        AC_PREPROC_IFELSE([AC_LANG_SOURCE([[#include <irrlicht.h>]])], [cegui_irr_h_found=yes; cegui_irr_flags="$cegui_path"; break],[cegui_irr_h_found=no])
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

        dnl ** Test for Irr SDK 1.4 or 1.5.x **
        AC_COMPILE_IFELSE([AC_LANG_SOURCE([
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
        ]])],
        [AC_DEFINE([CEGUI_IRR_SDK_VERSION],[14],[Defines irrlicht SDK version.  14 is 1.4 or 1.5.x and 16 is 1.6 or later.])
        gotirrsdkver=yes])

        dnl ** Test for Irr SDK 1.6 (and above?) **
        if test x$gotirrsdkver = xno; then
        AC_COMPILE_IFELSE([AC_LANG_SOURCE([
        [
            #include <irrlicht.h>
            class test : public irr::io::IReadFile
            {
            public:
                bool seek(long finalPos, bool relativeMovement = false) {return false; }
                irr::s32 read(void* buffer, irr::u32 sizeToRead) {return 0;}
                long getSize() const {return 0;}
                long getPos() const {return 0;}
                const irr::io::path& getFileName() const {return filename;}
            protected:
                irr::io::path filename;
            };
            int main(int argc, char** argv) { test x; return 0; }
        ]])],
        [AC_DEFINE([CEGUI_IRR_SDK_VERSION],[16],[Defines irrlicht SDK version.  14 is 1.4 or 1.5.x and 16 is 1.6 or later.])
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

    cegui_saved_LIBS="$LIBS"
    cegui_saved_CFLAGS="$CFLAGS"

    dnl detect OpenGL libs (done differently on mingw32 than on other systems)
    dnl -- On mingw32, checking for functions does not seem to work so well, so we just check for existance and hope for the best :-/
    case $host_os in
    *mingw32* )
        AC_CHECK_LIB([opengl32], [main], [cegui_found_lib_GL=yes; LIBS="-lopengl32 $LIBS"], [cegui_found_lib_GL=no])
        AC_CHECK_LIB([glu32], [main], [cegui_found_lib_GLU=yes; LIBS="-lglu32 $LIBS"], [cegui_found_lib_GLU=no])

        dnl Check for glut header
        AC_CHECK_HEADER([glut.h], [cegui_found_glut_h=yes],
            [AC_CHECK_HEADER([GL/glut.h], [cegui_found_glut_h=yes], [cegui_found_glut_h=no])])

        if test x$cegui_found_glut_h = xyes; then
            dnl Check for some glut variants.  Done like this because AC_SEARCH_LIBS did not work at all here
            AC_CHECK_LIB([freeglut], [main], [cegui_found_lib_glut=yes; LIBS="-lfreeglut $LIBS"], [cegui_found_lib_glut=no])
            if test x$cegui_found_lib_glut = xno; then
                AC_CHECK_LIB([glut32], [main], [cegui_found_lib_glut=yes; LIBS="-lglut32 $LIBS"], [cegui_found_lib_glut=no])
                if test x$cegui_found_lib_glut = xno; then
                    AC_CHECK_LIB([glut], [main], [cegui_found_lib_glut=yes; LIBS="-lglut $LIBS"], [cegui_found_lib_glut=no])
                fi
            fi
        else
            cegui_found_lib_glut=no
        fi

        OpenGL_CFLAGS=""
        OpenGL_LIBS=$LIBS
        echo $LIBS
        ;;
    * )
        AC_PATH_XTRA
        LIBS="$X_PRE_LIBS $X_LIBS $X_EXTRA_LIBS"

        AC_SEARCH_LIBS(glInterleavedArrays, MesaGL GL, cegui_found_lib_GL=yes, cegui_found_lib_GL=no)
        AC_SEARCH_LIBS(gluOrtho2D, MesaGLU GLU, cegui_found_lib_GLU=yes,  cegui_found_lib_GLU=no)
        AC_SEARCH_LIBS(glutInit, glut, cegui_found_lib_glut=yes, cegui_found_lib_glut=no)
        OpenGL_CFLAGS="$X_CFLAGS"
        OpenGL_LIBS="$LIBS"
        ;;
    esac

    dnl Deal with possibility / option for external GLEW lib
    AC_ARG_ENABLE([external-glew], AC_HELP_STRING([--disable-external-glew], [Disables the use of any external GLEW library, forcing the use of the version that accompanies CEGUI.]),
        [cegui_use_external_glew=$enableval], [cegui_use_external_glew=yes])
    if test x$cegui_use_external_glew = xyes; then
        CEGUI_CHECK_GLEW([GLEW],
            [cegui_found_glew=yes; OpenGL_CFLAGS="$OpenGL_CFLAGS $GLEW_CFLAGS"; OpenGL_LIBS="$OpenGL_LIBS $GLEW_LIBS"],
            [cegui_found_glew=no])
    else
        cegui_found_glew=no
    fi

    LIBS="$cegui_saved_LIBS"
    CFLAGS="$cegui_saved_CFLAGS"

    dnl decide whether to really build the OpenGL renderer
    if test x$cegui_enable_opengl = xyes && test x$cegui_found_lib_GL = xyes && test x$cegui_found_lib_GLU = xyes; then
        cegui_enable_opengl=yes
    else
        cegui_enable_opengl=no
    fi

    if test x$cegui_enable_opengl = xyes; then
        AC_MSG_NOTICE([OpenGL renderer enabled])

        if test x$cegui_found_lib_glut = xyes; then
            cegui_samples_use_opengl=yes
            AC_DEFINE(CEGUI_SAMPLES_USE_OPENGL, [], [Define to have the OpenGL CEGUI renderer available in the samples (requires glut)])
            AC_MSG_NOTICE([Use of OpenGL in Samples is enabled])
        else
            cegui_samples_use_opengl=no
            AC_MSG_NOTICE([Use of OpenGL in Samples is disabled - Did not find GLUT library.])
        fi

    else
        AC_MSG_NOTICE([OpenGL renderer disabled])
    fi

    AM_CONDITIONAL([BUILD_OPENGL_RENDERER], [test x$cegui_enable_opengl = xyes])
    AM_CONDITIONAL([CEGUI_SAMPLES_USE_OPENGL], [test x$cegui_samples_use_opengl = xyes])
    AM_CONDITIONAL([BUILD_USING_INTERNAL_GLEW], [test x$cegui_found_glew = xno])

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
        AC_PREPROC_IFELSE([AC_LANG_SOURCE([
            [#include <xercesc/sax2/DefaultHandler.hpp>]])],
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
        AC_PREPROC_IFELSE([AC_LANG_SOURCE([
            [#include <expat.h>]])],
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

# CEGUI_CHECK_RAPIDXML(variable, [action-if-found], [action-if-not-found])
# checks for RapidXML headers in some common places.
AC_DEFUN([CEGUI_CHECK_RAPIDXML],[
    AC_ARG_WITH([rapidxml-incdir], AC_HELP_STRING([--with-rapidxml-incdir=DIR], [Optionally specifies location of the RapidXML header files.]),
        [cegui_rapidxml_incdir=$withval],[cegui_rapidxml_incdir=[.]])
    cegui_inc_paths="$cegui_rapidxml_incdir /usr/local/include/rapidxml /usr/include/rapidxml /usr/local/include /usr/include"

    AC_LANG_PUSH([C++])

    $1_CFLAGS=

    cegui_saved_CFLAGS="$CPPFLAGS"

    for cegui_path in $cegui_inc_paths; do
        ifelse($cegui_path, [.], CPPFLAGS="$cegui_saved_CFLAGS", CPPFLAGS="-I$cegui_path $cegui_saved_CFLAGS")
        AC_PREPROC_IFELSE([AC_LANG_SOURCE([
            [#include <rapidxml.hpp>]])],
            [cegui_rapidxml_h_found=yes; cegui_rapidxml_flags="$cegui_path"; break],
            [cegui_rapidxml_h_found=no])
    done

    CPPFLAGS="$cegui_saved_CFLAGS"

    if test x$cegui_rapidxml_h_found = xyes; then
        if test x$cegui_rapidxml_flags != x.; then
            $1_CFLAGS="-I$cegui_rapidxml_flags"
        fi

        ifelse([$2], [], :, [$2])
    else
        ifelse([$3], [], :, [$3])
    fi

    AC_LANG_POP([C++])
])

AC_DEFUN([CEGUI_CHECK_LUA],[
    PKG_CHECK_MODULES(Lua, lua >= 5.1,
                      [cegui_found_lua=yes; Lua_CFLAGS="$Lua_CFLAGS -DCEGUI_LUA_VER=51"],
                      [cegui_found_lua=no])

    dnl If that did not work, try again with alternate names for the packages (as used on various distros)
    if test x$cegui_found_lua = xno; then
        PKG_CHECK_MODULES(Lua, lua5.2,
                        [cegui_found_lua=yes; Lua_CFLAGS="$Lua_CFLAGS -DCEGUI_LUA_VER=52"],
                        [cegui_found_lua=no])
    fi
    if test x$cegui_found_lua = xno; then
        PKG_CHECK_MODULES(Lua, lua5.1,
                        [cegui_found_lua=yes; Lua_CFLAGS="$Lua_CFLAGS -DCEGUI_LUA_VER=51"],
                        [cegui_found_lua=no])
    fi

    AC_ARG_ENABLE([lua-module], AC_HELP_STRING([--disable-lua-module], [Disables building of the Lua scripting module.]),
                [cegui_with_lua=$enableval], [cegui_with_lua=yes])
    AC_ARG_ENABLE([toluacegui], AC_HELP_STRING([--enable-toluacegui], [Enables building of our custom tolua++ based binding generator tool, tolua++cegui.]),
                [cegui_build_toluatool=$enableval], [cegui_build_toluatool=no])
    AC_ARG_ENABLE([safe-lua-module], AC_HELP_STRING([--enable-safe-lua-module],
                                                    [Enable building the Lua binding with extra safety checks
                                                     enabled at the expense of size and speed.  If you build
                                                     with debug enabled, these checks are also enabled
                                                     automatically.]),
        [cegui_enable_safe_lua=$enableval], [cegui_enable_safe_lua=no])

    if test x$cegui_found_lua = xyes && test x$cegui_with_lua = xyes; then
        cegui_with_lua=yes

        dnl if this is a debug build, we want to force the use of the 'safe' module
        if test x$cegui_enable_debug = xyes; then
            cegui_enable_safe_lua=yes
        fi

        dnl Decide which tolua++ library to use
        AC_ARG_ENABLE([external-toluapp], AC_HELP_STRING([--disable-external-toluapp], [Disables the use of any external tolua++ library, forcing the use of the version that accompanies CEGUI.]),
                [cegui_use_external_tolua=$enableval], [cegui_use_external_tolua=yes])

        if test x$cegui_use_external_tolua = xyes; then
            CEGUI_CHECK_TOLUAPP([toluapp],[cegui_found_toluapp=yes],[cegui_found_toluapp=no])
        else
            cegui_found_toluapp=no
        fi

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

    AM_CONDITIONAL([CEGUI_BUILD_LUA_MODULE_UNSAFE], [test x$cegui_enable_safe_lua = xno])
    AM_CONDITIONAL([CEGUI_BUILD_LUA_MODULE], [test x$cegui_with_lua = xyes])
    AM_CONDITIONAL([CEGUI_BUILD_TOLUAPPLIB], [test x$cegui_found_toluapp = xno])
    AM_CONDITIONAL([CEGUI_BUILD_TOLUACEGUI], [test x$cegui_build_toluatool = xyes])
    AC_SUBST(Lua_CFLAGS)
    AC_SUBST(Lua_LIBS)
    AC_SUBST(toluapp_CFLAGS)
    AC_SUBST(toluapp_LIBS)
])

# CEGUI_CHECK_TOLUAPP(variable, [action-if-found], [action-if-not-found])
# Checks for an installed copy of the tolua++ library
AC_DEFUN([CEGUI_CHECK_TOLUAPP],[
    AC_ARG_WITH([tolua++-incdir], AC_HELP_STRING([--with-tolua++-incdir=DIR], [Optionally specifies location of the tolua++ includes]),
        [cegui_tolua_incdir=$withval],[cegui_tolua_incdir=[.]])
    AC_ARG_WITH([tolua++-libdir], AC_HELP_STRING([--with-tolua++-libdir=DIR], [Optionally specifies location of the tolua++ library]),
        [cegui_tolua_libdir=$withval],[cegui_tolua_libdir=[.]])

    cegui_lib_paths="$cegui_tolua_libdir"
    cegui_inc_paths="$cegui_tolua_incdir /usr/local/include/tolua++ /usr/include/tolua++ /usr/local/include /usr/include"

    $1_CFLAGS=
    $1_LIBS=

    cegui_saved_CFLAGS="$CPPFLAGS"
    cegui_saved_LIBS="$LIBS"

    for cegui_path in $cegui_inc_paths; do
        ifelse($cegui_path, [.], CPPFLAGS="$cegui_saved_CFLAGS $Lua_CFLAGS", CPPFLAGS="-I$cegui_path $cegui_saved_CFLAGS $Lua_CFLAGS")
        AC_PREPROC_IFELSE([AC_LANG_SOURCE([
            [#include <tolua++.h>]])],
            [cegui_tolua_h_found=yes; cegui_tolua_flags="$cegui_path"; break],
            [cegui_tolua_h_found=no])
    done

    for cegui_path in $cegui_lib_paths; do
        for cegui_lib in tolua++ tolua++5.1 tolua++-5.1; do
            ifelse($cegui_path, [.], LIBS="$cegui_saved_LIBS", LIBS="-L$cegui_path $cegui_saved_LIBS")
            AC_CHECK_LIB([$cegui_lib], [tolua_endmodule],
                [cegui_tolua_l_found=yes; cegui_tolua_libs="$cegui_path"; cegui_tolua_libname="$cegui_lib"; break],
                [cegui_tolua_l_found=no],
                [$Lua_LIBS])
        done
        if test x"$cegui_tolua_l_found" = "xyes"; then
            break
        fi
    done

    CPPFLAGS="$cegui_saved_CFLAGS"
    LIBS="$cegui_saved_LIBS"

    if test x$cegui_tolua_h_found = xyes && test x$cegui_tolua_l_found = xyes; then
        if test x$cegui_tolua_flags != x.; then
            $1_CFLAGS="-I$cegui_tolua_flags"
        fi
        if test x$cegui_tolua_libs = x.; then
            $1_LIBS="-l$cegui_tolua_libname"
        else
            $1_LIBS="-L$cegui_tolua_libs -l$cegui_tolua_libname"
        fi

        ifelse([$2], [], :, [$2])
    else
        ifelse([$3], [], :, [$3])
    fi
])

# CEGUI_CHECK_TINYXML(variable, [action-if-found], [action-if-not-found])
# Checks for an installed copy of the TinyXML
AC_DEFUN([CEGUI_CHECK_TINYXML],[
    AC_ARG_WITH([tinyxml-incdir], AC_HELP_STRING([--with-tinyxml-incdir=DIR], [Optionally specifies location of the tinyxml includes]),
        [cegui_tinyxml_incdir=$withval],[cegui_tinyxml_incdir=[.]])
    AC_ARG_WITH([tinyxml-libdir], AC_HELP_STRING([--with-tinyxml-libdir=DIR], [Optionally specifies location of the tinyxml library]),
        [cegui_tinyxml_libdir=$withval],[cegui_tinyxml_libdir=[.]])

    AC_LANG_PUSH([C++])

    cegui_lib_paths="$cegui_tinyxml_libdir"
    cegui_inc_paths="$cegui_tinyxml_incdir /usr/local/include/tinyxml /usr/include/tinyxml /usr/local/include /usr/include"

    $1_CFLAGS=
    $1_LIBS=

    cegui_saved_CFLAGS="$CPPFLAGS"
    cegui_saved_LIBS="$LIBS"

    for cegui_path in $cegui_inc_paths; do
        ifelse($cegui_path, [.], CPPFLAGS="$cegui_saved_CFLAGS", CPPFLAGS="-I$cegui_path $cegui_saved_CFLAGS")
        AC_PREPROC_IFELSE([AC_LANG_SOURCE([
            [#include <tinyxml.h>]])],
            [cegui_tinyxml_h_found=yes; cegui_tinyxml_flags="$cegui_path"; break],
            [cegui_tinyxml_h_found=no])
    done

    for cegui_path in $cegui_lib_paths; do
        ifelse($cegui_path, [.], LIBS="$cegui_saved_LIBS", LIBS="-L$cegui_path $cegui_saved_LIBS")
        AC_CHECK_LIB([tinyxml], [main],
            [cegui_tinyxml_l_found=yes; cegui_tinyxml_libs="$cegui_path"; break],
            [cegui_tinyxml_l_found=no])
    done

    CPPFLAGS="$cegui_saved_CFLAGS"
    LIBS="$cegui_saved_LIBS"

    if test x$cegui_tinyxml_h_found = xyes && test x$cegui_tinyxml_l_found = xyes; then
        if test x$cegui_tinyxml_flags != x.; then
            $1_CFLAGS="-I$cegui_tinyxml_flags"
        fi
        if test x$cegui_tinyxml_libs = x.; then
            $1_LIBS="-ltinyxml"
        else
            $1_LIBS="-L$cegui_tinyxml_libs -ltinyxml"
        fi

        ifelse([$2], [], :, [$2])
    else
        ifelse([$3], [], :, [$3])
    fi

    AC_LANG_POP([C++])
])

# CEGUI_CHECK_GLEW(variable, [action-if-found], [action-if-not-found])
# Checks for an installed copy of GLEW
AC_DEFUN([CEGUI_CHECK_GLEW],[
    AC_ARG_WITH([glew-incdir], AC_HELP_STRING([--with-glew-incdir=DIR], [Optionally specifies location of the GL/glew.h headers]),
        [cegui_glew_incdir=$withval],[cegui_glew_incdir=[.]])
    AC_ARG_WITH([glew-libdir], AC_HELP_STRING([--with-glew-libdir=DIR], [Optionally specifies location of the glew library]),
        [cegui_glew_libdir=$withval],[cegui_glew_libdir=[.]])

    AC_LANG_PUSH([C])

    cegui_lib_paths="$cegui_glew_libdir"
    cegui_inc_paths="$cegui_glew_incdir /usr/local/include/ /usr/include/"

    $1_CFLAGS=
    $1_LIBS=

    cegui_glew_saved_CFLAGS="$CPPFLAGS"
    cegui_glew_saved_LIBS="$LIBS"

    for cegui_path in $cegui_inc_paths; do
        ifelse($cegui_path, [.], CPPFLAGS="$cegui_saved_CFLAGS", CPPFLAGS="-I$cegui_path $cegui_saved_CFLAGS")
        AC_PREPROC_IFELSE([AC_LANG_SOURCE([
            [#include <GL/glew.h>]])],
            [cegui_glew_h_found=yes; cegui_glew_flags="$cegui_path"; break],
            [cegui_glew_h_found=no])
    done

    for cegui_path in $cegui_lib_paths; do
        ifelse($cegui_path, [.], LIBS="$cegui_saved_LIBS", LIBS="-L$cegui_path $cegui_saved_LIBS")
        AC_CHECK_LIB([GLEW], [glewInit],
            [cegui_glew_l_found=yes; cegui_glew_libs="$cegui_path"; break],
            [cegui_glew_l_found=no])
    done

    CPPFLAGS="$cegui_glew_saved_CFLAGS"
    LIBS="$cegui_glew_saved_LIBS"

    if test x$cegui_glew_h_found = xyes && test x$cegui_glew_l_found = xyes; then
        if test x$cegui_glew_flags != x.; then
            $1_CFLAGS="-I$cegui_glew_flags"
        fi
        if test x$cegui_glew_libs = x.; then
            $1_LIBS="-lGLEW"
        else
            $1_LIBS="-L$cegui_glew_libs -lGLEW"
        fi

        ifelse([$2], [], :, [$2])
    else
        ifelse([$3], [], :, [$3])
    fi

    AC_LANG_POP([C])
])

# Check if we should enable bi-directional text, and which library to use.
AC_DEFUN([CEGUI_CHECK_BIDI],[
    AC_ARG_ENABLE([bidirectional-text],
                  AC_HELP_STRING([--enable-bidirectional-text],
                                 [Enable bidirectional text support.]),
                  [cegui_enable_bidi=$enableval], [cegui_enable_bidi=no])

    AC_ARG_WITH([embedded-minibidi], AC_HELP_STRING([--with-embedded-minibidi],
                                 [Use embedded minibi code for bidirectional text support]),
                  [cegui_with_minibidi=$withval], [cegui_with_minibidi=no])

    if test x$cegui_enable_bidi = xyes; then
        if test x$cegui_with_minibidi = xno; then
            PKG_CHECK_MODULES(fribidi, fribidi >= 0.10,
                              [cegui_found_fribidi=yes],
                              [cegui_found_fribidi=no; cegui_with_minibidi=yes])
        fi

        AC_DEFINE(CEGUI_BIDI_SUPPORT, [], [Define to enable bidirectional text support in CEGUI])
        AC_MSG_NOTICE([Bi-directional text support is enabled.])

        if test x$cegui_with_minibidi = xyes; then
            AC_DEFINE(CEGUI_USE_MINIBIDI, [], [Define to use embedded minibidi lib.])
            AC_MSG_NOTICE([Bi-directional text support is using internal minibidi library.])
        else
            AC_DEFINE(CEGUI_USE_FRIBIDI, [], [Define to use external fribidi lib.])
            AC_MSG_NOTICE([Bi-directional text support is using external fribidi library.])
        fi
    fi

    AM_CONDITIONAL([CEGUI_BUILD_FRIBIDI_MAPPER], [test x$cegui_enable_bidi = xyes && test x$cegui_with_minibidi != xyes])
    AM_CONDITIONAL([CEGUI_BUILD_MINIBIDI_MAPPER], [test x$cegui_enable_bidi = xyes && test x$cegui_with_minibidi = xyes])

    AC_SUBST(fribidi_CFLAGS)
    AC_SUBST(fribidi_LIBS)
])

# Check if freetype is available and if it's been disabled
AC_DEFUN([CEGUI_CHECK_FREETYPE],[
    AC_ARG_ENABLE([freetype],
                  AC_HELP_STRING([--disable-freetype],
                                 [Disable the use of the Freetype 2 library for font support.]),
                  [cegui_enable_freetype=$enableval], [cegui_enable_freetype=yes])

    if test x$cegui_enable_freetype = xyes; then
        PKG_CHECK_MODULES(freetype2, freetype2 >= 0.15.0,
            [AC_DEFINE(CEGUI_HAS_FREETYPE, [], [Define to enable freetype 2 font support in CEGUI])
             cegui_found_freetype=yes],
            [AC_MSG_ERROR([Freetype2 library not found!  To continue anyway, rerun configure with the --disable-freetype option.])
             cegui_found_freetype=no])
    else
        AC_MSG_NOTICE([Freetype font support was disabled; hope you know what you're doing!?])
    fi

    AM_CONDITIONAL([CEGUI_BUILD_FREETYPE_FONT], [test x$cegui_enable_freetype = xyes && test x$cegui_found_freetype=xyes])
    AC_SUBST(freetype2_CFLAGS)
    AC_SUBST(freetype2_LIBS)
])

AC_DEFUN([CEGUI_CHECK_DEFAULT_LOGGER],[
    AC_ARG_ENABLE([default-logger],
                  AC_HELP_STRING([--disable-default-logger],
                                 [Disable building the default logger.  NB: You must provide a custom alternative
                                 logger class based on the CEGUI::Logger class and instantiate it prior to creating
                                 the CEGUI::System object.]),
                  [cegui_enable_default_logger=$enableval], [cegui_enable_default_logger=yes])

    if test x$cegui_enable_default_logger = xyes; then
        AC_DEFINE(CEGUI_HAS_DEFAULT_LOGGER, [], [Define to enable possible use of CEGUI::DefaultLogger.])
    else
        AC_MSG_NOTICE([CEGUI::DefaultLogger will not be built; you must provide an alternative
                       and instantiate it before creating the CEGUI::System object!])
    fi

    AM_CONDITIONAL([CEGUI_BUILD_DEFAULT_LOGGER], [test x$cegui_enable_default_logger = xyes])
])

AC_DEFUN([CEGUI_LIBTOOL_OPTIONS],[
    AC_ARG_ENABLE([version-suffix],
                  AC_HELP_STRING([--disable-version-suffix],
                                 [Disable adding the release version to shared libraries.
                                  Using this option leaves you with no version suffix, so may not
                                  be what you want to do (except for Win32 builds, perhaps).
                                  See also --with-version-suffix for how to modify the version
                                  suffix to be used.]),
                  [cegui_enable_version_suffix=$enableval],
                  [
                      if test x$MINGW32 = xyes; then
                          cegui_enable_version_suffix=no
                      else
                          cegui_enable_version_suffix=yes
                      fi
                  ])

    AC_ARG_WITH([version-suffix],
                  AC_HELP_STRING([--with-version-suffix],
                                 [The release version added to shared libraries unless the
                                  --disable-version-suffix option is given (default=CEGUI_RELEASE_VERSION)]),
                  [cegui_lib_version_suffix=$withval], [cegui_lib_version_suffix=CEGUI_RELEASE_VERSION])

    if test x$cegui_enable_version_suffix = xyes; then
        AC_DEFINE(CEGUI_HAS_VERSION_SUFFIX, [], [Define if shared libraries have a version suffix.])
        AC_DEFINE_UNQUOTED(CEGUI_VERSION_SUFFIX, ["$cegui_lib_version_suffix"], [Defines the shared library version suffix.])

        CEGUI_LIB_LINK_FLAGS="-release $cegui_lib_version_suffix"
    else
        CEGUI_LIB_LINK_FLAGS="-avoid-version"
    fi

    if test x$MINGW32 = xyes; then
        if test x$enable_shared = xyes; then
            CEGUI_LIB_LINK_FLAGS="$CEGUI_LIB_LINK_FLAGS -shared -no-undefined"
        else
            CEGUI_LIB_LINK_FLAGS="$CEGUI_LIB_LINK_FLAGS -static"
        fi
    fi

    AC_SUBST(CEGUI_LIB_LINK_FLAGS)
])

AC_DEFUN([CEGUI_CHECK_BUILD_SUFFIX],[
    AC_ARG_WITH([build-suffix],
                  AC_HELP_STRING([--with-build-suffix],
                                 [Adds a custom suffix to the name of all binaries.]),
                  [cegui_bsfx=$withval], [cegui_bsfx=""])

    if test "x$cegui_bsfx" != "x"; then
        AC_DEFINE(CEGUI_HAS_BUILD_SUFFIX, [], [Define if output names have an added suffix.])
        AC_DEFINE_UNQUOTED(CEGUI_BUILD_SUFFIX, ["$cegui_bsfx"], [Defines the suffix appended to output binaries.])
    fi

    AC_SUBST(cegui_bsfx)
])

# Check if pcre is available and if it's been disabled
AC_DEFUN([CEGUI_CHECK_PCRE],[
    AC_ARG_ENABLE([pcre],
                  AC_HELP_STRING([--disable-pcre],
                                 [Disable the use of the PCRE library for regular expression support.]),
                  [cegui_enable_pcre=$enableval], [cegui_enable_pcre=yes])

    if test x$cegui_enable_pcre = xyes; then
        PKG_CHECK_MODULES(pcre, libpcre >= 5.0,
            [AC_DEFINE(CEGUI_HAS_PCRE_REGEX, [], [Define to enable PCRE regular expression support in CEGUI])
             cegui_found_pcre=yes],
            [AC_MSG_ERROR([PCRE library not found!  To continue anyway, rerun configure with the --disable-pcre option.])
             cegui_found_pcre=no])
    else
        AC_MSG_NOTICE([PCRE regular expression support was disabled; hope you know what you're doing!?])
    fi

    AM_CONDITIONAL([CEGUI_BUILD_PCRE_REGEX], [test x$cegui_enable_pcre = xyes && test x$cegui_found_pcre=xyes])
    AC_SUBST(pcre_CFLAGS)
    AC_SUBST(pcre_LIBS)
])

# check and initialise static build setup.
# MUST be done after checks for XMLParser and ImageCodec modules
# since we need to know about the default parsers to link in for
# the samples.
AC_DEFUN([CEGUI_CHECK_STATIC],[
    if test x$enable_static = xyes && test x$enable_shared = xyes; then
            AC_MSG_ERROR([
You have enabled the static build, this can not be used at the same time as
shared builds due to the different configurations requiring different build
options.  You should add the --disable-shared option also if you wish to
continue with the static build.])
    fi

    if test x$enable_static = xyes; then
        SAMPLE_STATIC_LDFLAGS='$(top_builddir)'"/cegui/src/WindowRendererSets/Falagard/libCEGUIFalagardWRBase${cegui_bsfx}.la "

        case $cegui_default_image_codec in
            FreeImageImageCodec ) SAMPLE_STATIC_LDFLAGS+='$(top_builddir)'"/cegui/src/ImageCodecModules/FreeImageImageCodec/libCEGUIFreeImageImageCodec${cegui_bsfx}.la "
                                    ;;
            SILLYImageCodec ) SAMPLE_STATIC_LDFLAGS+='$(top_builddir)'"/cegui/src/ImageCodecModules/SILLYImageCodec/libCEGUISILLYImageCodec${cegui_bsfx}.la "
                                    ;;
            DevILImageCodec ) SAMPLE_STATIC_LDFLAGS+='$(top_builddir)'"/cegui/src/ImageCodecModules/DevILImageCodec/libCEGUIDevILImageCodec${cegui_bsfx}.la "
                                    ;;
            CoronaImageCodec ) SAMPLE_STATIC_LDFLAGS+='$(top_builddir)'"/cegui/src/ImageCodecModules/CoronaImageCodec/libCEGUICoronaImageCodec${cegui_bsfx}.la "
                                    ;;
            TGAImageCodec ) SAMPLE_STATIC_LDFLAGS+='$(top_builddir)'"/cegui/src/ImageCodecModules/TGAImageCodec/libCEGUITGAImageCodec${cegui_bsfx}.la "
                                    ;;
        esac

        case $cegui_default_parser in
            ExpatParser ) SAMPLE_STATIC_LDFLAGS+='$(top_builddir)'"/cegui/src/XMLParserModules/ExpatParser/libCEGUIExpatParser${cegui_bsfx}.la "
                                    ;;
            LibxmlParser ) SAMPLE_STATIC_LDFLAGS+='$(top_builddir)'"/cegui/src/XMLParserModules/LibxmlParser/libCEGUILibxmlParser${cegui_bsfx}.la "
                                    ;;
            TinyXMLParser ) SAMPLE_STATIC_LDFLAGS+='$(top_builddir)'"/cegui/src/XMLParserModules/TinyXMLParser/libCEGUITinyXMLParser${cegui_bsfx}.la "
                                    ;;
            XercesParser ) SAMPLE_STATIC_LDFLAGS+='$(top_builddir)'"/cegui/src/XMLParserModules/XercesParser/libCEGUIXercesParser${cegui_bsfx}.la "
                                    ;;
        esac
    fi

    AM_CONDITIONAL([CEGUI_BUILD_STATIC], [test x$enable_static = xyes && test x$enable_shared != xyes])
    AC_SUBST(SAMPLE_STATIC_LDFLAGS)
])

AC_DEFUN([CEGUI_CHECK_MINIZIP_RESOURCE_PROVIDER],[
    AC_ARG_ENABLE([minizip-resource-provider],
                  AC_HELP_STRING([--enable-minizip-resource-provider],
                                 [Build the Minizip resource provider.]),
                  [cegui_enable_minizip_rp=$enableval], [cegui_enable_minizip_rp=no])

    if test "x$cegui_enable_minizip_rp" = "xyes"; then
        CEGUI_CHECK_ZLIB()
        if test "x$HAVE_ZLIB" = "xyes"; then
            AC_DEFINE(CEGUI_HAS_MINIZIP_RESOURCE_PROVIDER, [], [Defined when the resource provider with minizip support is built.])
        else
            cegui_enable_minizip_rp=no
            AC_MSG_ERROR([MiniZip resource provider requested but zlib was not found!  You can specfy zlib's location with the --with-zlib option.])
        fi
    fi

    AM_CONDITIONAL([CEGUI_BUILD_MINIZIP_RESOURCE_PROVIDER], [test "x$cegui_enable_minizip_rp" = "xyes" && test "x$HAVE_ZLIB" = "xyes"])
])

dnl This was borrowed and modified from the SILLY acinclude.m4 file ;)
AC_DEFUN([CEGUI_CHECK_ZLIB], [
dnl save environment 
cegui_zlib_save_CPPFLAGS="$CPPFLAGS"
cegui_zlib_save_LDFLAGS="$LDFLAGS"
AC_LANG_PUSH(C)

dnl clear these so as to isolate the zlib specific parts
CPPFLAGS=""
LDFLAGS=""

dnl define search path 
cegui_zlib_search="/usr/local /usr"
AC_ARG_WITH([zlib],
    AC_HELP_STRING([--with-zlib=DIR], [root directory path of zlib installation]), 
    [if test "$withval" != no ; then
        cegui_zlib_search="$withval $zlib_search"
    fi], 
    [])

dnl search in all path 
HAVE_ZLIB=no
for dir in $cegui_zlib_search  
do 
    if  test "$HAVE_ZLIB"  = "no" ; then 
        cegui_zlib_home="$dir"
        cegui_zlib_cppflags="-I$cegui_zlib_home/include" 
        cegui_zlib_ldflags="-L$cegui_zlib_home/lib" 
      	if test -f "${cegui_zlib_home}/include/zlib.h" ; then 
            CPPFLAGS="$cegui_zlib_cppflags $cegui_zlib_save_CPPFLAGS"
            LDFLAGS="$cegui_zlib_ldflags $cegui_zlib_save_LDFLAGS"
            AC_CHECK_HEADER(zlib.h, [cegui_zlib_cv_zlib_h=yes], [cegui_zlib_cv_zlib_h=no])
            AC_CHECK_LIB(z, inflateEnd, [cegui_zlib_cv_libz=yes], [cegui_zlib_cv_libz=no])
            if  test "$cegui_zlib_cv_libz" = "yes" -a "$cegui_zlib_cv_zlib_h" = "yes" ; then 
                HAVE_ZLIB=yes
                ZLIB_CFLAGS="$cegui_zlib_cppflags"
                ZLIB_LIBS="$cegui_zlib_ldflags -lz"
            fi
        fi
    fi
done

dnl if found update env 
if test "$HAVE_ZLIB" = "yes" ; then 
    AC_DEFINE_UNQUOTED([HAVE_ZLIB], [1], [zlib is available])
    AC_SUBST(HAVE_ZLIB)
    AC_SUBST(ZLIB_CFLAGS)
    AC_SUBST(ZLIB_LIBS)
fi

dnl restore previous environment 
AC_LANG_POP(C)
CPPFLAGS="$cegui_zlib_save_CPPFLAGS"
LDFLAGS="$cegui_zlib_save_LDFLAGS"
])


# Check for python
AC_DEFUN([CEGUI_CHECK_PYTHON],
[
    AC_ARG_ENABLE([python-module], AC_HELP_STRING([--disable-python-module], [Disables building of the Python extension module.]),
                  [cegui_with_python=$enableval], [cegui_with_python=yes])

    if test x"$cegui_with_python" = xyes; then
        AM_PATH_PYTHON(, , [:])
        AX_PYTHON()

        if test x"$PYTHON_LIB" != xno && test x"$PYTHON_INCLUDE_DIR" != xno; then
            AX_BOOST_PYTHON()

            if test x"$BOOST_PYTHON_LIB" = x; then
                cegui_with_python=no
            fi
        else
            cegui_with_python=no
        fi
    fi

    AM_CONDITIONAL([CEGUI_BUILD_PYTHON_MODULE], [test x"$cegui_with_python" = xyes])
])


# ===========================================================================
#         http://www.gnu.org/software/autoconf-archive/ax_python.html
# ===========================================================================
#
# SYNOPSIS
#libboost_python
#   AX_PYTHON
#
# DESCRIPTION
#
#   This macro does a complete Python development environment check.
#
#   It recurses through several python versions (from 2.1 to 2.6 in this
#   version), looking for an executable. When it finds an executable, it
#   looks to find the header files and library.
#
#   It sets PYTHON_BIN to the name of the python executable,
#   PYTHON_INCLUDE_DIR to the directory holding the header files, and
#   PYTHON_LIB to the name of the Python library.
#
#   This macro calls AC_SUBST on PYTHON_BIN (via AC_CHECK_PROG),
#   PYTHON_INCLUDE_DIR and PYTHON_LIB.
#
# LICENSE
#
#   Copyright (c) 2008 Michael Tindal
#
#   This program is free software; you can redistribute it and/or modify it
#   under the terms of the GNU General Public License as published by the
#   Free Software Foundation; either version 2 of the License, or (at your
#   option) any later version.
#
#   This program is distributed in the hope that it will be useful, but
#   WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
#   Public License for more details.
#
#   You should have received a copy of the GNU General Public License along
#   with this program. If not, see <http://www.gnu.org/licenses/>.
#
#   As a special exception, the respective Autoconf Macro's copyright owner
#   gives unlimited permission to copy, distribute and modify the configure
#   scripts that are the output of Autoconf when processing the Macro. You
#   need not follow the terms of the GNU General Public License when using
#   or distributing such scripts, even though portions of the text of the
#   Macro appear in them. The GNU General Public License (GPL) does govern
#   all other use of the material that constitutes the Autoconf Macro.
#
#   This special exception to the GPL applies to versions of the Autoconf
#   Macro released by the Autoconf Archive. When you make and distribute a
#   modified version of the Autoconf Macro, you may extend this special
#   exception to the GPL to apply to your modified version as well.
AC_DEFUN([AX_PYTHON],
[AC_MSG_CHECKING(for python build information)
AC_MSG_RESULT([])
for python in python2.7 python2.6 python2.5 python2.4 python2.3 python2.2 python2.1 python; do
AC_CHECK_PROGS(PYTHON_BIN, [$python])
ax_python_bin=$PYTHON_BIN
if test x$ax_python_bin != x; then
    AC_CHECK_LIB($ax_python_bin, main, ax_python_lib=$ax_python_bin, ax_python_lib=no)
    cegui_old_cppflags="$CPPFLAGS"
    for hpath in /usr/local/include /usr/include; do
        CPPFLAGS="-I$hpath/$ax_python_bin"
        AC_CHECK_HEADERS([Python.h],
            [[ax_python_header=$hpath/$ax_python_bin]],
            ax_python_header=no;unset ac_cv_header_Python_h)
        if test x$ax_python_header != xno; then
            break;
        fi
    done
    CPPFLAGS="$cegui_old_cppflags"
    if test x$ax_python_lib != xno; then
        if test x$ax_python_header != xno; then
            break;
        fi
    fi

    unset ac_cv_header_Python_h
fi
done
if test x$ax_python_bin = x; then
   ax_python_bin=no
fi
if test x$ax_python_header = x; then
   ax_python_header=no
fi
if test x$ax_python_lib = x; then
   ax_python_lib=no
fi

AC_MSG_RESULT([  results of the Python check:])
AC_MSG_RESULT([    Binary:      $ax_python_bin])
AC_MSG_RESULT([    Library:     $ax_python_lib])
AC_MSG_RESULT([    Include Dir: $ax_python_header])

if test x$ax_python_header != xno; then
  PYTHON_INCLUDE_DIR=$ax_python_header
  AC_SUBST(PYTHON_INCLUDE_DIR)
fi
if test x$ax_python_lib != xno; then
  PYTHON_LIB=$ax_python_lib
  AC_SUBST(PYTHON_LIB)
fi
])

# ===========================================================================
#      http://www.gnu.org/software/autoconf-archive/ax_boost_python.html
# ===========================================================================
#
# SYNOPSIS
#
#   AX_BOOST_PYTHON
#
# DESCRIPTION
#
#   This macro checks to see if the Boost.Python library is installed. It
#   also attempts to guess the currect library name using several attempts.
#   It tries to build the library name using a user supplied name or suffix
#   and then just the raw library.
#
#   If the library is found, HAVE_BOOST_PYTHON is defined and
#   BOOST_PYTHON_LIB is set to the name of the library.
#
#   This macro calls AC_SUBST(BOOST_PYTHON_LIB).
#
#   In order to ensure that the Python headers are specified on the include
#   path, this macro requires AX_PYTHON to be called.
#
# LICENSE
#
#   Copyright (c) 2008 Michael Tindal
#
#   This program is free software; you can redistribute it and/or modify it
#   under the terms of the GNU General Public License as published by the
#   Free Software Foundation; either version 2 of the License, or (at your
#   option) any later version.
#
#   This program is distributed in the hope that it will be useful, but
#   WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
#   Public License for more details.
#
#   You should have received a copy of the GNU General Public License along
#   with this program. If not, see <http://www.gnu.org/licenses/>.
#
#   As a special exception, the respective Autoconf Macro's copyright owner
#   gives unlimited permission to copy, distribute and modify the configure
#   scripts that are the output of Autoconf when processing the Macro. You
#   need not follow the terms of the GNU General Public License when using
#   or distributing such scripts, even though portions of the text of the
#   Macro appear in them. The GNU General Public License (GPL) does govern
#   all other use of the material that constitutes the Autoconf Macro.
#
#   This special exception to the GPL applies to versions of the Autoconf
#   Macro released by the Autoconf Archive. When you make and distribute a
#   modified version of the Autoconf Macro, you may extend this special
#   exception to the GPL to apply to your modified version as well.
AC_DEFUN([AX_BOOST_PYTHON],
[AC_REQUIRE([AX_PYTHON])dnl
AC_CACHE_CHECK(whether the Boost::Python library is available,
ac_cv_boost_python,
[AC_LANG_SAVE
 AC_LANG_CPLUSPLUS
 CPPFLAGS_SAVE=$CPPFLAGS
 if test x$PYTHON_INCLUDE_DIR != x; then
   CPPFLAGS="-I$PYTHON_INCLUDE_DIR $CPPFLAGS"
 fi
 AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
 #include <boost/python/module.hpp>
 using namespace boost::python;
 BOOST_PYTHON_MODULE(test) { throw "Boost::Python test."; }]],
			   [[return 0;]])],
			   ac_cv_boost_python=yes, ac_cv_boost_python=no)
 AC_LANG_RESTORE
 CPPFLAGS=$CPPFLAGS_SAVE
])
if test "$ac_cv_boost_python" = "yes"; then
  AC_DEFINE(HAVE_BOOST_PYTHON,,[define if the Boost::Python library is available])
  ax_python_lib=boost_python
  AC_ARG_WITH([boost-python],AS_HELP_STRING([--with-boost-python],[specify the boost python library or suffix to use]),
  [if test "x$with_boost_python" != "xno"; then
     ax_python_lib=$with_boost_python
     ax_boost_python_lib=boost_python-$with_boost_python
   fi])
  for ax_lib in $ax_python_lib $ax_boost_python_lib boost_python; do
    AC_CHECK_LIB($ax_lib, exit, [BOOST_PYTHON_LIB=$ax_lib break], [], [-l$PYTHON_LIB])
  done
  AC_SUBST(BOOST_PYTHON_LIB)
fi
])

