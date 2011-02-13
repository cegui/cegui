################################################################################
# Custom cmake module for CEGUI to find DirectFB
################################################################################
include(FindPackageHandleStandardArgs)

find_path(DIRECTFB_H_PATH NAMES directfb.h PATH_SUFFIXES directfb)
find_library(DIRECTFB_LIB NAMES directfb)
find_library(DIRECTFB_LIB_DBG NAMES directfb_d)

find_package_handle_standard_args(DIRECTFB DEFAULT_MSG DIRECTFB_LIB DIRECTFB_H_PATH)

mark_as_advanced(DIRECTFB_H_PATH DIRECTFB_LIB DIRECTFB_LIB_DBG)

# set up output vars
if (DIRECTFB_FOUND)
    set (DIRECTFB_INCLUDE_DIR ${DIRECTFB_H_PATH})
    set (DIRECTFB_LIBRARIES ${DIRECTFB_LIB})
    if (DIRECTFB_LIB_DBG)
        set (DIRECTFB_LIBRARIES_DBG ${DIRECTFB_LIB_DBG})
    endif()
else()
    set (DIRECTFB_INCLUDE_DIR)
    set (DIRECTFB_LIBRARIES)
    set (DIRECTFB_LIBRARIES_DBG)
endif()

