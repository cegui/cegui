################################################################################
# Custom cmake module for CEGUI to find GLEW
################################################################################
include(FindPackageHandleStandardArgs)

find_path(GLEW_H_PATH NAMES GL/glew.h)
find_library(GLEW_LIB NAMES GLEW libGLEW)
find_library(GLEW_LIB_DBG NAMES GLEW_d libGLEW_d)

find_package_handle_standard_args(GLEW DEFAULT_MSG GLEW_LIB GLEW_H_PATH)

mark_as_advanced(GLEW_H_PATH GLEW_LIB GLEW_LIB_DBG)

# set up output vars
if (GLEW_FOUND)
    set (GLEW_INCLUDE_DIR ${GLEW_H_PATH})
    set (GLEW_LIBRARIES ${GLEW_LIB})
    if (GLEW_LIB_DBG)
        set (GLEW_LIBRARIES_DBG ${GLEW_LIB_DBG})
    endif()
else()
    set (GLEW_INCLUDE_DIR)
    set (GLEW_LIBRARIES)
    set (GLEW_LIBRARIES_DBG)
endif()

