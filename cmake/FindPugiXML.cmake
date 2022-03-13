################################################################################
# Custom cmake module for CEGUI to find the PugiXML header file
################################################################################
include(FindPackageHandleStandardArgs)

find_path(PUGIXML_H_PATH NAMES pugixml.hpp)
find_library(PUGIXML_LIB NAMES pugixml PATH_SUFFIXES dynamic/${CEGUI_ARCH_SUFFIX})
find_library(PUGIXML_LIB_DBG NAMES pugixml_d PATH_SUFFIXES dynamic/${CEGUI_ARCH_SUFFIX})
mark_as_advanced(PUGIXML_H_PATH PUGIXML_LIB PUGIXML_LIB_DBG)

if (WIN32 OR APPLE)
    find_library(PUGIXML_LIB_STATIC NAMES pugixml PATH_SUFFIXES static/${CEGUI_ARCH_SUFFIX})
    find_library(PUGIXML_LIB_STATIC_DBG NAMES pugixml_d PATH_SUFFIXES static/${CEGUI_ARCH_SUFFIX})
    mark_as_advanced(PUGIXML_LIB_STATIC PUGIXML_LIB_STATIC_DBG)
endif()

find_package_handle_standard_args(PugiXML DEFAULT_MSG PUGIXML_H_PATH)

# set up output vars
if (PUGIXML_FOUND)
    set (PUGIXML_INCLUDE_DIR ${PUGIXML_H_PATH})
    set (PUGIXML_LIBRARIES ${PUGIXML_LIB})
    if (PUGIXML_LIB_DBG)
        set (PUGIXML_LIBRARIES_DBG ${PUGIXML_LIB_DBG})
    endif()
    if (PUGIXML_LIB_STATIC)
        set (PUGIXML_LIBRARIES_STATIC ${PUGIXML_LIB_STATIC})
    endif()
    if (PUGIXML_LIB_STATIC_DBG)
        set (PUGIXML_LIBRARIES_STATIC_DBG ${PUGIXML_LIB_STATIC_DBG})
    endif()
else()
    set (PUGIXML_INCLUDE_DIR)
    set (PUGIXML_LIBRARIES)
    set (PUGIXML_LIBRARIES_DBG)
    set (PUGIXML_LIBRARIES_STATIC)
    set (PUGIXML_LIBRARIES_STATIC_DBG)
endif()
