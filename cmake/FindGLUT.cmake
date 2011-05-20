################################################################################
# Custom cmake module for CEGUI to find glut / freeglut
################################################################################
include(FindPackageHandleStandardArgs)

find_path(GLUT_H_PATH NAMES GL/glut.h glut.h)
find_library(GLUT_LIB NAMES freeglut glut PATH_SUFFIXES dynamic)
find_library(GLUT_LIB_DBG NAMES freeglut_d glut_d PATH_SUFFIXES dynamic)
mark_as_advanced(GLUT_H_PATH GLUT_LIB GLUT_LIB_DBG)

if (WIN32)
    find_library(GLUT_LIB_STATIC NAMES freeglut glut PATH_SUFFIXES static)
    find_library(GLUT_LIB_STATIC_DBG NAMES freeglut_d glut_d PATH_SUFFIXES static)
    set( GLUT_DEFINITIONS_STATIC "FREEGLUT_STATIC" CACHE STRING "preprocessor definitions" )
    mark_as_advanced(GLUT_DEFINITIONS_STATIC GLUT_LIB_STATIC GLUT_LIB_STATIC_DBG)
endif()

find_package_handle_standard_args(GLUT DEFAULT_MSG GLUT_LIB GLUT_H_PATH)


# set up output vars
if (GLUT_FOUND)
    set (GLUT_INCLUDE_DIR ${GLUT_H_PATH})
    set (GLUT_LIBRARIES ${GLUT_LIB})
    if (GLUT_LIB_DBG)
        set (GLUT_LIBRARIES_DBG ${GLUT_LIB_DBG})
    endif()
    if (GLUT_LIB_STATIC)
        set (GLUT_LIBRARIES_STATIC ${GLUT_LIB_STATIC})
    endif()
    if (GLUT_LIB_STATIC_DBG)
        set (GLUT_LIBRARIES_STATIC_DBG ${GLUT_LIB_STATIC_DBG})
    endif()
else()
    set (GLUT_INCLUDE_DIR)
    set (GLUT_LIBRARIES)
    set (GLUT_LIBRARIES_DBG)
    set (GLUT_LIBRARIES_STATIC)
    set (GLUT_LIBRARIES_STATIC_DBG)
endif()

