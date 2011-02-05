################################################################################
# Custom cmake module for CEGUI to find freetype
################################################################################
include(FindPackageHandleStandardArgs)

find_path(FREETYPE_H_PATH_ft2build NAMES ft2build.h)
find_path(FREETYPE_H_PATH_ftconfig NAMES freetype/config/ftconfig.h PATH_SUFFIXES freetype2)
find_library(FREETYPE_LIB NAMES freetype2 freetype libfreetype)
find_library(FREETYPE_LIB_DBG NAMES freetype_d libfreetype_d)

find_package_handle_standard_args(FREETYPE DEFAULT_MSG FREETYPE_LIB FREETYPE_H_PATH_ft2build FREETYPE_H_PATH_ftconfig)

mark_as_advanced(FREETYPE_H_PATH_ft2build FREETYPE_H_PATH_ftconfig FREETYPE_LIB FREETYPE_LIB_DBG)

# set up output vars
if (FREETYPE_FOUND)
    set (FREETYPE_INCLUDE_DIR ${FREETYPE_H_PATH_ft2build} ${FREETYPE_H_PATH_ftconfig})
    set (FREETYPE_LIBRARIES ${FREETYPE_LIB})
    if (FREETYPE_LIB_DBG)
        set (FREETYPE_LIBRARIES_DBG ${FREETYPE_LIB_DBG})
    endif()
else()
    set (FREETYPE_INCLUDE_DIR)
    set (FREETYPE_LIBRARIES)
    set (FREETYPE_LIBRARIES_DBG)
endif()

