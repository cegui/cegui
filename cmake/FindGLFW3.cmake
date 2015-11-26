################################################################################
# Custom cmake module for CEGUI to find glfw3
################################################################################
include(FindPackageHandleStandardArgs)

find_path(GLFW3_H_PATH NAMES GLFW/glfw3.h)
find_library(GLFW3_LIB NAMES glfw3 glfw3dll.a PATH_SUFFIXES dynamic)
find_library(GLFW3_LIB_DBG NAMES glfw3_d glfw3_ddll.a PATH_SUFFIXES dynamic)
if (NOT (GLFW3_LIB OR GLFW3_LIB_DBG))
    find_library(GLFW3_LIB NAMES glfw glfwdll.a PATH_SUFFIXES dynamic)
    find_library(GLFW3_LIB_DBG NAMES glfw_d glfw_ddll.a PATH_SUFFIXES dynamic)
endif ()
mark_as_advanced(GLFW3_H_PATH GLFW3_LIB GLFW3_LIB_DBG)

if (WIN32)
    set(GLFW3_DEFINITIONS_DYNAMIC "GLFW3_DLL" CACHE STRING "preprocessor definitions" )
    mark_as_advanced(GLFW3_DEFINITIONS_DYNAMIC)
endif()

if (WIN32 OR APPLE)
    find_library(GLFW3_LIB_STATIC NAMES glfw3 glfw PATH_SUFFIXES static)
    find_library(GLFW3_LIB_STATIC_DBG NAMES glfw3_d glfw_d PATH_SUFFIXES static)
    mark_as_advanced(GLFW3_LIB_STATIC GLFW3_LIB_STATIC_DBG)
endif()

cegui_find_package_handle_standard_args(GLFW3 GLFW3_LIB GLFW3_H_PATH)


# set up output vars
if (GLFW3_FOUND)
    set (GLFW3_INCLUDE_DIR ${GLFW3_H_PATH})
    set (GLFW3_LIBRARIES ${GLFW3_LIB})
    if (GLFW3_LIB_DBG)
        set (GLFW3_LIBRARIES_DBG ${GLFW3_LIB_DBG})
    endif()
    if (GLFW3_LIB_STATIC)
        set (GLFW3_LIBRARIES_STATIC ${GLFW3_LIB_STATIC})
    endif()
    if (GLFW3_LIB_STATIC_DBG)
        set (GLFW3_LIBRARIES_STATIC_DBG ${GLFW3_LIB_STATIC_DBG})
    endif()
else()
    set (GLFW3_INCLUDE_DIR)
    set (GLFW3_LIBRARIES)
    set (GLFW3_LIBRARIES_DBG)
    set (GLFW3_LIBRARIES_STATIC)
    set (GLFW3_LIBRARIES_STATIC_DBG)
endif()

