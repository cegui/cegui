################################################################################
# Custom cmake module for CEGUI to find DirectFB
################################################################################
include(FindPackageHandleStandardArgs)

find_path(DIRECTFB_H_PATH NAMES directfb.h PATH_SUFFIXES directfb)
find_library(DIRECTFB_LIB NAMES directfb PATH_SUFFIXES dynamic)
find_library(DIRECTFB_LIB_DBG NAMES directfb_d PATH_SUFFIXES dynamic)
mark_as_advanced(DIRECTFB_H_PATH DIRECTFB_LIB DIRECTFB_LIB_DBG)

if (WIN32)
    find_library(DIRECTFB_LIB_STATIC NAMES directfb PATH_SUFFIXES static)
    find_library(DIRECTFB_LIB_STATIC_DBG NAMES directfb_d PATH_SUFFIXES static)
    mark_as_advanced(DIRECTFB_LIB_STATIC DIRECTFB_LIB_STATIC_DBG)
endif()

cegui_find_package_handle_standard_args(DIRECTFB DIRECTFB_LIB DIRECTFB_H_PATH)


# set up output vars
if (DIRECTFB_FOUND)
    set (DIRECTFB_INCLUDE_DIR ${DIRECTFB_H_PATH})
    set (DIRECTFB_LIBRARIES ${DIRECTFB_LIB})
    if (DIRECTFB_LIB_DBG)
        set (DIRECTFB_LIBRARIES_DBG ${DIRECTFB_LIB_DBG})
    endif()
    if (DIRECTFB_LIB_STATIC)
        set (DIRECTFB_LIBRARIES_STATIC ${DIRECTFB_LIB_STATIC})
    endif()
    if (DIRECTFB_LIB_STATIC_DBG)
        set (DIRECTFB_LIBRARIES_STATIC_DBG ${DIRECTFB_LIB_STATIC_DBG})
    endif()
else()
    set (DIRECTFB_INCLUDE_DIR)
    set (DIRECTFB_LIBRARIES)
    set (DIRECTFB_LIBRARIES_DBG)
    set (DIRECTFB_LIBRARIES_STATIC)
    set (DIRECTFB_LIBRARIES_STATIC_DBG)
endif()

