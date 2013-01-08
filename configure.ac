AC_PREREQ(2.59)

dnl Define the sub-version numbers for this release
dnl
dnl now done like this to save repetition, and make it more explicit where
dnl we should define these numbers (we could pick up the same info by other means
dnl though this leaves nobody in any doubt about where this is being defined).
m4_define([CEGUI_MAJ_VER], 0)
m4_define([CEGUI_MIN_VER], 7)
m4_define([CEGUI_REL_VER], 9)

dnl Construct the final version number out of the sub-version values specified above
m4_define([CEGUI_RELEASE_VERSION], [CEGUI_MAJ_VER.CEGUI_MIN_VER.CEGUI_REL_VER])

AC_INIT([CEGUI], [CEGUI_RELEASE_VERSION], [http://mantis.cegui.org.uk/], [CEGUI])
AC_CONFIG_SRCDIR([cegui/src/CEGUIBase.cpp])
AM_INIT_AUTOMAKE([dist-zip tar-ustar])
AC_CONFIG_HEADER([cegui/include/config.h])
AC_CANONICAL_HOST

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC

AC_CHECK_PROG([DOXYGEN], [doxygen], [doxygen], [])
AC_CHECK_PROG([DOXYGENDOT], [dot], [dot], [])

PKG_PROG_PKG_CONFIG
if test -z "$PKG_CONFIG"; then
    AC_MSG_ERROR([pkg-config was not found.  Please ensure pkg-config is installed and / or set the PKG_CONFIG variable])
fi

# Disable building static library
AC_DISABLE_STATIC

# We want DLLs on Win32 platform
AC_LIBTOOL_WIN32_DLL

# Initialise libtool
AM_PROG_LIBTOOL

# Setup some stuff depending on the system we're running on
case $host_os in
    *mingw32* ) MINGW32=yes
                cegui_pc_platform_libs=-lwinmm
                ;;
     *darwin* ) MACOSX=yes
                cegui_pc_platform_libs=
                ;;
            * ) MINGW32=no
                cegui_pc_platform_libs=
                ;;
esac

AC_SUBST(cegui_pc_platform_libs)

AM_CONDITIONAL([MINGW32_BUILD], [test x$MINGW32 = xyes])
AM_CONDITIONAL([MACOSX_BUILD], [test x$MACOSX = xyes])

# Checks for libraries (skip on mingw32).
if test x$MINGW32 != xyes; then
    AC_CHECK_LIB([dl], [dlopen])
    AC_CHECK_LIB([pthread], [pthread_create],, AC_MSG_ERROR([cannot find pthread library]))
fi

CEGUI_CODE_OPTIONS

CEGUI_CHECK_FREETYPE
CEGUI_CHECK_PCRE

CEGUI_CHECK_XML_PARSERS
CEGUI_CHECK_IMAGE_CODECS

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([limits.h locale.h malloc.h stddef.h stdlib.h string.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_CONST
AC_C_INLINE
AC_TYPE_SIZE_T
AC_STRUCT_TM
AC_C_VOLATILE
AC_CHECK_TYPES([ptrdiff_t])

# Checks for library functions.
AC_FUNC_ERROR_AT_LINE
AC_FUNC_MEMCMP
AC_FUNC_MKTIME
AC_FUNC_STRCOLL
AC_FUNC_STRFTIME
AC_CHECK_FUNCS([atexit floor memchr memmove memset pow regcomp setlocale sqrt strchr strcspn strerror strpbrk strtoul])

# check for renderers
CEGUI_ENABLE_OPENGL_RENDERER
CEGUI_ENABLE_OGRE_RENDERER
CEGUI_ENABLE_NULL_RENDERER
CEGUI_ENABLE_IRRLICHT_RENDERER
CEGUI_ENABLE_DIRECTFB_RENDERER

# decide whether to use gtk dialog in samples
CEGUI_CHECK_GTK_FOR_SAMPLES

# decide whether to build the samples
CEGUI_CHECK_WANTS_SAMPLES

# decide whether to build the lua module
CEGUI_CHECK_LUA

# decide whether to build the python bindings
CEGUI_CHECK_PYTHON

# handle config for bi-directional text support.
CEGUI_CHECK_BIDI

CEGUI_CHECK_DEFAULT_LOGGER
CEGUI_CHECK_MINIZIP_RESOURCE_PROVIDER

CEGUI_LIBTOOL_OPTIONS
CEGUI_CHECK_BUILD_SUFFIX
CEGUI_CHECK_STATIC

dnl Create the makefiles
AC_CONFIG_FILES([cegui/CEGUI-NULL.pc
                 cegui/CEGUI-OGRE.pc
                 cegui/CEGUI-OPENGL.pc
                 cegui/CEGUI.pc
                 cegui/CEGUI.spec
                 cegui/Makefile
                 cegui/include/elements/Makefile
                 cegui/include/falagard/Makefile
                 cegui/include/ImageCodecModules/CoronaImageCodec/Makefile
                 cegui/include/ImageCodecModules/DevILImageCodec/Makefile
                 cegui/include/ImageCodecModules/FreeImageImageCodec/Makefile
                 cegui/include/ImageCodecModules/Makefile
                 cegui/include/ImageCodecModules/SILLYImageCodec/Makefile
                 cegui/include/ImageCodecModules/STBImageCodec/Makefile
                 cegui/include/ImageCodecModules/TGAImageCodec/Makefile
                 cegui/include/Makefile
                 cegui/include/RendererModules/Direct3D10/Makefile
                 cegui/include/RendererModules/Direct3D11/Makefile
                 cegui/include/RendererModules/Direct3D9/Makefile
                 cegui/include/RendererModules/DirectFB/Makefile
                 cegui/include/RendererModules/Irrlicht/Makefile
                 cegui/include/RendererModules/Null/Makefile
                 cegui/include/RendererModules/Ogre/Makefile
                 cegui/include/RendererModules/OpenGL/Makefile
                 cegui/include/RendererModules/Makefile
                 cegui/include/ScriptingModules/LuaScriptModule/Makefile
                 cegui/include/ScriptingModules/LuaScriptModule/support/Makefile
                 cegui/include/ScriptingModules/LuaScriptModule/support/tolua++/Makefile
                 cegui/include/ScriptingModules/Makefile
                 cegui/include/WindowRendererSets/Falagard/Makefile
                 cegui/include/WindowRendererSets/Makefile
                 cegui/include/XMLParserModules/ExpatParser/Makefile
                 cegui/include/XMLParserModules/LibxmlParser/Makefile
                 cegui/include/XMLParserModules/Makefile
                 cegui/include/XMLParserModules/RapidXMLParser/Makefile
                 cegui/include/XMLParserModules/TinyXMLParser/Makefile
                 cegui/include/XMLParserModules/XercesParser/Makefile
                 cegui/src/ImageCodecModules/CoronaImageCodec/Makefile
                 cegui/src/ImageCodecModules/DevILImageCodec/Makefile
                 cegui/src/ImageCodecModules/FreeImageImageCodec/Makefile
                 cegui/src/ImageCodecModules/Makefile
                 cegui/src/ImageCodecModules/SILLYImageCodec/Makefile
                 cegui/src/ImageCodecModules/STBImageCodec/Makefile
                 cegui/src/ImageCodecModules/TGAImageCodec/Makefile
                 cegui/src/Makefile
                 cegui/src/RendererModules/Direct3D10/Makefile
                 cegui/src/RendererModules/Direct3D11/Makefile
                 cegui/src/RendererModules/Direct3D9/Makefile
                 cegui/src/RendererModules/DirectFB/Makefile
                 cegui/src/RendererModules/Irrlicht/Makefile
                 cegui/src/RendererModules/Null/Makefile
                 cegui/src/RendererModules/Ogre/Makefile
                 cegui/src/RendererModules/OpenGL/Makefile
                 cegui/src/RendererModules/Makefile
                 cegui/src/ScriptingModules/LuaScriptModule/Makefile
                 cegui/src/ScriptingModules/LuaScriptModule/package/Makefile
                 cegui/src/ScriptingModules/LuaScriptModule/support/Makefile
                 cegui/src/ScriptingModules/LuaScriptModule/support/tolua++bin/Makefile
                 cegui/src/ScriptingModules/LuaScriptModule/support/tolua++/Makefile
                 cegui/src/ScriptingModules/Makefile
                 cegui/src/ScriptingModules/PythonScriptModule/Makefile
                 cegui/src/ScriptingModules/PythonScriptModule/bindings/Makefile
                 cegui/src/WindowRendererSets/Falagard/Makefile
                 cegui/src/WindowRendererSets/Makefile
                 cegui/src/XMLParserModules/ExpatParser/Makefile
                 cegui/src/XMLParserModules/LibxmlParser/Makefile
                 cegui/src/XMLParserModules/Makefile
                 cegui/src/XMLParserModules/RapidXMLParser/Makefile
                 cegui/src/XMLParserModules/TinyXMLParser/Makefile
                 cegui/src/XMLParserModules/XercesParser/Makefile
                 datafiles/Makefile
                 doc/Makefile
                 doc/doxygen/doxyfile
                 doc/doxygen/Makefile
                 projects/Makefile
                 Samples/common/include/Makefile
                 Samples/common/Makefile
                 Samples/common/src/Makefile
                 Samples/Demo6/Makefile
                 Samples/Demo7/Makefile
                 Samples/Demo8/Makefile
                 Samples/DragDropDemo/Makefile
                 Samples/FalagardDemo1/Makefile
                 Samples/FirstWindow/Makefile
                 Samples/InventoryDemo/Makefile
                 Samples/FontDemo/Makefile
                 Samples/Makefile
                 Samples/Minesweeper/Makefile
                 Samples/ScrollablePaneDemo/Makefile
                 Samples/TabControlDemo/Makefile
                 Samples/TextDemo/Makefile
                 Samples/TreeDemo/Makefile
                 Makefile])
AC_OUTPUT

dnl Now generate some output to summarise what we have done here
echo
echo "********************************************************************************"
echo "* Crazy Eddie's GUI System - Configuration Results Summary"
echo "********************************************************************************"
echo "* Library Release Version:                              CEGUI_RELEASE_VERSION"
echo "*"
echo "* Code options:"
echo "*         Building CEGUI in debug mode:                 ${cegui_enable_debug-no}"
echo "*"
echo "* Bi-directional text support:                          ${cegui_enable_bidi-no}"
if test x$cegui_enable_bidi = xyes; then
    if test x$cegui_with_minibidi = xyes; then
echo "*         Using: internal minibidi library"
    else
echo "*         Using: external fribidi library"
    fi
fi
echo "*"
echo "* Building MinizipResourceProvider:                     ${cegui_enable_minizip_rp-no}"
echo "*"
echo "* Renderer Modules:"
echo "*         Building OpenGL Renderer:                     ${cegui_enable_opengl-no}"
echo "*             Using external GLEW library:              ${cegui_found_glew-no}"
echo "*         Building Ogre Renderer:                       ${cegui_enable_ogre-no}"
echo "*         Building Irrlicht Renderer:                   ${cegui_enable_irrlicht-no}"
echo "*         Building DirectFB Renderer:                   ${cegui_enable_directfb-no}"
echo "*         Building Null Renderer:                       ${cegui_enable_null-no}"
echo "*"
echo "* Image Loading Codec Modules:"
echo "*         Building Corona Image Codec:                  ${cegui_with_corona-no}"
echo "*         Building DevIL Image Codec:                   ${cegui_with_devil-no}"
echo "*         Building FreeImage Image Codec:               ${cegui_with_freeimage-no}"
echo "*         Building SILLY Image Codec:                   ${cegui_with_silly-no}"
echo "*         Building STB Image Codec:                     ${cegui_with_stb-no}"
echo "*         Building TGA Image Codec:                     ${cegui_with_tga-no}"
echo "*"
echo "*         Default Image Codec will be:                  ${cegui_default_image_codec}"
if test x"$cegui_default_image_codec" != x"DevILImageCodec" &&
   test x"$cegui_default_image_codec" != x"FreeImageImageCodec" &&
   test x"$cegui_default_image_codec" != x"SILLYImageCodec" &&
   test x"$cegui_default_image_codec" != x"CoronaImageCodec" &&
   test x"$cegui_default_image_codec" != x"STBImageCodec" &&
   test x"$cegui_default_image_codec" != x"TGAImageCodec"; then
echo "*"
echo "*         WARNING!  Default codec is not set to one of the pre-made codecs:"
echo "*         WARNING!      DevILImageCodec, FreeImageImageCodec, SILLYImageCodec,"
echo "*         WARNING!      CoronaImageCodec, STBImageCodec or TGAImageCodec."
echo "*         WARNING!"
echo "*         WARNING!  This is fine if you meant to do this and are using a custom"
echo "*         WARNING!  codec as the 'hard-coded' default, else you should go back"
echo "*         WARNING!  and check the config values specified for building codecs"
echo "*         WARNING!  and setting the default codec (--with-default-image-codec)."
fi
echo "*"
echo "* XML Parser Modules:"
echo "*         Building TinyXMLParser:                       ${cegui_with_tinyxml-no}"
if test x$cegui_with_tinyxml = xyes; then
echo "*             Using external TinyXML library:           ${cegui_found_tinyxml-no}"
fi
echo "*         Building ExpatParser:                         ${cegui_with_expat-no}"
echo "*         Building LibXMLParser:                        ${cegui_with_libxml-no}"
echo "*         Building XercesParser:                        ${cegui_with_xerces-no}"
echo "*         Building RapidXMLParser:                      ${cegui_with_rapidxml-no}"
echo "*"
echo "*         Default XML Parser is:                        ${cegui_default_parser-none}"
if test x"$cegui_default_parser" != x"XercesParser" &&
   test x"$cegui_default_parser" != x"ExpatParser" &&
   test x"$cegui_default_parser" != x"LibxmlParser" &&
   test x"$cegui_default_parser" != x"RapidXMLParser" &&
   test x"$cegui_default_parser" != x"TinyXMLParser"; then
echo "*"
echo "*         WARNING!  Default parser is not set to one of the pre-made parsers:"
echo "*         WARNING!  XercesParser, ExpatParser, LibxmlParser, RapidXMLParser"
echo "*         WARNING!  or TinyXMLParser."
echo "*         WARNING!"
echo "*         WARNING!  This is fine if you meant to do this and are using a custom"
echo "*         WARNING!  parser as the 'hard-coded' default, else you should go back"
echo "*         WARNING!  and check the config values specified for building parsers"
echo "*         WARNING!  and setting the default parser (--with-default-xml-parser)."
fi
echo "*"
echo "* Scripting:"
echo "*         Building Lua scripting module:                ${cegui_with_lua-no}"
if test x$cegui_with_lua = xyes; then
echo "*             Safety checks enabled:                    ${cegui_enable_safe_lua-no}"
fi
echo "*         Building tolua++cegui generator:              ${cegui_build_toluatool-no}"
if test x$cegui_with_lua = xyes; then
echo "*             Using external tolua++ library:           ${cegui_found_toluapp-no}"
fi
echo "*"
echo "*         Building Python extension module(s):          ${cegui_with_python-no}"
echo "*"
echo "* Samples Framework:"
echo "*         Building Samples:                             ${cegui_build_samples-no}"
echo "*         GTK2 based dialog for renderer selection:     ${cegui_with_gtk-no}"
echo "*         OpenGL Renderer available in samples:         ${cegui_samples_use_opengl-no}"
echo "*         Irrlicht Renderer available in samples:       ${cegui_samples_use_irrlicht-no}"
echo "*         DirectFB Renderer available in samples:       ${cegui_samples_use_directfb-no}"
echo "*         Ogre3D Renderer available in samples:         ${cegui_samples_use_ogre-no}"
echo "********************************************************************************"
echo
echo "Now you can do make && make install.  Good Luck!"
echo
