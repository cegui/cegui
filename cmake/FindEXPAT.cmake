################################################################################
# Custom cmake module for CEGUI to find the Expat XML parser
################################################################################
include(FindPackageHandleStandardArgs)

find_path(EXPAT_H_PATH NAMES expat.h)
find_library(EXPAT_LIB NAMES expat libexpat PATH_SUFFIXES dynamic)
find_library(EXPAT_LIB_DBG NAMES expat_d libexpat_d PATH_SUFFIXES dynamic)
mark_as_advanced(EXPAT_H_PATH EXPAT_LIB EXPAT_LIB_DBG)

if (WIN32 OR APPLE)
    find_library(EXPAT_LIB_STATIC NAMES expat libexpat PATH_SUFFIXES static)
    find_library(EXPAT_LIB_STATIC_DBG NAMES expat_d libexpat_d PATH_SUFFIXES static)
    set( EXPAT_DEFINITIONS "XML_STATIC" CACHE STRING "preprocessor definitions" )
    mark_as_advanced(EXPAT_DEFINITIONS EXPAT_LIB_STATIC EXPAT_LIB_STATIC_DBG)
endif()

cegui_find_package_handle_standard_args(EXPAT EXPAT_LIB EXPAT_H_PATH)


# set up output vars
if (EXPAT_FOUND)
    set (EXPAT_INCLUDE_DIR ${EXPAT_H_PATH})
    set (EXPAT_LIBRARIES ${EXPAT_LIB})
    if (EXPAT_LIB_DBG)
        set (EXPAT_LIBRARIES_DBG ${EXPAT_LIB_DBG})
    endif()
    if (EXPAT_LIB_STATIC)
        set (EXPAT_LIBRARIES_STATIC ${EXPAT_LIB_STATIC})
    endif()
    if (EXPAT_LIB_STATIC_DBG)
        set (EXPAT_LIBRARIES_STATIC_DBG ${EXPAT_LIB_STATIC_DBG})
    endif()
else()
    set (EXPAT_INCLUDE_DIR)
    set (EXPAT_LIBRARIES)
    set (EXPAT_LIBRARIES_DBG)
    set (EXPAT_LIBRARIES_STATIC)
    set (EXPAT_LIBRARIES_STATIC_DBG)
endif()

