################################################################################
# Custom cmake module for CEGUI to find DevIL
################################################################################
include(FindPackageHandleStandardArgs)

find_path(IL_H_PATH NAMES IL/il.h il.h)
find_library(IL_LIB NAMES IL DevIL)
find_library(IL_LIB_DBG NAMES IL_d DevIL_d)
find_library(ILU_LIB NAMES ILU)
find_library(ILU_LIB_DBG NAMES ILU_d)

find_package_handle_standard_args(IL DEFAULT_MSG IL_LIB ILU_LIB IL_H_PATH)

mark_as_advanced(IL_H_PATH IL_LIB IL_LIB_DBG ILU_LIB ILU_LIB_DBG)

# set up output vars
if (IL_FOUND)
    set (IL_INCLUDE_DIR ${IL_H_PATH})
    set (IL_LIBRARIES ${ILU_LIB} ${IL_LIB})
    if (IL_LIB_DBG AND ILU_LIB_DBG)
        set (IL_LIBRARIES_DBG ${ILU_LIB_DBG} ${IL_LIB_DBG})
    endif()
else()
    set (IL_INCLUDE_DIR)
    set (IL_LIBRARIES)
    set (IL_LIBRARIES_DBG)
endif()

