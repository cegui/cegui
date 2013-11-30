################################################################################
# Custom cmake module for CEGUI to find OGRE
################################################################################
include(FindPackageHandleStandardArgs)
include(DetermineOgreThreadProvider)

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
    ${OGRE_PATH_SUFFIXES} include OgreMain/include
)

# Find the Ogre include headers.
find_path(OGRE_H_PATH
          NAMES Ogre.h
          PATHS ${OGRE_PREFIX_PATH}
          PATH_SUFFIXES ${OGRE_PATH_SUFFIXES} OGRE)

# Also find the build settings Ogre header.
find_path(OGRE_H_BUILD_SETTINGS_PATH
          NAMES OgreBuildSettings.h
          PATHS ${OGRE_PREFIX_PATH}
          PATH_SUFFIXES ${OGRE_PATH_SUFFIXES} OGRE)

# Find the release and debug libraries.
find_library(OGRE_LIB NAMES OgreMain
    PATHS ${OGRE_PREFIX_PATH}
    PATH_SUFFIXES Release lib/Release
)
find_library(OGRE_LIB_DBG NAMES OgreMain_d
    PATHS ${OGRE_PREFIX_PATH}
    PATH_SUFFIXES Debug lib/Debug
)
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
        set (OGRE_LIBRARIES_DBG ${OGRE_LIB_DBG};${Boost_THREAD_LIBRARY_DEBUG};${Boost_SYSTEM_LIBRARY_DEBUG};${Boost_DATE_TIME_LIBRARY_DEBUG};${Boost_CHRONO_LIBRARY_DEBUG})
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

