################################################################################
# Custom cmake module for CEGUI to find OGRE
################################################################################
# Additionally this script searches for the following optional
# parts of the Ogre package:
#  RenderSystem_GL, RenderSystem_GL3Plus,
#  RenderSystem_GLES, RenderSystem_GLES2,
#  RenderSystem_Direct3D9, RenderSystem_Direct3D11
#
# For each of these components, the following variables are defined:
#
#  OGRE_${COMPONENT}_FOUND - ${COMPONENT} is available
#  OGRE_${COMPONENT}_INCLUDE_DIRS - additional include directories for ${COMPONENT}
#  OGRE_${COMPONENT}_LIBRARIES - link these to use ${COMPONENT}
#  OGRE_${COMPONENT}_BINARY_REL - location of the component binary (win32 non-static only, release)
#  OGRE_${COMPONENT}_BINARY_DBG - location of the component binary (win32 non-static only, debug)
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

set (OGRE_INCLUDE_DIRS ${OGRE_H_PATH})

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

if (NOT OGRE_LIB AND OGRE_LIB_DBG)
    set(OGRE_LIB OGRE_LIB_DBG)
endif()
             
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
        find_package(OpenGL QUIET)
        find_package(OpenGLES QUIET)
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
        if (OGRE_LIBRARIES_DBG)
            set(OGRE_LIBRARIES_DBG ${OGRE_LIBRARIES_DBG} ${ZZIP_LIBRARIES} ${ZLIB_LIBRARIES} ${FREEIMAGE_LIBRARIES} ${FREETYPE_LIBRARIES})
        endif()
        if (APPLE)
            set(OGRE_LIBRARIES ${OGRE_LIBRARIES} ${X11_LIBRARIES} ${X11_Xt_LIBRARIES} ${XAW_LIBRARY} ${X11_Xrandr_LIB} ${Carbon_LIBRARIES} ${Cocoa_LIBRARIES})
            if (OGRE_LIBRARIES_DBG)
                set(OGRE_LIBRARIES_DBG ${OGRE_LIBRARIES_DBG} ${X11_LIBRARIES} ${X11_Xt_LIBRARIES} ${XAW_LIBRARY} ${X11_Xrandr_LIB} ${Carbon_LIBRARIES} ${Cocoa_LIBRARIES})
            endif()
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
            set(OGRE_LIBRARIES ${OGRE_LIBRARIES} ${X11_LIBRARIES} ${X11_Xt_LIB} ${XAW_LIBRARY} ${X11_Xrandr_LIB} pthread dl)
            if (OGRE_LIBRARIES_DBG)
                set(OGRE_LIBRARIES_DBG ${OGRE_LIBRARIES_DBG} ${X11_LIBRARIES} ${X11_Xt_LIB} ${XAW_LIBRARY} ${X11_Xrandr_LIB} pthread dl)
            endif()
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

# Generate debug names from given release names                                                                       
macro(get_debug_names PREFIX)
    foreach(i ${${PREFIX}})
        set(${PREFIX}_DBG ${${PREFIX}_DBG} ${i}d ${i}D ${i}_d ${i}_D ${i}_debug ${i})
    endforeach(i)
endmacro(get_debug_names)
# Couple a set of release AND debug libraries (or frameworks)
macro(make_library_set PREFIX)
    if (${PREFIX}_FWK)
        set(${PREFIX} ${${PREFIX}_FWK})
    elseif (${PREFIX}_REL AND ${PREFIX}_DBG)
        set(${PREFIX} optimized ${${PREFIX}_REL} debug ${${PREFIX}_DBG})
    elseif (${PREFIX}_REL)
        set(${PREFIX} ${${PREFIX}_REL})
    elseif (${PREFIX}_DBG)
        set(${PREFIX} ${${PREFIX}_DBG})
    endif ()
endmacro(make_library_set)

#########################################################
# Find Ogre plugins
#########################################################        
macro(ogre_find_plugin PLUGIN HEADER)
    # On Unix, the plugins might have no prefix
    if (CMAKE_FIND_LIBRARY_PREFIXES)
        set(TMP_CMAKE_LIB_PREFIX ${CMAKE_FIND_LIBRARY_PREFIXES})
        set(CMAKE_FIND_LIBRARY_PREFIXES ${CMAKE_FIND_LIBRARY_PREFIXES} "")
    endif()

    # strip RenderSystem_ or Plugin_ prefix from plugin name
    string(REPLACE "RenderSystem_" "" PLUGIN_TEMP ${PLUGIN})
    string(REPLACE "Plugin_" "" PLUGIN_NAME ${PLUGIN_TEMP})

    # header files for plugins are not usually needed, but find them anyway if they are present
    set(OGRE_PLUGIN_PATH_SUFFIXES
        PlugIns PlugIns/${PLUGIN_NAME} Plugins Plugins/${PLUGIN_NAME} ${PLUGIN} 
        RenderSystems RenderSystems/${PLUGIN_NAME} ${ARGN})
    find_path(OGRE_${PLUGIN}_INCLUDE_DIR NAMES ${HEADER} 
        HINTS ${OGRE_INCLUDE_DIRS} ${OGRE_PREFIX_SOURCE}  
        PATH_SUFFIXES ${OGRE_PLUGIN_PATH_SUFFIXES})
    # find link libraries for plugins
    set(OGRE_${PLUGIN}_LIBRARY_NAMES "${PLUGIN}${OGRE_LIB_SUFFIX}")
    get_debug_names(OGRE_${PLUGIN}_LIBRARY_NAMES)
    set(OGRE_${PLUGIN}_LIBRARY_FWK ${OGRE_LIBRARY_FWK})
    find_library(OGRE_${PLUGIN}_LIBRARY_REL NAMES ${OGRE_${PLUGIN}_LIBRARY_NAMES}
        HINTS "${OGRE_BUILD}/lib" ${OGRE_LIBRARY_DIRS} ${OGRE_FRAMEWORK_PATH} PATH_SUFFIXES "" OGRE OGRE-${OGRE_VERSION} opt Release Release/opt RelWithDebInfo RelWithDebInfo/opt MinSizeRel MinSizeRel/opt)
    find_library(OGRE_${PLUGIN}_LIBRARY_DBG NAMES ${OGRE_${PLUGIN}_LIBRARY_NAMES_DBG}
        HINTS "${OGRE_BUILD}/lib" ${OGRE_LIBRARY_DIRS} ${OGRE_FRAMEWORK_PATH} PATH_SUFFIXES "" OGRE OGRE-${OGRE_VERSION} opt Debug Debug/opt)
    make_library_set(OGRE_${PLUGIN}_LIBRARY)

    if (OGRE_${PLUGIN}_LIBRARY OR OGRE_${PLUGIN}_INCLUDE_DIR)
        set(OGRE_${PLUGIN}_FOUND TRUE)
        if (OGRE_${PLUGIN}_INCLUDE_DIR)
            set(OGRE_${PLUGIN}_INCLUDE_DIRS ${OGRE_${PLUGIN}_INCLUDE_DIR})
        endif()
        set(OGRE_${PLUGIN}_LIBRARIES ${OGRE_${PLUGIN}_LIBRARY})
    endif ()

    mark_as_advanced(OGRE_${PLUGIN}_INCLUDE_DIR OGRE_${PLUGIN}_LIBRARY_REL OGRE_${PLUGIN}_LIBRARY_DBG OGRE_${PLUGIN}_LIBRARY_FWK)

    # look for plugin dirs
    if (OGRE_${PLUGIN}_FOUND)
        if (NOT OGRE_PLUGIN_DIR_REL OR NOT OGRE_PLUGIN_DIR_DBG)
            if (WIN32)
                set(OGRE_PLUGIN_SEARCH_PATH_REL 
                    ${OGRE_LIBRARY_DIR_REL}/..
                    ${OGRE_LIBRARY_DIR_REL}/../..
                    ${OGRE_BIN_SEARCH_PATH}
                    )
                set(OGRE_PLUGIN_SEARCH_PATH_DBG
                    ${OGRE_LIBRARY_DIR_DBG}/..
                    ${OGRE_LIBRARY_DIR_DBG}/../..
                    ${OGRE_BIN_SEARCH_PATH}
                    )
                find_path(OGRE_PLUGIN_DIR_REL NAMES "${PLUGIN}.dll" HINTS ${OGRE_PLUGIN_SEARCH_PATH_REL}
                    PATH_SUFFIXES "" bin bin/Release bin/RelWithDebInfo bin/MinSizeRel Release)
                find_path(OGRE_PLUGIN_DIR_DBG NAMES "${PLUGIN}_d.dll" HINTS ${OGRE_PLUGIN_SEARCH_PATH_DBG}
                    PATH_SUFFIXES "" bin bin/Debug Debug)
            elseif (UNIX)
                get_filename_component(OGRE_PLUGIN_DIR_TMP ${OGRE_${PLUGIN}_LIBRARY_REL} PATH)
                set(OGRE_PLUGIN_DIR_REL ${OGRE_PLUGIN_DIR_TMP} CACHE STRING "Ogre plugin dir (release)" FORCE)
                get_filename_component(OGRE_PLUGIN_DIR_TMP ${OGRE_${PLUGIN}_LIBRARY_DBG} PATH)
                set(OGRE_PLUGIN_DIR_DBG ${OGRE_PLUGIN_DIR_TMP} CACHE STRING "Ogre plugin dir (debug)" FORCE)  
            endif ()
        endif ()

        # find binaries
        if (NOT OGRE_STATIC)
            if (WIN32)
                find_file(OGRE_${PLUGIN}_REL NAMES "${PLUGIN}.dll" HINTS ${OGRE_PLUGIN_DIR_REL})
                find_file(OGRE_${PLUGIN}_DBG NAMES "${PLUGIN}_d.dll" HINTS ${OGRE_PLUGIN_DIR_DBG})
            endif()
            mark_as_advanced(OGRE_${PLUGIN}_REL OGRE_${PLUGIN}_DBG)
        endif()

    endif ()

    if (TMP_CMAKE_LIB_PREFIX)
        set(CMAKE_FIND_LIBRARY_PREFIXES ${TMP_CMAKE_LIB_PREFIX})
    endif ()
endmacro(ogre_find_plugin)

ogre_find_plugin(RenderSystem_GL OgreGLRenderSystem.h RenderSystems/GL/include)
ogre_find_plugin(RenderSystem_GL3Plus OgreGL3PlusRenderSystem.h RenderSystems/GL3Plus/include)
ogre_find_plugin(RenderSystem_GLES OgreGLESRenderSystem.h RenderSystems/GLES/include)
ogre_find_plugin(RenderSystem_GLES2 OgreGLES2RenderSystem.h RenderSystems/GLES2/include)
ogre_find_plugin(RenderSystem_Direct3D9 OgreD3D9RenderSystem.h RenderSystems/Direct3D9/include)
ogre_find_plugin(RenderSystem_Direct3D11 OgreD3D11RenderSystem.h RenderSystems/Direct3D11/include)

if (OGRE_STATIC)
    # check if dependencies for plugins are met
    if (NOT DirectX_FOUND)
        set(OGRE_RenderSystem_Direct3D9_FOUND FALSE)
    endif ()
    if (NOT DirectX_D3D11_FOUND)
        set(OGRE_RenderSystem_Direct3D11_FOUND FALSE)
    endif ()
    if (NOT OPENGL_FOUND)
        set(OGRE_RenderSystem_GL_FOUND FALSE)
    endif ()
    if (NOT OPENGL_FOUND)
        set(OGRE_RenderSystem_GL3Plus_FOUND FALSE)
    endif ()
    if (NOT OPENGLES_FOUND)
        set(OGRE_RenderSystem_GLES_FOUND FALSE)
    endif ()
    if (NOT OPENGLES2_FOUND)
        set(OGRE_RenderSystem_GLES2_FOUND FALSE)
    endif ()
  
    set(OGRE_RenderSystem_Direct3D9_LIBRARIES ${OGRE_RenderSystem_Direct3D9_LIBRARIES}
        ${DirectX_LIBRARIES}
    )
    set(OGRE_RenderSystem_Direct3D11_LIBRARIES ${OGRE_RenderSystem_Direct3D11_LIBRARIES}
        ${DirectX_D3D11_LIBRARIES}
    )
    set(OGRE_RenderSystem_GL_LIBRARIES ${OGRE_RenderSystem_GL_LIBRARIES}
        ${OPENGL_LIBRARIES}
    )
    set(OGRE_RenderSystem_GL3Plus_LIBRARIES ${OGRE_RenderSystem_GL3Plus_LIBRARIES}
        ${OPENGL_LIBRARIES}
    )
    set(OGRE_RenderSystem_GLES_LIBRARIES ${OGRE_RenderSystem_GLES_LIBRARIES}
        ${OPENGLES_LIBRARIES}
    )
    set(OGRE_RenderSystem_GLES2_LIBRARIES ${OGRE_RenderSystem_GLES2_LIBRARIES}
        ${OPENGLES2_LIBRARIES}
    )
endif ()

