################################################################################
# Custom cmake module for CEGUI to find PCRE
################################################################################
include(FindPackageHandleStandardArgs)

find_path(PCRE_H_PATH NAMES pcre.h)
find_library(PCRE_LIB NAMES PCRE pcre libpcre PATH_SUFFIXES dynamic)
find_library(PCRE_LIB_DBG NAMES pcre_d libpcre_d PATH_SUFFIXES dynamic)
mark_as_advanced(PCRE_H_PATH PCRE_LIB PCRE_LIB_DBG)

if (WIN32 OR APPLE)
    find_library(PCRE_LIB_STATIC NAMES PCRE pcre libpcre PATH_SUFFIXES static)
    find_library(PCRE_LIB_STATIC_DBG NAMES pcre_d libpcre_d PATH_SUFFIXES static)
    set( PCRE_DEFINITIONS_STATIC "PCRE_STATIC" CACHE STRING "preprocessor definitions" )
    mark_as_advanced(PCRE_LIB_STATIC PCRE_LIB_STATIC_DBG PCRE_DEFINITIONS)
endif()

cegui_find_package_handle_standard_args(PCRE PCRE_LIB PCRE_H_PATH)

# set up output vars
if (PCRE_FOUND)
    set (PCRE_INCLUDE_DIR ${PCRE_H_PATH})
    set (PCRE_LIBRARIES ${PCRE_LIB})

    if (PCRE_LIB_DBG)
        set (PCRE_LIBRARIES_DBG ${PCRE_LIB_DBG})
    endif()

    if (PCRE_LIB_STATIC)
        set (PCRE_LIBRARIES_STATIC ${PCRE_LIB_STATIC})
    endif()

    if (PCRE_LIB_STATIC_DBG)
        set (PCRE_LIBRARIES_STATIC_DBG ${PCRE_LIB_STATIC_DBG})
    endif()
else()
    set (PCRE_INCLUDE_DIR)
    set (PCRE_LIBRARIES)
    set (PCRE_LIBRARIES_DBG)
    set (PCRE_LIBRARIES_STATIC)
    set (PCRE_LIBRARIES_STATIC_DBG)
endif()

