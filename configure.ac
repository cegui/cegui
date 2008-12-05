AC_PREREQ(2.59)

dnl Define the sub-version numbers for this release
dnl
dnl now done like this to save repetition, and make it more explicit where
dnl we should define these numbers (we could pick up the same info by other means
dnl though this leaves nobody in any doubt about where this is being defined).
m4_define([CEGUI_MAJ_VER], 0)
m4_define([CEGUI_MIN_VER], 6)
m4_define([CEGUI_REL_VER], 2)

dnl Construct the final version number out of the sub-version values specified above
m4_define([CEGUI_RELEASE_VERSION], [CEGUI_MAJ_VER.CEGUI_MIN_VER.CEGUI_REL_VER])

AC_INIT([CEGUI], [CEGUI_RELEASE_VERSION])
AM_INIT_AUTOMAKE([CEGUI],[CEGUI_RELEASE_VERSION])
AC_CONFIG_HEADER([include/config.h])
AC_CANONICAL_HOST

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC

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

PKG_CHECK_MODULES(freetype2, freetype2 >= 0.15.0)
AC_SUBST(freetype2_CFLAGS)
AC_SUBST(freetype2_LIBS)

PKG_CHECK_MODULES(pcre, libpcre >= 5.0)
AC_SUBST(pcre_CFLAGS)
AC_SUBST(pcre_LIBS)

CEGUI_CHECK_XML_PARSERS

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
CEGUI_ENABLE_IRRLICHT_RENDERER
CEGUI_ENABLE_DIRECTFB_RENDERER

# decide whether to use gtk dialog in samples
CEGUI_CHECK_GTK_FOR_SAMPLES

# decide whether to build the samples
CEGUI_CHECK_WANTS_SAMPLES

# decide whether to build the lua module
CEGUI_CHECK_LUA

dnl versioning info for libtool
dnl Note this is the ABI version which is not the same as our actual library version
CEGUI_CURRENT=3
CEGUI_REVISION=0
CEGUI_AGE=2
CEGUI_VERSION_INFO=$CEGUI_CURRENT:$CEGUI_REVISION:$CEGUI_AGE
AC_SUBST(CEGUI_VERSION_INFO)

dnl versioning info for the Irrlicht renderer
CEGUI_IRRLICHT_CURRENT=1
CEGUI_IRRLICHT_REVISION=0
CEGUI_IRRLICHT_AGE=1
CEGUI_IRRLICHT_VERSION_INFO=$CEGUI_IRRLICHT_CURRENT:$CEGUI_IRRLICHT_REVISION:$CEGUI_IRRLICHT_AGE
AC_SUBST(CEGUI_IRRLICHT_VERSION_INFO)

dnl versioning info for the OpenGL renderer
CEGUI_OPENGL_CURRENT=1
CEGUI_OPENGL_REVISION=0
CEGUI_OPENGL_AGE=1
CEGUI_OPENGL_VERSION_INFO=$CEGUI_OPENGL_CURRENT:$CEGUI_OPENGL_REVISION:$CEGUI_OPENGL_AGE
AC_SUBST(CEGUI_OPENGL_VERSION_INFO)

dnl versioning info for the DirectFB renderer
CEGUI_DIRECTFB_CURRENT=0
CEGUI_DIRECTFB_REVISION=0
CEGUI_DIRECTFB_AGE=0
CEGUI_DIRECTFB_VERSION_INFO=$CEGUI_DIRECTFB_CURRENT:$CEGUI_DIRECTFB_REVISION:$CEGUI_DIRECTFB_AGE
AC_SUBST(CEGUI_DIRECTFB_VERSION_INFO)

CEGUI_CODE_OPTIONS

dnl Create the makefiles
AC_CONFIG_FILES([Makefile
                 ImageCodecModules/Makefile
                 ImageCodecModules/CoronaImageCodec/Makefile
                 ImageCodecModules/DevILImageCodec/Makefile
                 ImageCodecModules/FreeImageImageCodec/Makefile
                 ImageCodecModules/SILLYImageCodec/Makefile
                 ImageCodecModules/TGAImageCodec/Makefile
                 RendererModules/directx81GUIRenderer/Makefile
                 RendererModules/directx9GUIRenderer/Makefile
                 RendererModules/directx10GUIRenderer/Makefile
                 RendererModules/directfbRenderer/Makefile
                 RendererModules/IrrlichtRenderer/Makefile
                 RendererModules/Makefile
                 RendererModules/OpenGLGUIRenderer/Makefile
                 Samples/Demo6/Makefile
                 Samples/Demo6/src/Makefile
                 Samples/Demo7/Makefile
                 Samples/Demo7/src/Makefile
                 Samples/Demo8/Makefile
                 Samples/Demo8/src/Makefile
                 Samples/DragDropDemo/Makefile
                 Samples/FalagardDemo1/Makefile
                 Samples/FalagardDemo1/include/Makefile
                 Samples/FalagardDemo1/src/Makefile
                 Samples/FirstWindow/Makefile
                 Samples/FirstWindow/include/Makefile
                 Samples/FirstWindow/src/Makefile
                 Samples/FontDemo/Makefile
                 Samples/FontDemo/src/Makefile
                 Samples/InstancedWindows/Makefile
                 Samples/InstancedWindows/include/Makefile
                 Samples/InstancedWindows/src/Makefile
                 Samples/Minesweeper/Makefile
                 Samples/Minesweeper/src/Makefile
                 Samples/ScrollablePaneDemo/Makefile
                 Samples/ScrollablePaneDemo/src/Makefile
                 Samples/TabControlDemo/Makefile
                 Samples/TabControlDemo/src/Makefile
                 Samples/TextDemo/Makefile
                 Samples/TextDemo/src/Makefile
                 Samples/TreeDemo/Makefile
                 Samples/TreeDemo/include/Makefile
                 Samples/TreeDemo/src/Makefile
                 Samples/Makefile
                 Samples/common/Makefile
                 Samples/common/include/Makefile
                 Samples/common/src/Makefile
                 Samples/datafiles/Makefile
                 ScriptingModules/CEGUILua/LuaScriptModule/Makefile
                 ScriptingModules/CEGUILua/LuaScriptModule/include/Makefile
                 ScriptingModules/CEGUILua/LuaScriptModule/package/Makefile
                 ScriptingModules/CEGUILua/LuaScriptModule/src/Makefile
                 ScriptingModules/CEGUILua/Makefile
                 ScriptingModules/CEGUILua/tolua++/Makefile
                 ScriptingModules/CEGUILua/tolua++bin/Makefile
                 ScriptingModules/Makefile
                 WindowRendererSets/Makefile
                 WindowRendererSets/Falagard/Makefile
                 WindowRendererSets/Falagard/include/Makefile
                 WindowRendererSets/Falagard/src/Makefile
                 XMLParserModules/Makefile
                 XMLParserModules/TinyXMLParser/Makefile
                 XMLParserModules/XercesParser/Makefile
                 XMLParserModules/expatParser/Makefile
                 XMLParserModules/libxmlParser/Makefile
                 include/Makefile
                 include/elements/Makefile
                 include/falagard/Makefile
                 makefiles/Makefile
                 src/Makefile
                 CEGUI.pc
                 CEGUI-OPENGL.pc
                 CEGUI.spec])
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
echo "* Renderer Modules:"
echo "*         Building OpenGL Renderer:                     ${cegui_enable_opengl-no}"
echo "*             Using external GLEW library:              ${cegui_found_glew-no}"
echo "*         Building Irrlicht Renderer:                   ${cegui_enable_irrlicht-no}"
echo "*         Building DirectFB Renderer:                   ${cegui_enable_directfb-no}"
echo "*"
if test x"$cegui_enable_opengl" = x"yes"; then
echo "* Image Loading Codec Modules (currently for OpenGL Renderer only):"
echo "*         Building Corona Image Codec:                  ${cegui_with_corona-no}"
echo "*         Building DevIL Image Codec:                   ${cegui_with_devil-no}"
echo "*         Building FreeImage Image Codec:               ${cegui_with_freeimage-no}"
echo "*         Building SILLY Image Codec:                   ${cegui_with_silly-no}"
echo "*         Building TGA Image Codec:                     ${cegui_with_tga-no}"
echo "*"
echo "*         Default Image Codec will be:                  ${cegui_default_image_codec}"
echo "*"
fi
echo "* XML Parser Modules:"
echo "*         Building TinyXMLParser:                       ${cegui_with_tinyxml-no}"
if test x$cegui_with_tinyxml = xyes; then
echo "*             Using external TinyXML library:           ${cegui_found_tinyxml-no}"
fi
echo "*         Building ExpatParser:                         ${cegui_with_expat-no}"
echo "*         Building LibXMLParser:                        ${cegui_with_libxml-no}"
echo "*         Building XercesParser:                        ${cegui_with_xerces-no}"
echo "*"
echo "*         Default XML Parser is:                        ${cegui_default_parser-none}"
if test x"$cegui_default_parser" != x"XercesParser" &&
   test x"$cegui_default_parser" != x"ExpatParser" &&
   test x"$cegui_default_parser" != x"LibxmlParser" &&
   test x"$cegui_default_parser" != x"TinyXMLParser"; then
echo "*"
echo "*         WARNING!  Default parser is not set to one of the pre-made parsers:"
echo "*         WARNING!  XercesParser, ExpatParser, LibxmlParser, or TinyXMLParser."
echo "*         WARNING!"
echo "*         WARNING!  This is fine if you meant to do this and are using a custom"
echo "*         WARNING!  parser as the 'hard-coded' default, else you should go back"
echo "*         WARNING!  and check the config values specified for building parsers"
echo "*         WARNING!  and setting the default parser (--with-default-xml-parser)."
fi
echo "*"
echo "* Scripting:"
echo "*         Building Lua scripting module:                ${cegui_with_lua-no}"
echo "*         Building tolua++cegui generator:              ${cegui_build_toluatool-no}"
if test x$cegui_with_lua = xyes; then
echo "*             Using external tolua++ library:           ${cegui_found_toluapp-no}"
fi
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
