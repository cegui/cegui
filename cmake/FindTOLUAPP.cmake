################################################################################
# Custom cmake module for CEGUI to find tolua++
################################################################################
include(FindPackageHandleStandardArgs)

find_path(TOLUAPP_H_PATH NAMES tolua++.h)
find_library(TOLUAPP_LIB NAMES tolua++5.1 tolua++)
find_library(TOLUAPP_LIB_DBG NAMES tolua++5.1_d tolua++_d)

find_package_handle_standard_args(TOLUAPP DEFAULT_MSG TOLUAPP_LIB TOLUAPP_H_PATH)

mark_as_advanced(TOLUAPP_H_PATH TOLUAPP_LIB TOLUAPP_LIB_DBG)

# set up output vars
if (TOLUAPP_FOUND)
    set (TOLUAPP_INCLUDE_DIR ${TOLUAPP_H_PATH})
    set (TOLUAPP_LIBRARIES ${TOLUAPP_LIB})
    if (TOLUAPP_LIB_DBG)
        set (TOLUAPP_LIBRARIES_DBG ${TOLUAPP_LIB_DBG})
    endif()
else()
    set (TOLUAPP_INCLUDE_DIR)
    set (TOLUAPP_LIBRARIES)
    set (TOLUAPP_LIBRARIES_DBG)
endif()

