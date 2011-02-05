################################################################################
# Custom cmake module for CEGUI to find zlib
################################################################################
include(FindPackageHandleStandardArgs)

find_path(ZLIB_H_PATH NAMES zlib.h)
find_library(ZLIB_LIB NAMES z zlib zdll)
find_library(ZLIB_LIB_DBG NAMES z_d zlib_d zdll_d)

find_package_handle_standard_args(ZLIB DEFAULT_MSG ZLIB_LIB ZLIB_H_PATH)

mark_as_advanced(ZLIB_H_PATH ZLIB_LIB ZLIB_LIB_DBG)

# set up output vars
if (ZLIB_FOUND)
    set (ZLIB_INCLUDE_DIR ${ZLIB_H_PATH})
    set (ZLIB_LIBRARIES ${ZLIB_LIB})
    if (ZLIB_LIB_DBG)
        set (ZLIB_LIBRARIES_DBG ${ZLIB_LIB_DBG})
    endif()
else()
    set (ZLIB_INCLUDE_DIR)
    set (ZLIB_LIBRARIES)
    set (ZLIB_LIBRARIES_DBG)
endif()

