AC_PREREQ(2.59)
AC_INIT([CEGUI], [0.5.0])
AM_INIT_AUTOMAKE([CEGUI], 0.5.0)
AC_CONFIG_HEADER([include/config.h])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC

# Disable building static library
AC_DISABLE_STATIC
AM_PROG_LIBTOOL

# Checks for libraries.
AC_CHECK_LIB([dl], [main])
AC_CHECK_LIB([pthread], [main],, AC_MSG_ERROR([cannot find pthread library]))

CEGUI_CHECK_XML_PARSERS

PKG_CHECK_MODULES(freetype2, freetype2 >= 0.15.0)
AC_SUBST(freetype2_CFLAGS)
AC_SUBST(freetype2_LIBS)

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

# decide whether to use gtk dialog in samples
CEGUI_CHECK_GTK_FOR_SAMPLES

# decide whether to build the samples
CEGUI_CHECK_WANTS_SAMPLES

dnl versioning info for libtool
dnl Note this is the ABI version which is not the same as our actual library version
CEGUI_CURRENT=1
CEGUI_REVISION=0
CEGUI_AGE=0
CEGUI_VERSION_INFO=$CEGUI_CURRENT:$CEGUI_REVISION:$CEGUI_AGE
AC_SUBST(CEGUI_VERSION_INFO)

dnl versioning info for the Irrlicht renderer
CEGUI_IRRLICHT_CURRENT=0
CEGUI_IRRLICHT_REVISION=1
CEGUI_IRRLICHT_AGE=0
CEGUI_IRRLICHT_VERSION_INFO=$CEGUI_IRRLICHT_CURRENT:$CEGUI_IRRLICHT_REVISION:$CEGUI_IRRLICHT_AGE
AC_SUBST(CEGUI_IRRLICHT_VERSION_INFO)

dnl versioning info for the OpenGL renderer
CEGUI_OPENGL_CURRENT=0
CEGUI_OPENGL_REVISION=1
CEGUI_OPENGL_AGE=0
CEGUI_OPENGL_VERSION_INFO=$CEGUI_OPENGL_CURRENT:$CEGUI_OPENGL_REVISION:$CEGUI_OPENGL_AGE
AC_SUBST(CEGUI_OPENGL_VERSION_INFO)

dnl Create the makefiles
AC_CONFIG_FILES([Makefile
                 Samples/Demo4/Makefile
                 Samples/Demo4/src/Makefile
                 Samples/Demo6/Makefile
                 Samples/Demo6/src/Makefile
                 Samples/Demo7/Makefile
                 Samples/Demo7/src/Makefile
                 Samples/Demo8/Makefile
                 Samples/Demo8/src/Makefile
                 Samples/FalagardDemo1/Makefile
                 Samples/FalagardDemo1/include/Makefile
                 Samples/FalagardDemo1/src/Makefile
                 Samples/FirstWindow/Makefile
                 Samples/FirstWindow/include/Makefile
                 Samples/FirstWindow/src/Makefile
                 Samples/Makefile
                 Samples/common/Makefile
                 Samples/common/include/Makefile
                 Samples/common/src/Makefile
                 ScriptingModules/CEGUILua/LuaScriptModule/Makefile
                 ScriptingModules/CEGUILua/LuaScriptModule/include/Makefile
                 ScriptingModules/CEGUILua/LuaScriptModule/package/Makefile
                 ScriptingModules/CEGUILua/LuaScriptModule/src/Makefile
                 ScriptingModules/CEGUILua/Makefile
                 ScriptingModules/CEGUILua/lua_and_tolua++/Makefile
                 ScriptingModules/CEGUILua/lua_and_tolua++/include/Makefile
                 ScriptingModules/CEGUILua/lua_and_tolua++/src/Makefile
                 ScriptingModules/Makefile
                 WindowRendererSets/Makefile
                 WindowRendererSets/Falagard/Makefile
                 WindowRendererSets/Falagard/include/Makefile
                 WindowRendererSets/Falagard/src/Makefile
                 include/Makefile
                 include/elements/Makefile
                 include/falagard/Makefile
                 include/renderers/IrrlichtRenderer/Makefile
                 include/renderers/Makefile
                 include/renderers/OpenGLGUIRenderer/Makefile
                 src/Makefile
                 src/renderers/IrrlichtRenderer/Makefile
                 src/renderers/Makefile
                 src/renderers/OpenGLGUIRenderer/Makefile
                 CEGUI.pc
                 CEGUI-OPENGL.pc
                 CEGUI.spec])
AC_OUTPUT
