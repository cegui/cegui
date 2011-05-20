################################################################################
# Custom cmake module for CEGUI to find the irrlicht engine
################################################################################
include(FindPackageHandleStandardArgs)

find_path(IRRLICHT_H_PATH NAMES irrlicht.h PATH_SUFFIXES irrlicht)
find_library(IRRLICHT_LIB NAMES Irrlicht libIrrlicht)
find_library(IRRLICHT_LIB_DBG NAMES Irrlicht_d libIrrlicht_d)
mark_as_advanced(IRRLICHT_H_PATH IRRLICHT_LIB IRRLICHT_LIB_DBG)

find_package_handle_standard_args(IRRLICHT DEFAULT_MSG IRRLICHT_LIB IRRLICHT_H_PATH)


# set up output vars
if (IRRLICHT_FOUND)
    set (IRRLICHT_INCLUDE_DIR ${IRRLICHT_H_PATH})
    set (IRRLICHT_LIBRARIES ${IRRLICHT_LIB})
    if (IRRLICHT_LIB_DBG)
        set (IRRLICHT_LIBRARIES_DBG ${IRRLICHT_LIB_DBG})
    endif()
else()
    set (IRRLICHT_INCLUDE_DIR)
    set (IRRLICHT_LIBRARIES)
    set (IRRLICHT_LIBRARIES_DBG)
endif()

