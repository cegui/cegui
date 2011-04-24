################################################################################
# Custom cmake module for CEGUI to find SILLY
################################################################################
include(FindPackageHandleStandardArgs)

find_path(SILLY_H_PATH NAMES SILLY.h PATH_SUFFIXES SILLY)
find_library(SILLY_LIB NAMES SILLY libSILLY)
find_library(SILLY_LIB_DBG NAMES SILLY_d libSILLY_d)

find_package_handle_standard_args(SILLY DEFAULT_MSG SILLY_LIB SILLY_H_PATH)

mark_as_advanced(SILLY_H_PATH SILLY_LIB SILLY_LIB_DBG)

# set up output vars
if (SILLY_FOUND)
    set (SILLY_INCLUDE_DIR ${SILLY_H_PATH})
    set (SILLY_LIBRARIES ${SILLY_LIB})
    if (SILLY_LIB_DBG)
        set (SILLY_LIBRARIES_DBG ${SILLY_LIB_DBG})
    endif()
else()
    set (SILLY_INCLUDE_DIR)
    set (SILLY_LIBRARIES)
    set (SILLY_LIBRARIES_DBG)
endif()

