# Try to find EGL library and include dir.
# Once done this will define
#
# EGL_FOUND        - true if EGL has been found
# EGL_INCLUDE_DIR  - where the EGL/egl.h and KHR/khrplatform.h can be found
# EGL_LIBRARIES    - link this to use libEGL.so.1

include(FindPackageHandleStandardArgs)

find_path(EGL_H_PATH NAMES EGL/egl.h)
find_library(EGL_LIB NAMES EGL)
mark_as_advanced(EGL_H_PATH EGL_LIB)

cegui_find_package_handle_standard_args(EGL EGL_LIB EGL_H_PATH)

if (EGL_FOUND)
    set (EGL_INCLUDE_DIR ${EGL_H_PATH})
    set (EGL_LIBRARIES ${EGL_LIB})
else()
    set (EGL_INCLUDE_DIR)
    set (EGL_LIBRARIES)
endif()

