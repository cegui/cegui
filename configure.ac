AC_PREREQ(2.59)
AC_INIT([CEGUI], [0.01])
AM_INIT_AUTOMAKE([CEGUI], 0.01)
AC_CONFIG_HEADER([include/config.h])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC

AM_PROG_LIBTOOL

# Checks for libraries.
AC_CHECK_LIB([xerces-c], [main],, AC_MSG_ERROR([cannot find xerces library]))
#AC_HAVE_LIBRARY(boost_signals-gcc,[main] , [AC_MSG_ERROR([cannot find boost libraries])])
PKG_CHECK_MODULES(OGRE, OGRE >= 0.14.0)
AC_SUBST(OGRE_CFLAGS)
AC_SUBST(OGRE_LIBS)

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_CONST
AC_C_INLINE
AC_TYPE_SIZE_T
AC_STRUCT_TM
AC_CHECK_TYPES([ptrdiff_t])

# Checks for library functions.
AC_FUNC_ERROR_AT_LINE
AC_HEADER_STDC
AC_CHECK_FUNCS([memmove memset])

dnl versioning info for libtool
dnl Note this is the ABI version which is not the same as our actual library ver sion
CEGUI_CURRENT=0
CEGUI_REVISION=0
CEGUI_AGE=0
CEGUI_VERSION_INFO=$CEGUI_CURRENT:$CEGUI_REVISION:$CEGUI_AGE
AC_SUBST(CEGUI_VERSION_INFO)

dnl Create the makefiles
AC_CONFIG_FILES([Makefile \
                 include/Makefile \
                 include/elements/Makefile \
                 include/renderers/Makefile \
                 include/renderers/OgreGUIRenderer/Makefile \
                 src/Makefile \
                 src/renderers/Makefile \
                 src/renderers/OgreGUIRenderer/Makefile \
                 WidgetSets/Makefile \
                 WidgetSets/TaharezLook/Makefile \
                 WidgetSets/TaharezLook/src/Makefile \
                 WidgetSets/WindowsLook/Makefile \
                 WidgetSets/WindowsLook/src/Makefile \
                 CEGUI.pc])

AC_OUTPUT
