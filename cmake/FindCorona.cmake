################################################################################
# Custom cmake module for CEGUI to find corona
################################################################################
include(FindPackageHandleStandardArgs)

find_path(CORONA_H_PATH NAMES corona.h)
find_library(CORONA_LIB NAMES corona libcorona PATH_SUFFIXES dynamic)
find_library(CORONA_LIB_DBG NAMES corona_d libcorona_d PATH_SUFFIXES dynamic)
mark_as_advanced(CORONA_H_PATH CORONA_LIB CORONA_LIB_DBG)

if (WIN32 OR APPLE)
    find_library(CORONA_LIB_STATIC NAMES corona libcorona PATH_SUFFIXES static)
    find_library(CORONA_LIB_STATIC_DBG NAMES corona_d libcorona_d PATH_SUFFIXES static)

    cegui_find_libraries(CORONA_LIB_STATIC "libpng;jpeg" static)
    cegui_find_libraries(CORONA_LIB_STATIC_DBG "libpng_d;jpeg_d" static)

    mark_as_advanced(CORONA_LIB_STATIC CORONA_LIB_STATIC_DBG)
endif()

cegui_find_package_handle_standard_args(CORONA CORONA_LIB CORONA_H_PATH)

# set up output vars
if (CORONA_FOUND)
    set (CORONA_INCLUDE_DIR ${CORONA_H_PATH})
    set (CORONA_LIBRARIES ${CORONA_LIB})
    if (CORONA_LIB_DBG)
        set (CORONA_LIBRARIES_DBG ${CORONA_LIB_DBG})
    endif()
    if (CORONA_LIB_STATIC)
        set (CORONA_LIBRARIES_STATIC ${CORONA_LIB_STATIC})
    endif()
    if (CORONA_LIB_STATIC_DBG)
        set (CORONA_LIBRARIES_STATIC_DBG ${CORONA_LIB_STATIC_DBG})
    endif()
else()
    set (CORONA_INCLUDE_DIR)
    set (CORONA_LIBRARIES)
    set (CORONA_LIBRARIES_DBG)
    set (CORONA_LIBRARIES_STATIC)
    set (CORONA_LIBRARIES_STATIC_DBG)
endif()

