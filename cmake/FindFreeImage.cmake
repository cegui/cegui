################################################################################
# Custom cmake module for CEGUI to find FreeImage 
################################################################################
include(FindPackageHandleStandardArgs)

find_path(FREEIMAGE_H_PATH NAMES FreeImage.h)
find_library(FREEIMAGE_LIB NAMES freeimage libfreeimage FreeImage libFreeImage PATH_SUFFIXES dynamic)
find_library(FREEIMAGE_LIB_DBG NAMES freeimaged libfreeimaged FreeImage_d libFreeImage_d PATH_SUFFIXES dynamic)
mark_as_advanced(FREEIMAGE_H_PATH FREEIMAGE_LIB FREEIMAGE_LIB_DBG)

if (WIN32 OR APPLE)
    find_library(FREEIMAGE_LIB_STATIC NAMES freeimage libfreeimage FreeImage libFreeImage PATH_SUFFIXES static)
    find_library(FREEIMAGE_LIB_STATIC_DBG NAMES freeimaged libfreeimaged FreeImage_d libFreeImage_d PATH_SUFFIXES static)
    set( FREEIMAGE_DEFINITIONS_STATIC "FREEIMAGE_LIB" CACHE STRING "preprocessor definitions" )
    mark_as_advanced(FREEIMAGE_DEFINITIONS_STATIC FREEIMAGE_LIB_STATIC FREEIMAGE_LIB_STATIC_DBG)
endif()

cegui_find_package_handle_standard_args(FREEIMAGE FREEIMAGE_LIB FREEIMAGE_H_PATH)


# set up output vars
if (FREEIMAGE_FOUND)
    set (FREEIMAGE_INCLUDE_DIR ${FREEIMAGE_H_PATH})
    set (FREEIMAGE_LIBRARIES ${FREEIMAGE_LIB})
    if (FREEIMAGE_LIB_DBG)
        set (FREEIMAGE_LIBRARIES_DBG ${FREEIMAGE_LIB_DBG})
    endif()
    if (FREEIMAGE_LIB_STATIC)
        set (FREEIMAGE_LIBRARIES_STATIC ${FREEIMAGE_LIB_STATIC})
    endif()
    if (FREEIMAGE_LIB_STATIC_DBG)
        set (FREEIMAGE_LIBRARIES_STATIC_DBG ${FREEIMAGE_LIB_STATIC_DBG})
    endif()
else()
    set (FREEIMAGE_INCLUDE_DIR)
    set (FREEIMAGE_LIBRARIES)
    set (FREEIMAGE_LIBRARIES_DBG)
    set (FREEIMAGE_LIBRARIES_STATIC)
    set (FREEIMAGE_LIBRARIES_STATIC_DBG)
endif()

