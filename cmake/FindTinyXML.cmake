################################################################################
# Custom cmake module for CEGUI to find tinyxml
################################################################################
include(FindPackageHandleStandardArgs)

find_path(TINYXML_H_PATH NAMES tinyxml.h)
find_library(TINYXML_LIB NAMES tinyxml libtinyxml)
find_library(TINYXML_LIB_DBG NAMES tinyxml_d libtinyxml_d)
mark_as_advanced(TINYXML_H_PATH TINYXML_LIB TINYXML_LIB_DBG)

find_package_handle_standard_args(TINYXML DEFAULT_MSG TINYXML_LIB TINYXML_H_PATH)


# set up output vars
if (TINYXML_FOUND)
    set (TINYXML_INCLUDE_DIR ${TINYXML_H_PATH})
    set (TINYXML_LIBRARIES ${TINYXML_LIB})
    if (TINYXML_LIB_DBG)
        set (TINYXML_LIBRARIES_DBG ${TINYXML_LIB_DBG})
    endif()
else()
    set (TINYXML_INCLUDE_DIR)
    set (TINYXML_LIBRARIES)
    set (TINYXML_LIBRARIES_DBG)
endif()

