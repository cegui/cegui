################################################################################
# Custom cmake module for CEGUI to find zzip 
################################################################################
include(FindPackageHandleStandardArgs)

find_path(ZZIP_H_PATH NAMES zzip.h)
find_library(ZZIP_LIB NAMES zzip libzzip PATH_SUFFIXES dynamic)
find_library(ZZIP_LIB_DBG NAMES zzipd libzzipd zzip_d libzzip_d PATH_SUFFIXES dynamic)
mark_as_advanced(ZZIP_H_PATH ZZIP_LIB ZZIP_LIB_DBG)

if (ZZIP_H_PATH AND ZZIP_LIB)
  set(ZZIP_FOUND TRUE)
endif()

# set up output vars
if (ZZIP_FOUND)
    set (ZZIP_INCLUDE_DIR ${ZZIP_H_PATH})
    set (ZZIP_LIBRARIES ${ZZIP_LIB})
    if (ZZIP_LIB_DBG)
        set (ZZIP_LIBRARIES_DBG ${ZZIP_LIB_DBG})
    endif()
else()
    set (ZZIP_INCLUDE_DIR)
    set (ZZIP_LIBRARIES)
    set (ZZIP_LIBRARIES_DBG)
    set (ZZIP_LIBRARIES_STATIC)
    set (ZZIP_LIBRARIES_STATIC_DBG)
endif()

