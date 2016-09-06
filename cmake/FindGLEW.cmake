################################################################################
# Custom cmake module for CEGUI to find GLEW
################################################################################
include(FindPackageHandleStandardArgs)

find_path(GLEW_H_PATH NAMES GL/glew.h)
find_library(GLEW_LIB NAMES GLEW libGLEW glew libglew PATH_SUFFIXES dynamic)
find_library(GLEW_LIB_DBG NAMES GLEW_d libGLEW_d glew_d libglew_d PATH_SUFFIXES dynamic)
mark_as_advanced(GLEW_H_PATH GLEW_LIB GLEW_LIB_DBG)

if (WIN32 OR APPLE)
    find_library(GLEW_LIB_STATIC NAMES GLEW libGLEW glew libglew PATH_SUFFIXES static)
    find_library(GLEW_LIB_STATIC_DBG NAMES GLEW_d libGLEW_d glew_d libglew_d PATH_SUFFIXES static)
    mark_as_advanced(GLEW_LIB_STATIC GLEW_LIB_STATIC_DBG)
endif()

cegui_find_package_handle_standard_args(GLEW GLEW_LIB GLEW_H_PATH)


# set up output vars
if (GLEW_FOUND)
    set (GLEW_INCLUDE_DIR ${GLEW_H_PATH})
    set (GLEW_LIBRARIES ${GLEW_LIB})
    if (GLEW_LIB_DBG)
        set (GLEW_LIBRARIES_DBG ${GLEW_LIB_DBG})
    endif()
    if (GLEW_LIB_STATIC)
        set (GLEW_LIBRARIES_STATIC ${GLEW_LIB_STATIC})
    endif()
    if (GLEW_LIB_STATIC_DBG)
        set (GLEW_LIBRARIES_STATIC_DBG ${GLEW_LIB_STATIC_DBG})
    endif()
else()
    set (GLEW_INCLUDE_DIR)
    set (GLEW_LIBRARIES)
    set (GLEW_LIBRARIES_DBG)
    set (GLEW_LIBRARIES_STATIC)
    set (GLEW_LIBRARIES_STATIC_DBG)
endif()

