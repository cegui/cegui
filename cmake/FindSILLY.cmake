################################################################################
# Custom cmake module for CEGUI to find SILLY
################################################################################
include(FindPackageHandleStandardArgs)

find_path(SILLY_H_PATH NAMES SILLY.h PATH_SUFFIXES SILLY)
find_library(SILLY_LIB NAMES SILLY libSILLY PATH_SUFFIXES dynamic)
find_library(SILLY_LIB_DBG NAMES SILLY_d libSILLY_d PATH_SUFFIXES dynamic)
mark_as_advanced(SILLY_H_PATH SILLY_LIB SILLY_LIB_DBG)

if (WIN32 OR APPLE)
    find_library(SILLY_LIB_STATIC NAMES SILLY libSILLY PATH_SUFFIXES static)
    find_library(SILLY_LIB_STATIC_DBG NAMES SILLY_d libSILLY_d PATH_SUFFIXES static)

    cegui_find_libraries(SILLY_LIB_STATIC "libpng;jpeg;zlib" static)
    cegui_find_libraries(SILLY_LIB_STATIC_DBG "libpng_d;jpeg_d;zlib_d" static)

    set( SILLY_DEFINITIONS_STATIC "SILLY_STATIC" CACHE STRING "preprocessor definitions" )
    mark_as_advanced(SILLY_DEFINITIONS_STATIC SILLY_LIB_STATIC SILLY_LIB_STATIC_DBG)
endif()

if (UNIX AND NOT APPLE)
    set( SILLY_DEFINITIONS "USE_SILLYOPTIONS_H" CACHE STRING "preprocessor definitions" )
    mark_as_advanced(SILLY_DEFINITIONS)
endif()

cegui_find_package_handle_standard_args(SILLY SILLY_LIB SILLY_H_PATH)


# set up output vars
if (SILLY_FOUND)
    set (SILLY_INCLUDE_DIR ${SILLY_H_PATH})
    set (SILLY_LIBRARIES ${SILLY_LIB})
    if (SILLY_LIB_DBG)
        set (SILLY_LIBRARIES_DBG ${SILLY_LIB_DBG})
    endif()
    if (SILLY_LIB_STATIC)
        set (SILLY_LIBRARIES_STATIC ${SILLY_LIB_STATIC})
    endif()
    if (SILLY_LIB_STATIC_DBG)
        set (SILLY_LIBRARIES_STATIC_DBG ${SILLY_LIB_STATIC_DBG})
    endif()
else()
    set (SILLY_INCLUDE_DIR)
    set (SILLY_LIBRARIES)
    set (SILLY_LIBRARIES_DBG)
    set (SILLY_LIBRARIES_STATIC)
    set (SILLY_LIBRARIES_STATIC_DBG)
endif()

