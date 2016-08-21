################################################################################
# Custom cmake module for CEGUI to find libraqm
################################################################################
include(FindPackageHandleStandardArgs)

find_package(Freetype QUIET)
find_package(Fribidi QUIET)
find_package(Harfbuzz QUIET)


find_path(LIBRAQM_H_PATH NAMES libraqm/src/raqm.h)
find_library(LIBRAQM_LIB NAMES libraqm PATH_SUFFIXES dynamic)
find_library(LIBRAQM_LIB_DBG NAMES libraqm_d PATH_SUFFIXES dynamic)
mark_as_advanced(LIBRAQM_H_PATH LIBRAQM_LIB LIBRAQM_LIB_DBG)

if (WIN32 OR APPLE)
    find_library(LIBRAQM_LIB_STATIC NAMES libraqm PATH_SUFFIXES static)
    find_library(LIBRAQM_LIB_DBG_STATIC NAMES libraqm_d PATH_SUFFIXES static)
    mark_as_advanced(LIBRAQM_LIB_STATIC LIBRAQM_LIB_DBG_STATIC)
endif ()

cegui_find_package_handle_standard_args(libraqm LIBRAQM_LIB LIBRAQM_H_PATH)

set(LIBRAQM_DEPS_FOUND TRUE)

if (WIN32 OR APPLE)
    if(HARFBUZZ_LIB_STATIC)
        if (NOT FREETYPE_FOUND)
            set(LIBRAQM_DEPS_FOUND FALSE)
        endif ()
        if (NOT FRIBIDI_FOUND)
            set(LIBRAQM_DEPS_FOUND FALSE)
        endif ()
        if (NOT HARFBUZZ_FOUND)
            set(LIBRAQM_DEPS_FOUND FALSE)
        endif ()

        if (NOT LIBRAQM_DEPS_FOUND)
            message(STATUS "Could not find all required dependencies for libraqm library.")
            set(LIBRAQM_FOUND FALSE)
        endif ()
    endif ()
endif ()
