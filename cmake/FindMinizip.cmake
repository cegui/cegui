################################################################################
# Custom cmake module for CEGUI to find the MiniZip library
################################################################################
include(FindPackageHandleStandardArgs)

find_path(MINIZIP_H_PATH NAMES minizip/unzip.h)
find_library(MINIZIP_LIB NAMES minizip PATH_SUFFIXES dynamic)
find_library(MINIZIP_LIB_DBG NAMES minizip_d minizipd PATH_SUFFIXES dynamic)
mark_as_advanced(MINIZIP_H_PATH MINIZIP_LIB MINIZIP_LIB_DBG)

if (WIN32 OR APPLE)
    find_library(MINIZIP_LIB_STATIC NAMES minizip PATH_SUFFIXES static)
    find_library(MINIZIP_LIB_STATIC_DBG NAMES minizip_d minizipd PATH_SUFFIXES static)
    mark_as_advanced(MINIZIP_LIB_STATIC MINIZIP_LIB_STATIC_DBG)
endif()

if (APPLE)
    set(MINIZIP_DEFINITIONS USE_FILE32API)
    mark_as_advanced(MINIZIP_DEFINITIONS)
endif()

cegui_find_package_handle_standard_args(MINIZIP MINIZIP_LIB MINIZIP_H_PATH)

# set up output vars
if (MINIZIP_FOUND)
    set (MINIZIP_INCLUDE_DIR ${MINIZIP_H_PATH})
    set (MINIZIP_LIBRARIES ${MINIZIP_LIB})
    if (MINIZIP_LIB_DBG)
        set (MINIZIP_LIBRARIES_DBG ${MINIZIP_LIB_DBG})
    endif()
    if (MINIZIP_LIB_STATIC)
        set (MINIZIP_LIBRARIES_STATIC ${MINIZIP_LIB_STATIC})
    endif()
    if (MINIZIP_LIB_STATIC_DBG)
        set (MINIZIP_LIBRARIES_STATIC_DBG ${MINIZIP_LIB_STATIC_DBG})
    endif()
else()
    set (MINIZIP_INCLUDE_DIR)
    set (MINIZIP_LIBRARIES)
    set (MINIZIP_LIBRARIES_DBG)
    set (MINIZIP_LIBRARIES_STATIC)
    set (MINIZIP_LIBRARIES_STATIC_DBG)
endif()

