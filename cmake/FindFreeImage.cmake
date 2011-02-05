################################################################################
# Custom cmake module for CEGUI to find FreeImage 
################################################################################
include(FindPackageHandleStandardArgs)

find_path(FREEIMAGE_H_PATH NAMES FreeImage.h)
find_library(FREEIMAGE_LIB NAMES freeimage libfreeimage)
find_library(FREEIMAGE_LIB_DBG NAMES freeimaged libfreeimaged)

find_package_handle_standard_args(FREEIMAGE DEFAULT_MSG FREEIMAGE_LIB FREEIMAGE_H_PATH)

mark_as_advanced(FREEIMAGE_H_PATH FREEIMAGE_LIB FREEIMAGE_LIB_DBG)

# set up output vars
if (FREEIMAGE_FOUND)
    set (FREEIMAGE_INCLUDE_DIR ${FREEIMAGE_H_PATH})
    set (FREEIMAGE_LIBRARIES ${FREEIMAGE_LIB})
    if (FREEIMAGE_LIB_DBG)
        set (FREEIMAGE_LIBRARIES_DBG ${FREEIMAGE_LIB_DBG})
    endif()
else()
    set (FREEIMAGE_INCLUDE_DIR)
    set (FREEIMAGE_LIBRARIES)
    set (FREEIMAGE_LIBRARIES_DBG)
endif()

