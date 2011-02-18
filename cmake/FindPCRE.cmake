################################################################################
# Custom cmake module for CEGUI to find PCRE
################################################################################
include(FindPackageHandleStandardArgs)

find_path(PCRE_H_PATH NAMES pcre.h)
find_library(PCRE_LIB NAMES PCRE pcre libpcre)
find_library(PCRE_LIB_DBG NAMES pcre_d libpcre_d)

find_package_handle_standard_args(PCRE DEFAULT_MSG PCRE_LIB PCRE_H_PATH)

mark_as_advanced(PCRE_H_PATH PCRE_LIB PCRE_LIB_DBG)

# set up output vars
if (PCRE_FOUND)
    set (PCRE_INCLUDE_DIR ${PCRE_H_PATH})
    set (PCRE_LIBRARIES ${PCRE_LIB})
    if (PCRE_LIB_DBG)
        set (PCRE_LIBRARIES_DBG ${PCRE_LIB_DBG})
    endif()
else()
    set (PCRE_INCLUDE_DIR)
    set (PCRE_LIBRARIES)
    set (PCRE_LIBRARIES_DBG)
endif()

