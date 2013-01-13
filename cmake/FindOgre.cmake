################################################################################
# Custom cmake module for CEGUI to find OGRE
################################################################################
include(FindPackageHandleStandardArgs)

find_path(OGRE_H_PATH NAMES Ogre.h PATH_SUFFIXES OGRE)
find_library(OGRE_LIB NAMES OgreMain)
find_library(OGRE_LIB_DBG NAMES OgreMain_d)
mark_as_advanced(OGRE_H_PATH OGRE_LIB OGRE_LIB_DBG)

find_package_handle_standard_args(OGRE DEFAULT_MSG OGRE_LIB OGRE_H_PATH)


# set up output vars
if (OGRE_FOUND)
    set (OGRE_INCLUDE_DIR ${OGRE_H_PATH})
    set (OGRE_LIBRARIES ${OGRE_LIB})
    if (OGRE_LIB_DBG)
        set (OGRE_LIBRARIES_DBG ${OGRE_LIB_DBG})
    endif()
else()
    set (OGRE_INCLUDE_DIR)
    set (OGRE_LIBRARIES)
    set (OGRE_LIBRARIES_DBG)
endif()

