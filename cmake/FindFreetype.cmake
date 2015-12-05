################################################################################
# Custom cmake module for CEGUI to find freetype
################################################################################
include(FindPackageHandleStandardArgs)

find_path(FREETYPE_H_PATH_ft2build NAMES ft2build.h PATH_SUFFIXES include/freetype2 include freetype2)
find_path(FREETYPE_H_PATH_ftconfig NAMES freetype/config/ftconfig.h PATH_SUFFIXES freetype2)
if (NOT FREETYPE_H_PATH_ftconfig)
    find_path(FREETYPE_H_PATH_ftconfig NAMES config/ftconfig.h PATH_SUFFIXES include/freetype2 include freetype2)
endif()

find_library(FREETYPE_LIB NAMES freetype2 freetype libfreetype PATH_SUFFIXES dynamic)
find_library(FREETYPE_LIB_DBG NAMES freetype_d libfreetype_d PATH_SUFFIXES dynamic)
mark_as_advanced(FREETYPE_H_PATH_ft2build FREETYPE_H_PATH_ftconfig FREETYPE_LIB FREETYPE_LIB_DBG)

if (WIN32 OR APPLE)
    find_library(FREETYPE_LIB_STATIC NAMES freetype2 freetype libfreetype PATH_SUFFIXES static)
    find_library(FREETYPE_LIB_STATIC_DBG NAMES freetype_d libfreetype_d PATH_SUFFIXES static)
    mark_as_advanced(FREETYPE_LIB_STATIC FREETYPE_LIB_STATIC_DBG)
endif()

cegui_find_package_handle_standard_args(FREETYPE FREETYPE_LIB FREETYPE_H_PATH_ft2build FREETYPE_H_PATH_ftconfig)

# set up output vars
if (FREETYPE_FOUND)
    set (FREETYPE_INCLUDE_DIR ${FREETYPE_H_PATH_ft2build} ${FREETYPE_H_PATH_ftconfig})
    set (FREETYPE_LIBRARIES ${FREETYPE_LIB})

    if (FREETYPE_LIB_DBG)
        set (FREETYPE_LIBRARIES_DBG ${FREETYPE_LIB_DBG})
    endif()

    if (FREETYPE_LIB_STATIC)
        set (FREETYPE_LIBRARIES_STATIC ${FREETYPE_LIB_STATIC})
    endif()

    if (FREETYPE_LIB_STATIC_DBG)
        set (FREETYPE_LIBRARIES_STATIC_DBG ${FREETYPE_LIB_STATIC_DBG})
    endif()
else()
    set (FREETYPE_INCLUDE_DIR)
    set (FREETYPE_LIBRARIES)
    set (FREETYPE_LIBRARIES_DBG)
    set (FREETYPE_LIBRARIES_STATIC)
    set (FREETYPE_LIBRARIES_STATIC_DBG)
endif()

