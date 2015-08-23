################################################################################
# Custom cmake module for CEGUI to find PVRTools lib (part of PowerVR SDK) 
################################################################################
include(FindPackageHandleStandardArgs)

find_path(PVRTOOLS_H_PATH NAMES PVRTDecompress.h)
find_library(PVRTOOLS_LIB NAMES oglestools)
mark_as_advanced(PVRTOOLS_H_PATH PVRTOOLS_LIB PVRTOOLS_LIB_DBG)

find_package_handle_standard_args(PVRTOOLS DEFAULT_MSG PVRTOOLS_LIB PVRTOOLS_H_PATH)

# set up output vars
if (PVRTOOLS_FOUND)
    set (PVRTOOLS_INCLUDE_DIR ${PVRTOOLS_H_PATH})
    set (PVRTOOLS_LIBRARIES ${PVRTOOLS_LIB})
    if (PVRTOOLS_LIB_DBG)
        set (PVRTOOLS_LIBRARIES_DBG ${PVRTOOLS_LIB_DBG})
    endif()
else()
    set (PVRTOOLS_INCLUDE_DIR)
    set (PVRTOOLS_LIBRARIES)
    set (PVRTOOLS_LIBRARIES_DBG)
endif()

