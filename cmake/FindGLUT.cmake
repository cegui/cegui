################################################################################
# Custom cmake module for CEGUI to find glut / freeglut
################################################################################
include(FindPackageHandleStandardArgs)

find_path(GLUT_H_PATH NAMES GL/glut.h glut.h)
find_library(GLUT_LIB NAMES freeglut glut)
find_library(GLUT_LIB_DBG NAMES freeglut_d glut_d)

find_package_handle_standard_args(GLUT DEFAULT_MSG GLUT_LIB GLUT_H_PATH)

mark_as_advanced(GLUT_H_PATH GLUT_LIB GLUT_LIB_DBG)

# set up output vars
if (GLUT_FOUND)
    set (GLUT_INCLUDE_DIR ${GLUT_H_PATH})
    set (GLUT_LIBRARIES ${GLUT_LIB})
    if (GLUT_LIB_DBG)
        set (GLUT_LIBRARIES_DBG ${GLUT_LIB_DBG})
    endif()
else()
    set (GLUT_INCLUDE_DIR)
    set (GLUT_LIBRARIES)
    set (GLUT_LIBRARIES_DBG)
endif()

