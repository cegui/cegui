################################################################################
# Custom cmake module for CEGUI to find OpenGL ES (2.0) 
#
# Placeholder module to create some vars we can manually set.
# Later will create a proper tests (or steal them, anyway)
################################################################################
include(FindPackageHandleStandardArgs)

find_path(OPENGLES2_H_PATH NAMES GLES2/gl2.h)
find_library(OPENGLES2_LIB NAMES GLESv2)
mark_as_advanced(OPENGLES2_H_PATH OPENGLES2_LIB OPENGLES2_LIB_DBG)

find_package_handle_standard_args(OPENGLES2 DEFAULT_MSG OPENGLES2_LIB OPENGLES2_H_PATH)

# set up output vars
if (OPENGLES2_FOUND)
    set (OPENGLES2_INCLUDE_DIR ${OPENGLES2_H_PATH})
    set (OPENGLES2_LIBRARIES ${OPENGLES2_LIB})
    if (OPENGLES2_LIB_DBG)
        set (OPENGLES2_LIBRARIES_DBG ${OPENGLES2_LIB_DBG})
    endif()
else()
    set (OPENGLES2_INCLUDE_DIR)
    set (OPENGLES2_LIBRARIES)
    set (OPENGLES2_LIBRARIES_DBG)
endif()

