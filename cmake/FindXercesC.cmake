################################################################################
# Custom cmake module for CEGUI to find the Xerces-C++ XML parser
################################################################################
include(FindPackageHandleStandardArgs)

find_path(XERCESC_H_PATH NAMES xercesc/util/XercesVersion.hpp)
find_library(XERCESC_LIB NAMES xerces-c libxerces-c xerces-c_3 xerces-c_2 Xerces)
find_library(XERCESC_LIB_DBG NAMES xerces-c_3D xerces-c_2D)

find_package_handle_standard_args(XERCESC DEFAULT_MSG XERCESC_LIB XERCESC_H_PATH)

mark_as_advanced(XERCESC_H_PATH XERCESC_LIB XERCESC_LIB_DBG)

# set up output vars
if (XERCESC_FOUND)
    set (XERCESC_INCLUDE_DIR ${XERCESC_H_PATH})
    set (XERCESC_LIBRARIES ${XERCESC_LIB})
    if (XERCESC_LIB_DBG)
        set (XERCESC_LIBRARIES_DBG ${XERCESC_LIB_DBG})
    endif()
else()
    set (XERCESC_INCLUDE_DIR)
    set (XERCESC_LIBRARIES)
    set (XERCESC_LIBRARIES_DBG)
endif()

