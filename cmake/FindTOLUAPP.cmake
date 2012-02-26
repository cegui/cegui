################################################################################
# Custom cmake module for CEGUI to find tolua++
################################################################################
include(FindPackageHandleStandardArgs)

find_path(TOLUAPP_H_PATH NAMES tolua++.h)
find_library(TOLUAPP_LIB NAMES tolua++5.1 tolua++ toluapp PATH_SUFFIXES dynamic)
find_library(TOLUAPP_LIB_DBG NAMES tolua++5.1_d tolua++_d toluapp_d PATH_SUFFIXES dynamic)
mark_as_advanced(TOLUAPP_H_PATH TOLUAPP_LIB TOLUAPP_LIB_DBG)

if (WIN32 OR APPLE)
    find_library(TOLUAPP_LIB_STATIC NAMES tolua++5.1 tolua++ toluapp PATH_SUFFIXES static)
    find_library(TOLUAPP_LIB_STATIC_DBG NAMES tolua++5.1_d tolua++_d toluapp_d PATH_SUFFIXES static)
    mark_as_advanced(TOLUAPP_LIB_STATIC TOLUAPP_LIB_STATIC_DBG)
endif()

cegui_find_package_handle_standard_args(TOLUAPP TOLUAPP_LIB TOLUAPP_H_PATH)


# set up output vars
if (TOLUAPP_FOUND)
    set (TOLUAPP_INCLUDE_DIR ${TOLUAPP_H_PATH})
    set (TOLUAPP_LIBRARIES ${TOLUAPP_LIB})
    if (TOLUAPP_LIB_DBG)
        set (TOLUAPP_LIBRARIES_DBG ${TOLUAPP_LIB_DBG})
    endif()
    if (TOLUAPP_LIB_STATIC)
        set (TOLUAPP_LIBRARIES_STATIC ${TOLUAPP_LIB_STATIC})
    endif()
    if (TOLUAPP_LIB_STATIC_DBG)
        set (TOLUAPP_LIBRARIES_STATIC_DBG ${TOLUAPP_LIB_STATIC_DBG})
    endif()
else()
    set (TOLUAPP_INCLUDE_DIR)
    set (TOLUAPP_LIBRARIES)
    set (TOLUAPP_LIBRARIES_DBG)
    set (TOLUAPP_LIBRARIES_STATIC)
    set (TOLUAPP_LIBRARIES_STATIC_DBG)
endif()

