################################################################################
# Custom cmake module for CEGUI to find tinyxml2
################################################################################
find_path(TINYXML2_H_PATH NAMES tinyxml2.h)
find_library(TINYXML2_LIB NAMES tinyxml2 libtinyxml2 PATH_SUFFIXES dynamic)
find_library(TINYXML2_LIB_DBG NAMES tinyxml2_d libtinyxml2_d PATH_SUFFIXES dynamic)
mark_as_advanced(TINYXML2_H_PATH TINYXML2_LIB TINYXML2_LIB_DBG)

if (WIN32 OR APPLE)
    find_library(TINYXML2_LIB_STATIC NAMES tinyxml2 libtinyxml2 PATH_SUFFIXES static)
    find_library(TINYXML2_LIB_STATIC_DBG NAMES tinyxml2_d libtinyxml2_d PATH_SUFFIXES static)
    mark_as_advanced(TINYXML2_LIB_STATIC TINYXML2_LIB_STATIC_DBG)
endif()

cegui_find_package_handle_standard_args(TINYXML2 TINYXML2_LIB TINYXML2_H_PATH)

if (TINYXML2_FOUND)
    set (TINYXML2_INCLUDE_DIR ${TINYXML2_H_PATH})
    set (TINYXML2_LIBRARIES ${TINYXML2_LIB})
    if (TINYXML2_LIB_DBG)
        set (TINYXML2_LIBRARIES_DBG ${TINYXML2_LIB_DBG})
    endif()
    if (TINYXML2_LIB_STATIC)
        set (TINYXML2_LIBRARIES_STATIC ${TINYXML2_LIB_STATIC})
    endif()
    if (TINYXML2_LIB_STATIC_DBG)
        set (TINYXML2_LIBRARIES_STATIC_DBG ${TINYXML2_LIB_STATIC_DBG})
    endif()
else()
    set (TINYXML2_INCLUDE_DIR)
    set (TINYXML2_LIBRARIES)
    set (TINYXML2_LIBRARIES_DBG)
    set (TINYXML2_LIBRARIES_STATIC)
    set (TINYXML2_LIBRARIES_STATIC_DBG)
endif()

