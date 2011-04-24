################################################################################
# Custom cmake module for CEGUI to find the Expat XML parser
################################################################################
include(FindPackageHandleStandardArgs)

find_path(EXPAT_H_PATH NAMES expat.h)
find_library(EXPAT_LIB NAMES expat libexpat)
find_library(EXPAT_LIB_DBG NAMES expat_d libexpat_d)

find_package_handle_standard_args(EXPAT DEFAULT_MSG EXPAT_LIB EXPAT_H_PATH)

mark_as_advanced(EXPAT_H_PATH EXPAT_LIB EXPAT_LIB_DBG)

# set up output vars
if (EXPAT_FOUND)
    set (EXPAT_INCLUDE_DIR ${EXPAT_H_PATH})
    set (EXPAT_LIBRARIES ${EXPAT_LIB})
    if (EXPAT_LIB_DBG)
        set (EXPAT_LIBRARIES_DBG ${EXPAT_LIB_DBG})
    endif()
else()
    set (EXPAT_INCLUDE_DIR)
    set (EXPAT_LIBRARIES)
    set (EXPAT_LIBRARIES_DBG)
endif()

