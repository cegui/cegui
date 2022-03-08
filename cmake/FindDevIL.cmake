################################################################################
# Custom cmake module for CEGUI to find DevIL
################################################################################
include(FindPackageHandleStandardArgs)

find_path(IL_H_PATH NAMES IL/il.h il.h)
find_library(IL_LIB NAMES IL DevIL ILU PATH_SUFFIXES dynamic/${CEGUI_ARCH_SUFFIX})
find_library(IL_LIB_DBG NAMES IL_d DevIL_d ILU_d PATH_SUFFIXES dynamic/${CEGUI_ARCH_SUFFIX})
mark_as_advanced(IL_H_PATH IL_LIB IL_LIB_DBG)

if (WIN32 OR APPLE)
    find_library(IL_LIB_STATIC NAMES IL DevIL ILU PATH_SUFFIXES static/${CEGUI_ARCH_SUFFIX})
    find_library(IL_LIB_STATIC_DBG NAMES IL_d DevIL_d ILU_d PATH_SUFFIXES static/${CEGUI_ARCH_SUFFIX})

    cegui_find_libraries(IL_LIB_STATIC "libpng;libtiff;jpeg;libmng" static/${CEGUI_ARCH_SUFFIX})
    cegui_find_libraries(IL_LIB_STATIC_DBG "libpng_d;libtiff_d;jpeg_d;libmng_d" static/${CEGUI_ARCH_SUFFIX})

    set( IL_DEFINITIONS_STATIC "IL_STATIC_LIB" CACHE STRING "preprocessor definitions" )
    mark_as_advanced( IL_DEFINITIONS_STATIC IL_LIB_STATIC IL_LIB_STATIC_DBG)
endif()

cegui_find_package_handle_standard_args(DevIL IL_LIB IL_H_PATH)

# set up output vars
if (DEVIL_FOUND)
    set (IL_INCLUDE_DIR ${IL_H_PATH})
    set (IL_LIBRARIES ${IL_LIB})
    if (IL_LIB_DBG)
        set (IL_LIBRARIES_DBG ${IL_LIB_DBG})
    endif()
    if (IL_LIB_STATIC)
        set (IL_LIBRARIES_STATIC ${IL_LIB_STATIC})
    endif()
    if (IL_LIB_STATIC_DBG)
        set (IL_LIBRARIES_STATIC_DBG ${IL_LIB_STATIC_DBG})
    endif()
else()
    set (IL_INCLUDE_DIR)
    set (IL_LIBRARIES)
    set (IL_LIBRARIES_DBG)
    set (IL_LIBRARIES_STATIC)
    set (IL_LIBRARIES_STATIC_DBG)
endif()

