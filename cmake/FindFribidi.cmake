################################################################################
# Custom cmake module for CEGUI to find the fribidi library & header
################################################################################
include(FindPackageHandleStandardArgs)

find_path(FRIBIDI_H_PATH NAMES fribidi.h PATH_SUFFIXES fribidi)
find_library(FRIBIDI_LIB NAMES fribidi PATH_SUFFIXES dynamic/${CEGUI_ARCH_SUFFIX})
find_library(FRIBIDI_LIB_DBG NAMES fribidi_d PATH_SUFFIXES dynamic/${CEGUI_ARCH_SUFFIX})
mark_as_advanced(FRIBIDI_H_PATH FRIBIDI_LIB FRIBIDI_LIB_DBG)

if (WIN32 OR APPLE)
    find_library(FRIBIDI_LIB_STATIC NAMES fribidi PATH_SUFFIXES static/${CEGUI_ARCH_SUFFIX})
    find_library(FRIBIDI_LIB_STATIC_DBG NAMES fribidi_d PATH_SUFFIXES static/${CEGUI_ARCH_SUFFIX})
    mark_as_advanced(FRIBIDI_LIB_STATIC FRIBIDI_LIB_STATIC_DBG)
endif()

cegui_find_package_handle_standard_args(Fribidi FRIBIDI_LIB FRIBIDI_H_PATH)


# set up output vars
if (FRIBIDI_FOUND)
    set (FRIBIDI_INCLUDE_DIR ${FRIBIDI_H_PATH})
    set (FRIBIDI_LIBRARIES ${FRIBIDI_LIB})
    if (FRIBIDI_LIB_DBG)
        set (FRIBIDI_LIBRARIES_DBG ${FRIBIDI_LIB_DBG})
    endif()
    if (FRIBIDI_LIB_STATIC)
        set (FRIBIDI_LIBRARIES_STATIC ${FRIBIDI_LIB_STATIC})
    endif()
    if (FRIBIDI_LIB_STATIC_DBG)
        set (FRIBIDI_LIBRARIES_STATIC_DBG ${FRIBIDI_LIB_STATIC_DBG})
    endif()
else()
    set (FRIBIDI_INCLUDE_DIR)
    set (FRIBIDI_LIBRARIES)
    set (FRIBIDI_LIBRARIES_DBG)
    set (FRIBIDI_LIBRARIES_STATIC)
    set (FRIBIDI_LIBRARIES_STATIC_DBG)
endif()

