################################################################################
# Custom cmake module for CEGUI to find OpenGL ES (1.1) 
#
# Placeholder module to create some vars we can manually set.
# Later will create a proper tests (or steal them, anyway)
################################################################################
include(FindPackageHandleStandardArgs)

find_path(OPENGLES_H_PATH NAMES GLES/gl.h)
find_library(OPENGLES_LIB NAMES GLES_CM)
mark_as_advanced(OPENGLES_H_PATH OPENGLES_LIB OPENGLES_LIB_DBG)

find_package_handle_standard_args(OPENGLES DEFAULT_MSG OPENGLES_LIB OPENGLES_H_PATH)

# set up output vars
if (OPENGLES_FOUND)
    set (OPENGLES_INCLUDE_DIR ${OPENGLES_H_PATH})
    set (OPENGLES_LIBRARIES ${OPENGLES_LIB})
    if (OPENGLES_LIB_DBG)
        set (OPENGLES_LIBRARIES_DBG ${OPENGLES_LIB_DBG})
    endif()
else()
    set (OPENGLES_INCLUDE_DIR)
    set (OPENGLES_LIBRARIES)
    set (OPENGLES_LIBRARIES_DBG)
endif()

