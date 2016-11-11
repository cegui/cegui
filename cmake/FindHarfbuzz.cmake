################################################################################
# Custom cmake module for CEGUI to find harfbuzz
################################################################################
include(FindPackageHandleStandardArgs)

find_path(HARFBUZZ_H_PATH NAMES hb.h PATH_SUFFIXES harfbuzz/src)
find_library(HARFBUZZ_LIB NAMES harfbuzz PATH_SUFFIXES dynamic)
find_library(HARFBUZZ_LIB_DBG NAMES harfbuzz_d PATH_SUFFIXES dynamic)
mark_as_advanced(HARFBUZZ_H_PATH HARFBUZZ_LIB HARFBUZZ_LIB_DBG)

if (WIN32 OR APPLE)
    find_library(HARFBUZZ_LIB_STATIC NAMES harfbuzz PATH_SUFFIXES static)
    find_library(HARFBUZZ_LIB_DBG_STATIC NAMES harfbuzz_d PATH_SUFFIXES static)
    mark_as_advanced(HARFBUZZ_LIB_STATIC HARFBUZZ_LIB_DBG_STATIC)
endif ()

cegui_find_package_handle_standard_args(harfbuzz HARFBUZZ_LIB HARFBUZZ_H_PATH)

# set up output vars
if (HARFBUZZ_FOUND)
    set (HARFBUZZ_INCLUDE_DIR ${HARFBUZZ_H_PATH})
    set (HARFBUZZ_LIBRARIES ${HARFBUZZ_LIB})
    if (HARFBUZZ_LIB_DBG)
        set (HARFBUZZ_LIBRARIES_DBG ${HARFBUZZ_LIB_DBG})
    endif()
    if (HARFBUZZ_LIB_STATIC)
        set (HARFBUZZ_LIBRARIES_STATIC ${HARFBUZZ_LIB_STATIC})
    endif()
    if (HARFBUZZ_LIB_STATIC_DBG)
        set (HARFBUZZ_LIBRARIES_STATIC_DBG ${HARFBUZZ_LIB_STATIC_DBG})
    endif()
else()
    set (HARFBUZZ_INCLUDE_DIR)
    set (HARFBUZZ_LIBRARIES)
    set (HARFBUZZ_LIBRARIES_DBG)
    set (HARFBUZZ_LIBRARIES_STATIC)
    set (HARFBUZZ_LIBRARIES_STATIC_DBG)
endif()