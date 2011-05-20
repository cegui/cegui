################################################################################
# Custom cmake module for CEGUI to find the RapidXML header file
################################################################################
include(FindPackageHandleStandardArgs)

find_path(RAPIDXML_H_PATH NAMES rapidxml.hpp PATH_SUFFIXES rapidxml)
mark_as_advanced(RAPIDXML_H_PATH)

find_package_handle_standard_args(RAPIDXML DEFAULT_MSG RAPIDXML_H_PATH)


# set up output vars
if (RAPIDXML_FOUND)
    set (RAPIDXML_INCLUDE_DIR ${RAPIDXML_H_PATH})
else()
    set (RAPIDXML_INCLUDE_DIR)
endif()

