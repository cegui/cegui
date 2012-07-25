################################################################################
# Custom cmake module for CEGUI to find glfw
################################################################################
include(FindPackageHandleStandardArgs)

find_path(GLFW_H_PATH NAMES GL/glfw.h glfw.h)
find_library(GLFW_LIB NAMES glfw PATH_SUFFIXES dynamic)
find_library(GLFW_LIB_DBG NAMES glfw_d PATH_SUFFIXES dynamic)
mark_as_advanced(GLFW_H_PATH GLFW_LIB GLFW_LIB_DBG)

if (WIN32)
    set( GLFW_DEFINITIONS_DYNAMIC "GLFW_DLL" CACHE STRING "preprocessor definitions" )
    mark_as_advanced(GLFW_DEFINITIONS_DYNAMIC)
endif()

if (WIN32 OR APPLE)
    find_library(GLFW_LIB_STATIC NAMES glfw PATH_SUFFIXES static)
    find_library(GLFW_LIB_STATIC_DBG NAMES glfw_d PATH_SUFFIXES static)
    mark_as_advanced(GLFW_LIB_STATIC GLFW_LIB_STATIC_DBG)
endif()

cegui_find_package_handle_standard_args(GLFW GLFW_LIB GLFW_H_PATH)


# set up output vars
if (GLFW_FOUND)
    set (GLFW_INCLUDE_DIR ${GLFW_H_PATH})
    set (GLFW_LIBRARIES ${GLFW_LIB})
    if (GLFW_LIB_DBG)
        set (GLFW_LIBRARIES_DBG ${GLFW_LIB_DBG})
    endif()
    if (GLFW_LIB_STATIC)
        set (GLFW_LIBRARIES_STATIC ${GLFW_LIB_STATIC})
    endif()
    if (GLFW_LIB_STATIC_DBG)
        set (GLFW_LIBRARIES_STATIC_DBG ${GLFW_LIB_STATIC_DBG})
    endif()
else()
    set (GLFW_INCLUDE_DIR)
    set (GLFW_LIBRARIES)
    set (GLFW_LIBRARIES_DBG)
    set (GLFW_LIBRARIES_STATIC)
    set (GLFW_LIBRARIES_STATIC_DBG)
endif()

