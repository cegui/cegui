################################################################################
# Custom cmake module for CEGUI to find OIS
################################################################################
include(FindPackageHandleStandardArgs)

find_path(OIS_H_PATH NAMES OIS.h PATH_SUFFIXES OIS)
find_library(OIS_LIB NAMES OIS libOIS)
find_library(OIS_LIB_DBG NAMES OIS_d libOIS_d)

find_package_handle_standard_args(OIS DEFAULT_MSG OIS_LIB OIS_H_PATH)

mark_as_advanced(OIS_H_PATH OIS_LIB OIS_LIB_DBG)

# set up output vars
if (OIS_FOUND)
    set (OIS_INCLUDE_DIR ${OIS_H_PATH})
    set (OIS_LIBRARIES ${OIS_LIB})
    if (OIS_LIB_DBG)
        set (OIS_LIBRARIES_DBG ${OIS_LIB_DBG})
    endif()
else()
    set (OIS_INCLUDE_DIR)
    set (OIS_LIBRARIES)
    set (OIS_LIBRARIES_DBG)
endif()

