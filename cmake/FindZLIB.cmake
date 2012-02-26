################################################################################
# Custom cmake module for CEGUI to find zlib
################################################################################
include(FindPackageHandleStandardArgs)

find_path(ZLIB_H_PATH NAMES zlib.h)
find_library(ZLIB_LIB NAMES zlib zdll z PATH_SUFFIXES dynamic)
find_library(ZLIB_LIB_DBG NAMES zlib_d zdll_d z_d PATH_SUFFIXES dynamic)
mark_as_advanced(ZLIB_H_PATH ZLIB_LIB ZLIB_LIB_DBG)

if (WIN32 OR APPLE)
    find_library(ZLIB_LIB_STATIC NAMES zlib zdll z PATH_SUFFIXES static)
    find_library(ZLIB_LIB_STATIC_DBG NAMES zlib_d zdll_d z_d PATH_SUFFIXES static)
    mark_as_advanced(ZLIB_LIB_STATIC ZLIB_LIB_STATIC_DBG)
endif()

cegui_find_package_handle_standard_args(ZLIB ZLIB_LIB ZLIB_H_PATH)


# set up output vars
if (ZLIB_FOUND)
    set (ZLIB_INCLUDE_DIR ${ZLIB_H_PATH})
    set (ZLIB_LIBRARIES ${ZLIB_LIB})
    if (ZLIB_LIB_DBG)
        set (ZLIB_LIBRARIES_DBG ${ZLIB_LIB_DBG})
    endif()
    if (ZLIB_LIB_STATIC)
        set (ZLIB_LIBRARIES_STATIC ${ZLIB_LIB_STATIC})
    endif()
    if (ZLIB_LIB_STATIC_DBG)
        set (ZLIB_LIBRARIES_STATIC_DBG ${ZLIB_LIB_STATIC_DBG})
    endif()
else()
    set (ZLIB_INCLUDE_DIR)
    set (ZLIB_LIBRARIES)
    set (ZLIB_LIBRARIES_DBG)
    set (ZLIB_LIBRARIES_STATIC)
    set (ZLIB_LIBRARIES_STATIC_DBG)
endif()

