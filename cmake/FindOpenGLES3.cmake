################################################################################
# Custom cmake module for CEGUI to find OpenGL ES (3.0) 
#
# Placeholder module to create some vars we can manually set.
# Later will create a proper tests (or steal them, anyway)
################################################################################
include(FindPackageHandleStandardArgs)

find_path(OPENGLES3_H_PATH NAMES GLES3/gl3.h)
find_library(OPENGLES3_LIB NAMES GLESv3)
mark_as_advanced(OPENGLES3_H_PATH OPENGLES3_LIB OPENGLES3_LIB_DBG)

find_package_handle_standard_args(OPENGLES3 DEFAULT_MSG OPENGLES3_LIB OPENGLES3_H_PATH)

# set up output vars
if (OPENGLES3_FOUND)
    set (OPENGLES3_INCLUDE_DIR ${OPENGLES3_H_PATH})
    set (OPENGLES3_LIBRARIES ${OPENGLES3_LIB})
    if (OPENGLES3_LIB_DBG)
        set (OPENGLES3_LIBRARIES_DBG ${OPENGLES3_LIB_DBG})
    endif()
else()
    set (OPENGLES3_INCLUDE_DIR)
    set (OPENGLES3_LIBRARIES)
    set (OPENGLES3_LIBRARIES_DBG)
endif()

