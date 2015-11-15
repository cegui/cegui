################################################################################
# Custom cmake module for CEGUI to find Epoxy.
################################################################################
include(FindPackageHandleStandardArgs)

find_path(EPOXY_H_PATH NAMES epoxy/gl.h)
find_library(EPOXY_LIB NAMES epoxy epoxy_0)
mark_as_advanced(EPOXY_H_PATH EPOXY_LIB EPOXY_LIB_DBG)

find_package_handle_standard_args(EPOXY DEFAULT_MSG EPOXY_LIB EPOXY_H_PATH)

# set up output vars
if (EPOXY_FOUND)
    set (EPOXY_INCLUDE_DIR ${EPOXY_H_PATH})
    set (EPOXY_LIBRARIES ${EPOXY_LIB})
    if (EPOXY_LIB_DBG)
        set (EPOXY_LIBRARIES_DBG ${EPOXY_LIB_DBG})
    endif()
else()
    set (EPOXY_INCLUDE_DIR)
    set (EPOXY_LIBRARIES)
    set (EPOXY_LIBRARIES_DBG)
endif()
