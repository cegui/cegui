################################################################################
# Custom cmake module for CEGUI to find corona
################################################################################
include(FindPackageHandleStandardArgs)

find_path(CORONA_H_PATH NAMES corona.h)
find_library(CORONA_LIB NAMES corona libcorona)
find_library(CORONA_LIB_DBG NAMES corona_d libcorona_d)

find_package_handle_standard_args(CORONA DEFAULT_MSG CORONA_LIB CORONA_H_PATH)

mark_as_advanced(CORONA_H_PATH CORONA_LIB CORONA_LIB_DBG)

# set up output vars
if (CORONA_FOUND)
    set (CORONA_INCLUDE_DIR ${CORONA_H_PATH})
    set (CORONA_LIBRARIES ${CORONA_LIB})
    if (CORONA_LIB_DBG)
        set (CORONA_LIBRARIES_DBG ${CORONA_LIB_DBG})
    endif()
else()
    set (CORONA_INCLUDE_DIR)
    set (CORONA_LIBRARIES)
    set (CORONA_LIBRARIES_DBG)
endif()

