################################################################################
# Custom cmake module for CEGUI to find OGRE
################################################################################
include(FindPackageHandleStandardArgs)

# ----------------------------------------------------------------------------------------------------------------------
# Determine the OGRE Thread provider configured in the header file.
# Parameters:
#   - _tp_var: Output parameter. The variable in which to store the OGRE_THREAD_PROVIDER read from the OGRE config.
#   - _file: The name and path of the file to search into. This should typically be the "OgreBuildSettings.h" file.
#
# At the time of the writing of this function, the following possible values are defined in OGRE and can be stored in 
#_tp_var:
#   1 = boost
#   2 = poco
#   3 = tbb
#
# Function Author: Augustin Preda.
#     Based on the "DetermineVersion.cmake" file by  Reto Grieder.
#
# NB: Originally taken from Sumwars (http://sumwars.org), the function's code is public domain.
# ----------------------------------------------------------------------------------------------------------------------
function(determine_ogre_thread_provider _tp_var _file)
  if(EXISTS ${_file})
    file(READ ${_file} _file_content)
    set(_parts ${ARGN})
    list(LENGTH _parts _parts_length)
    if(NOT ${_parts_length} EQUAL 3)
      set(_parts MAJOR MINOR PATCH)
    endif()

    foreach(_part ${_parts})
      string(REGEX MATCH "#define OGRE_THREAD_PROVIDER +([0-9]+)" _match ${_file_content})
      if(_match)
        set(${_tp_var} ${CMAKE_MATCH_1})
        set(${_tp_var} ${CMAKE_MATCH_1} PARENT_SCOPE)
      else()
        set(${_tp_var} "0")
        set(${_tp_var} "0" PARENT_SCOPE)
      endif()
    endforeach(_part)
    set(${_tp_var}_VERSION "${${_tp_var}_VERSION}" PARENT_SCOPE)
  endif()
endfunction()

# Determine if OGRE is static configured in the header file.
#   - _tp_var: Output parameter. The variable in which to store the OGRE_STATIC_LIB read from the OGRE config.
#   - _file: The name and path of the file to search into. This should typically be the "OgreBuildSettings.h" file.
# similar functionality for consistentcy with existing "determine_ogre_thread_providier" function
function(determine_ogre_static _tp_var _file)
  if(EXISTS ${_file})
    file(READ ${_file} _file_content)
    string(REGEX MATCH "#define OGRE_STATIC_LIB" _match ${_file_content})
    if(_match)
      set(${_tp_var} TRUE PARENT_SCOPE)
    else()
      set(${_tp_var} FALSE PARENT_SCOPE)
    endif()
  endif()
endfunction()

# Determine if OGRE is configured with freeimage in the header file.
#   - _tp_var: Output parameter. The variable in which to store the OGRE_NO_FREEIMAGE read from the OGRE config.
#   - _file: The name and path of the file to search into. This should typically be the "OgreBuildSettings.h" file.
# similar functionality for consistentcy with existing "determine_ogre_thread_providier" function
# the regex matching is taken from Ogre's PreprocessorUtil.cmake 
function(determine_ogre_freeimage _tp_var _file)
    set(KEYWORD "OGRE_NO_FREEIMAGE")
    if(EXISTS ${_file})
        file(READ ${_file} _file_content)
        string(REGEX MATCH "# *define +${KEYWORD} +((\"([^\n]*)\")|([^ \n]*))" PREPROC_TEMP_VAR ${_file_content})
        if (CMAKE_MATCH_3)
            set(${_tp_var} ${CMAKE_MATCH_3} PARENT_SCOPE)
        else ()
            set(${_tp_var} ${CMAKE_MATCH_4} PARENT_SCOPE)
        endif ()
    endif()
endfunction()

set(ENV_OGRE_HOME $ENV{OGRE_HOME})
if (ENV_OGRE_HOME)
    string( REGEX REPLACE "\\\\" "/" ENV_OGRE_HOME ${ENV_OGRE_HOME} )
endif ()

set(ENV_OGRE_SDK $ENV{OGRE_SDK})
if (ENV_OGRE_SDK)
    string( REGEX REPLACE "\\\\" "/" ENV_OGRE_SDK ${ENV_OGRE_SDK} )
endif ()

set(OGRE_PREFIX_PATH
    ${OGRE_HOME} ${OGRE_SDK} ${ENV_OGRE_HOME} ${ENV_OGRE_SDK}
)

set(OGRE_PATH_SUFFIXES
    ${OGRE_PATH_SUFFIXES} include include/OGRE OGRE OgreMain/include OgreMain/include/OGRE
)

# Find the Ogre include headers.
find_path(OGRE_H_PATH
          NAMES Ogre.h
          PATHS ${OGRE_PREFIX_PATH}
          PATH_SUFFIXES ${OGRE_PATH_SUFFIXES})

# Also find the build settings Ogre header.
find_path(OGRE_H_BUILD_SETTINGS_PATH
          NAMES OgreBuildSettings.h
          PATHS ${OGRE_PREFIX_PATH}
          PATH_SUFFIXES ${OGRE_PATH_SUFFIXES})

if (OGRE_H_BUILD_SETTINGS_PATH)
    determine_ogre_static(OGRE_CONFIG_STATIC ${OGRE_H_BUILD_SETTINGS_PATH}/OgreBuildSettings.h)
    determine_ogre_freeimage(OGRE_CONFIG_FREEIMAGE ${OGRE_H_BUILD_SETTINGS_PATH}/OgreBuildSettings.h)
    set(OGRE_STATIC ${OGRE_CONFIG_STATIC})
endif ()

if (OGRE_STATIC)
    set(OGRE_LIB_SUFFIX "Static")
else ()
    set(OGRE_LIB_SUFFIX "")
endif ()
if(APPLE AND NOT OGRE_STATIC)
    set(OGRE_LIBRARY_NAMES "Ogre${OGRE_LIB_SUFFIX}")
else()
    set(OGRE_LIBRARY_NAMES "OgreMain${OGRE_LIB_SUFFIX}")
endif()

# Find the release and debug libraries.
find_library(OGRE_LIB NAMES ${OGRE_LIBRARY_NAMES}
             PATHS ${OGRE_PREFIX_PATH}
             PATH_SUFFIXES Release lib/Release)
             
find_library(OGRE_LIB_DBG NAMES ${OGRE_LIBRARY_NAMES}_d
             PATHS ${OGRE_PREFIX_PATH}
             PATH_SUFFIXES Debug lib/Debug)
             
mark_as_advanced(OGRE_H_PATH OGRE_H_BUILD_SETTINGS_PATH OGRE_LIB OGRE_LIB_DBG)

# If the Ogre build settings file was found, parse it to identify the Ogre Thread provider.
# Basically test to see whether ${OGRE_H_BUILD_SETTINGS_PATH} is not OGRE_H_BUILD_SETTINGS_PATH-NOTFOUND
if (OGRE_H_BUILD_SETTINGS_PATH)
    determine_ogre_thread_provider(OGRE_THREAD_PROVIDER ${OGRE_H_BUILD_SETTINGS_PATH}/OgreBuildSettings.h)
endif ()

find_package_handle_standard_args(OGRE DEFAULT_MSG OGRE_LIB OGRE_H_PATH OGRE_H_BUILD_SETTINGS_PATH)

# set up output vars
if (OGRE_FOUND)
    # Set-up the directory (actually: list of directories that should be added to the inclusion list).
    set (OGRE_INCLUDE_DIR ${OGRE_H_PATH};${OGRE_H_BUILD_SETTINGS_PATH})

    # Set-up the list of directories for the OGRE libraries to link against in the Ogre related components.
    set (OGRE_LIBRARIES ${OGRE_LIB})
    if (OGRE_LIB_DBG)
        set (OGRE_LIBRARIES_DBG ${OGRE_LIB_DBG})
    endif()

    # Ogre was found. If the thread provider is boost, also search for the boost libs.
    if ("${OGRE_THREAD_PROVIDER}" STREQUAL "1")
        if (Boost_VERSION LESS 105000) # boost thread transitionally requires boost system since Boost 1.50
            find_package(Boost COMPONENTS thread)

            set(Boost_SYSTEM_LIBRARY_RELEASE "")
            set(Boost_SYSTEM_LIBRARY_DEBUG "")
        else()
            find_package(Boost COMPONENTS thread system)
        endif()

        find_package_handle_standard_args(OGRE DEFAULT_MSG OGRE_LIB OGRE_H_PATH OGRE_H_BUILD_SETTINGS_PATH)

        # Add the Boost directories
        set(OGRE_INCLUDE_DIR ${OGRE_INCLUDE_DIR};${Boost_INCLUDE_DIR})

        # Add the Boost libraries.
        if (OGRE_LIBRARIES)
          set (OGRE_LIBRARIES ${OGRE_LIBRARIES};${Boost_THREAD_LIBRARY_RELEASE};${Boost_SYSTEM_LIBRARY_RELEASE})
        endif()

        if (OGRE_LIBRARIES_DBG)
          set (OGRE_LIBRARIES_DBG ${OGRE_LIBRARIES_DBG};${Boost_THREAD_LIBRARY_DEBUG};${Boost_SYSTEM_LIBRARY_DEBUG})
        endif()
    endif()

    # look for required Ogre dependencies in case of static build
    if (OGRE_STATIC)
        set(OGRE_DEPS_FOUND TRUE)
        find_package(FreeImage QUIET)
        find_package(Freetype QUIET)
        find_package(ZLIB QUIET)
        find_package(ZZip QUIET)
        if (UNIX AND NOT APPLE)
            find_package(X11 QUIET)
            find_library(XAW_LIBRARY NAMES Xaw Xaw7 PATHS ${DEP_LIB_SEARCH_DIR} ${X11_LIB_SEARCH_PATH})
            if (NOT XAW_LIBRARY OR NOT X11_Xt_FOUND)
                set(X11_FOUND FALSE)
            endif ()
        endif ()
        set(OGRE_LIBRARIES ${OGRE_LIBRARIES} ${ZZIP_LIBRARIES} ${ZLIB_LIBRARIES} ${FREEIMAGE_LIBRARIES} ${FREETYPE_LIBRARIES})
        if (APPLE)
            set(OGRE_LIBRARIES ${OGRE_LIBRARIES} ${X11_LIBRARIES} ${X11_Xt_LIBRARIES} ${XAW_LIBRARY} ${X11_Xrandr_LIB} ${Carbon_LIBRARIES} ${Cocoa_LIBRARIES})
        endif()
        if (NOT ZLIB_FOUND OR NOT ZZIP_FOUND)
            set(OGRE_DEPS_FOUND FALSE)
        endif ()
        if (NOT FREEIMAGE_FOUND AND NOT OGRE_CONFIG_FREEIMAGE)
            set(OGRE_DEPS_FOUND FALSE)
        endif ()
        if (NOT FREETYPE_FOUND)
            set(OGRE_DEPS_FOUND FALSE)
        endif ()
        if (UNIX AND NOT APPLE)
            if (NOT X11_FOUND)
                set(OGRE_DEPS_FOUND FALSE)
            endif ()
        endif ()
        if (NOT OGRE_DEPS_FOUND)
            message(status "Could not find all required dependencies for the Ogre package.")
            set(OGRE_FOUND FALSE)
        endif ()
    endif()

    file (STRINGS "${OGRE_H_PATH}/OgrePrerequisites.h" _CEGUIOGREVERSIONDEFINES REGEX ".*#define OGRE_VERSION_.*[0-9]+")

    foreach (ver MAJOR MINOR PATCH)
        set (CEGUI_FOUND_OGRE_VERSION_${ver} 0)
        if ("${_CEGUIOGREVERSIONDEFINES}" MATCHES ".*#define OGRE_VERSION_${ver} ([0-9]+).*")
            set (CEGUI_FOUND_OGRE_VERSION_${ver} "${CMAKE_MATCH_1}")
        endif()
    endforeach()
else()
    set (OGRE_INCLUDE_DIR)
    set (OGRE_LIBRARIES)
    set (OGRE_LIBRARIES_DBG)
    set (CEGUI_FOUND_OGRE_VERSION_MAJOR 0)
    set (CEGUI_FOUND_OGRE_VERSION_MINOR 0)
    set (CEGUI_FOUND_OGRE_VERSION_PATCH 0)
endif()

