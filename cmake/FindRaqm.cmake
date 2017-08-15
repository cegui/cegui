################################################################################
# Custom cmake module for CEGUI to find raqm
################################################################################
include(FindPackageHandleStandardArgs)

find_package(Freetype QUIET)
find_package(Fribidi QUIET)
find_package(Harfbuzz QUIET)


find_path(RAQM_H_PATH NAMES raqm.h PATH_SUFFIXES libraqm/src)
find_library(RAQM_LIB NAMES raqm PATH_SUFFIXES dynamic)
find_library(RAQM_LIB_DBG NAMES raqm_d PATH_SUFFIXES dynamic)
mark_as_advanced(RAQM_H_PATH RAQM_LIB RAQM_LIB_DBG)

if (WIN32 OR APPLE)
    find_library(RAQM_LIB_STATIC NAMES raqm PATH_SUFFIXES static)
    find_library(RAQM_LIB_DBG_STATIC NAMES raqm_d PATH_SUFFIXES static)
    mark_as_advanced(RAQM_LIB_STATIC RAQM_LIB_DBG_STATIC)
endif ()

cegui_find_package_handle_standard_args(raqm RAQM_LIB RAQM_H_PATH)

set(RAQM_DEPS_FOUND TRUE)

if (WIN32 OR APPLE)
    if(HARFBUZZ_LIB_STATIC)
        if (NOT FREETYPE_FOUND)
            set(RAQM_DEPS_FOUND FALSE)
        endif ()
        if (NOT FRIBIDI_FOUND)
            set(RAQM_DEPS_FOUND FALSE)
        endif ()
        if (NOT HARFBUZZ_FOUND)
            set(RAQM_DEPS_FOUND FALSE)
        endif ()

        if (NOT RAQM_DEPS_FOUND)
            message(STATUS "Could not find all required dependencies for raqm library.")
            set(RAQM_FOUND FALSE)
        endif ()
    endif ()
endif ()


# set up output vars
if (RAQM_FOUND)
    set (RAQM_INCLUDE_DIR ${RAQM_H_PATH})
    set (RAQM_LIBRARIES ${RAQM_LIB})
    if (RAQM_LIB_DBG)
        set (RAQM_LIBRARIES_DBG ${RAQM_LIB_DBG})
    endif()
    if (RAQM_LIB_STATIC)
        set (RAQM_LIBRARIES_STATIC ${RAQM_LIB_STATIC})
    endif()
    if (RAQM_LIB_STATIC_DBG)
        set (RAQM_LIBRARIES_STATIC_DBG ${RAQM_LIB_STATIC_DBG})
    endif()
else()
    set (RAQM_INCLUDE_DIR)
    set (RAQM_LIBRARIES)
    set (RAQM_LIBRARIES_DBG)
    set (RAQM_LIBRARIES_STATIC)
    set (RAQM_LIBRARIES_STATIC_DBG)
endif()
