################################################################################
# Custom cmake module for CEGUI to find OIS
################################################################################
include(FindPackageHandleStandardArgs)

set(ENV_OIS_HOME $ENV{OIS_HOME})
if (ENV_OIS_HOME)
  string( REGEX REPLACE "\\\\" "/" ENV_OIS_HOME ${ENV_OIS_HOME} )
endif ()

set(OIS_PREFIX_PATH
  ${OIS_HOME} ${ENV_OIS_HOME}
)


find_path(OIS_H_PATH NAMES OIS.h
          PATHS ${OIS_PREFIX_PATH} 
          PATH_SUFFIXES OIS include includes)
        
if(NOT OIS_H_PATH)
    find_path(OIS_H_PATH NAMES OIS.h
              PATHS ${OIS_PREFIX_PATH} 
              PATH_SUFFIXES OIS include include/OIS)
endif()
        
find_library(OIS_LIB NAMES OIS libOIS
             PATHS ${OIS_PREFIX_PATH}
             PATH_SUFFIXES Release lib/Release)
             
find_library(OIS_LIB_DBG NAMES OIS_d libOIS_d
             PATHS ${OIS_PREFIX_PATH}
             PATH_SUFFIXES Debug lib/Debug)
             
mark_as_advanced(OIS_H_PATH OIS_LIB OIS_LIB_DBG)

cegui_find_package_handle_standard_args(OIS OIS_LIB OIS_H_PATH)

# set up output vars
if (OIS_FOUND)
    set (OIS_INCLUDE_DIR ${OIS_H_PATH})
    set (OIS_LIBRARIES ${OIS_LIB})
    if (OIS_LIB_DBG)
        set (OIS_LIBRARIES_DBG ${OIS_LIB_DBG})
    endif()
else()
    set (OIS_INCLUDE_DIR)
    set (OIS_LIBRARIES)
    set (OIS_LIBRARIES_DBG)
endif()

