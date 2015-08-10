################################################################################
# Custom cmake module for CEGUI to find the Xerces-C++ XML parser
################################################################################
include(FindPackageHandleStandardArgs)

find_path(XERCESC_H_PATH NAMES xercesc/util/XercesVersion.hpp)
find_library(XERCESC_LIB NAMES xerces-c libxerces-c xerces-c_3 xerces-c_2 Xerces PATH_SUFFIXES dynamic)
find_library(XERCESC_LIB_DBG NAMES xerces-c_3_d xerces-c_3D xerces-c_2_d xerces-c_2D PATH_SUFFIXES dynamic)
mark_as_advanced(XERCESC_H_PATH XERCESC_LIB XERCESC_LIB_DBG)

if (WIN32 OR APPLE)
    find_library(XERCESC_LIB_STATIC NAMES xerces-c_3 xerces-c libxerces-c xerces-c_static_3 xerces-c_static_2 Xerces PATH_SUFFIXES static)
    find_library(XERCESC_LIB_STATIC_DBG NAMES xerces-c_3_d xerces-c_static_3D xerces-c_static_2D PATH_SUFFIXES static)
    set( XERCESC_DEFINITIONS_STATIC "XERCES_STATIC_LIBRARY" CACHE STRING "preprocessor definitions" )
    mark_as_advanced(XERCESC_DEFINITIONS_STATIC XERCESC_LIB_STATIC XERCESC_LIB_STATIC_DBG)
endif()

cegui_find_package_handle_standard_args(XERCESC XERCESC_LIB XERCESC_H_PATH)


# set up output vars
if (XERCESC_FOUND)
    set (XERCESC_INCLUDE_DIR ${XERCESC_H_PATH})
    set (XERCESC_LIBRARIES ${XERCESC_LIB})
    if (XERCESC_LIB_DBG)
        set (XERCESC_LIBRARIES_DBG ${XERCESC_LIB_DBG})
    endif()
    if (XERCESC_LIB_STATIC)
        set (XERCESC_LIBRARIES_STATIC ${XERCESC_LIB_STATIC})
    endif()
    if (XERCESC_LIB_STATIC_DBG)
        set (XERCESC_LIBRARIES_STATIC_DBG ${XERCESC_LIB_STATIC_DBG})
    endif()
else()
    set (XERCESC_INCLUDE_DIR)
    set (XERCESC_LIBRARIES)
    set (XERCESC_LIBRARIES_DBG)
    set (XERCESC_LIBRARIES_STATIC)
    set (XERCESC_LIBRARIES_STATIC_DBG)
endif()

