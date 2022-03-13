################################################################################
# Top-level CMakeLists.txt file for Crazy Eddie's GUI System
################################################################################
cmake_minimum_required(VERSION 3.1)

if (APPLE AND NOT CMAKE_OSX_ARCHITECTURES)
    set( CMAKE_OSX_ARCHITECTURES x86_64 CACHE STRING "Build architectures for Mac OS X")
endif()

# default to Release build (it's what most people will use, except cegui devs)
if (NOT CMAKE_BUILD_TYPE)
    set( CMAKE_BUILD_TYPE Release CACHE STRING "Sets the configuration to build (Release, Debug, etc...)")
endif()

################################################################################
# Start of main Project definitions
################################################################################
project(cegui)

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")
include(CEGUIMacros)
include(CMakeDependentOption)

################################################################################
# Set up version information
################################################################################
set( CEGUI_VERSION_MAJOR 9999)
set( CEGUI_VERSION_MINOR 0)
set( CEGUI_VERSION_PATCH 0)
set( CEGUI_VERSION ${CEGUI_VERSION_MAJOR}.${CEGUI_VERSION_MINOR}.${CEGUI_VERSION_PATCH} )

#
# This controls the binary versioning - which is different to release versions
# defined above.
#
# The following describes what these components are and how they are intended to
# be managed.
#
#   - CURRENT: The binary interface version provided by the library.
#   - REVISION: Revision of binary interface specified in CURRENT
#   - AGE: Number of previous binary interfaces this library is compatible with.
#
# Prior to issuing a release, modify the values as follows:
#   - If there were no changes to the source, do not change any values.
#   - If there were changes to the implementation, but no changes to the ABI at all (that is, nothing was added, removed, or changed that affects ABI), increase REVISION.
#   - If there were changes that modified the ABI, but the ABI remains compatible with the previous release, increase CURRENT, increase AGE, and set REVISION to 0.
#   - If there were ABI breaking changes, increase CURRENT, set AGE to 0 and set REVISION to 0.
#
set( CEGUI_ABI_CURRENT 6)
set( CEGUI_ABI_REVISION 2)
set( CEGUI_ABI_AGE 4)
MATH( EXPR CEGUI_SOVERSION ${CEGUI_ABI_CURRENT}-${CEGUI_ABI_AGE} )
set( CEGUI_ABI_VERSION ${CEGUI_SOVERSION}.${CEGUI_ABI_AGE}.${CEGUI_ABI_REVISION} )

if (CMAKE_SIZEOF_VOID_P EQUAL 4)
    set(CEGUI_ARCH_SUFFIX x86)
else()
    set(CEGUI_ARCH_SUFFIX x64)
endif()

################################################################################
# Check for unsupported compilers
################################################################################
cegui_check_mingw()
cegui_check_msvc()

################################################################################
# Check for libs and other packages we might use.
################################################################################
# be sure to search the provided dependencies on Win32 and Mac OS X platforms
set(CEGUI_DEPENDENCIES_DIR "${PROJECT_SOURCE_DIR}/dependencies" CACHE PATH "")
if (WIN32 OR APPLE)
    list(APPEND CMAKE_PREFIX_PATH ${CEGUI_DEPENDENCIES_DIR})
    # prefer the dylib deps in the dependencies over some other framework
    set(CMAKE_FIND_FRAMEWORK LAST)
elseif (ANDROID)
    set(CMAKE_FIND_ROOT_PATH ${CEGUI_DEPENDENCIES_DIR} "${CMAKE_FIND_ROOT_PATH}")
    set(CMAKE_LIBRARY_ARCHITECTURE ${ANDROID_ABI})
endif()

# Look for packages with CMake default Find* modules to prevent messing up our variables from CMake modules.
# GTK2 finds freetype inside and sets FREETYPE_FOUND, so we must call our find_package(Freetype) after this.
find_package(GTK2 COMPONENTS gtk)
# set for consistency with other deps
set (GTK2_INCLUDE_DIR ${GTK2_INCLUDE_DIRS})

# Look for packages
find_package(PCRE)
find_package(Freetype)
find_package(Minizip)
find_package(Fribidi)
find_package(Raqm)

if (NOT WIN32 AND NOT ANDROID)
    find_package(Iconv REQUIRED)
endif()

find_package(OpenGL)
find_package(GLEW)
find_package(GLM REQUIRED)
find_package(GLFW)
find_package(GLFW3)
find_package(SDL2)
find_package(SDL2_image)

set( CEGUI_SFML_COMPONENTS system window graphics )
if( WIN32 OR ANDROID OR IOS )
    list( APPEND CEGUI_SFML_COMPONENTS main )
endif()
set( SFML_FIND_REQUIRED FALSE )
find_package(SFML 2.3 COMPONENTS ${CEGUI_SFML_COMPONENTS})

find_package(DirectXSDK)
find_package(Irrlicht)

find_package(OGRE CONFIG)
if(NOT ${OGRE_FOUND})
    unset(OGRE_FOUND CACHE)
    unset(OGRE_DIR CACHE)
    find_package(Ogre)
endif()
if(${OGRE_FOUND})
    if (${OGRE_VERSION} VERSION_GREATER 1.11)
        set(CEGUI_FOUND_OGRE_VERSION_MAJOR ${OGRE_VERSION_MAJOR})
        set(CEGUI_FOUND_OGRE_VERSION_MINOR ${OGRE_VERSION_MINOR})
        set(CEGUI_FOUND_OGRE_VERSION_PATCH ${OGRE_VERSION_PATCH})
    elseif (${OGRE_VERSION} VERSION_LESS 1.10)
        find_package(OIS)
    endif()
endif()

find_package(DirectFB)
find_package(OpenGLES)
find_package(OpenGLES2)
find_package(OpenGLES3)
find_package(Epoxy)

find_package(PugiXML)
find_package(EXPAT)
find_package(XercesC)
find_package(LibXml2)
find_package(TinyXML2)

find_package(DevIL)
find_package(FreeImage)
find_package(SILLY)
find_package(Corona)
find_package(PVRTools)

find_package(Lua51)
find_package(TOLUAPP)

## search python3 libs
find_package(PythonLibs 3.5)
string(REGEX REPLACE "^([0-9]+)\\..*$" "\\1" PYTHON_VERSION_MAJOR "${PYTHONLIBS_VERSION_STRING}")
if(${PYTHON_VERSION_MAJOR} AND ${PYTHON_VERSION_MAJOR} EQUAL 3)
        set(PYTHON3LIBS_FOUND     ${PYTHONLIBS_FOUND}    CACHE STRING   "python3 found")
        set(PYTHON3_LIBRARIES     ${PYTHON_LIBRARIES}    CACHE FILEPATH "python3 lib path")
        set(PYTHON3_INCLUDE_DIRS  ${PYTHON_INCLUDE_DIRS} CACHE PATH     "python3 include dir path")
        mark_as_advanced(PYTHON3LIBS_FOUND PYTHON3_LIBRARIES PYTHON3_INCLUDE_DIRS)
endif()

## search python libs in version of default interpreter
find_package(PythonInterp)
find_package(PythonVersion)
if (UNIX AND PYTHON_VERSION_FOUND)
  find_package(PythonLibs ${PYTHON_VERSION_STRING} EXACT)
else()
  find_package(PythonLibs)
endif()
find_package(Boost 1.36.0 COMPONENTS python unit_test_framework system timer)
find_package(SWIG 3.0.8)

find_package(Doxygen)

## Search for ccache
find_program(CCACHE_FOUND ccache)
if(CCACHE_FOUND)
    set(ENV(CCACHE_CPP2) "yes")
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK ccache)
endif(CCACHE_FOUND)
mark_as_advanced(CCACHE_FOUND)

if (GLEW_FOUND OR EPOXY_FOUND)
    set (OPENGL_LOADER_FOUND TRUE)
else ()
    set (OPENGL_LOADER_FOUND FALSE)
endif ()

################################################################################
# Define the configurable options
################################################################################
cmake_dependent_option( CEGUI_MSVC_STATIC_RUNTIME "Specifies whether to the static runtime (/MT and /MTd) or the DLL runtime (/MD and /MDd).
NOTE: This will also affect which set of dependency libraries are linked with." FALSE "MSVC" FALSE )
cmake_dependent_option( CEGUI_BUILD_SHARED_LIBS_WITH_STATIC_DEPENDENCIES "For WIN32 and APPLE where the dependency pack is used, prefer the static
dependency libs over the shared/dynamic ones.  NOTE: On Windows you also need to
be mindful of which C/C++ runtime setting has been used to compile the various
components - they must all match or you will get crashes, heap corruption and/or
other issues." FALSE "WIN32 OR APPLE" FALSE)
option( CEGUI_BUILD_STATIC_CONFIGURATION "Specifies whether the static configs will be built. (NB: Shared configs are always built)" FALSE)
option( CEGUI_BUILD_DYNAMIC_CONFIGURATION "Dynamic libs to be built. Always true except for Android" TRUE)
option( CEGUI_BUILD_STATIC_FACTORY_MODULE "For static CEGUI builds, specifies whether to include the getWindowFactoryModule method.  When false, the developer must provide a custom implementation of the method" FALSE)
# sanity check on static build option for factory method
if (CEGUI_BUILD_STATIC_FACTORY_MODULE AND NOT CEGUI_BUILD_STATIC_CONFIGURATION)
    message(WARNING "CEGUI needs to be built statically (CEGUI_BUILD_STATIC_CONFIGURATION) if building the static factory method. CEGUI_BUILD_STATIC_FACTORY_MODULE is disabled.")
    set(CEGUI_BUILD_STATIC_FACTORY_MODULE FALSE)
endif()

if (PCRE_FOUND)
    set(CEGUI_REGEX_MATCHER "PCRE" CACHE STRING "Specifies a regular expression matcher used for editbox string validation")
    set_property(CACHE CEGUI_REGEX_MATCHER PROPERTY STRINGS "PCRE" "std" "None")
else ()
    set(CEGUI_REGEX_MATCHER "std" CACHE STRING "Specifies a regular expression matcher used for editbox string validation")
    set_property(CACHE CEGUI_REGEX_MATCHER PROPERTY STRINGS "std" "None")
endif ()

if (CEGUI_REGEX_MATCHER STREQUAL "PCRE")
    set(CEGUI_REGEX_MATCHER_PCRE TRUE)
elseif (CEGUI_REGEX_MATCHER STREQUAL "std")
    set(CEGUI_REGEX_MATCHER_STD TRUE)
endif()

option( CEGUI_BUILD_RESOURCE_PROVIDER_MINIZIP "Specifies whether to build the minizip based resource provider" ${MINIZIP_FOUND} )
option( CEGUI_USE_DEFAULT_LOGGER "Specifies whether to build and use the DefaultLogger implementation" TRUE)

option( CEGUI_BUILD_COMMON_DIALOGS "Specifies whether to build the CommonDialogs library, which contains the code for the ColourPicker and other dialogs" TRUE)

option( CEGUI_BUILD_XMLPARSER_PUGIXML "Specifies whether to build the PugiXML based XMLParser module" ${PUGIXML_FOUND} )
option( CEGUI_BUILD_XMLPARSER_EXPAT "Specifies whether to build the Expat based XMLParser module" ${EXPAT_FOUND} )
option( CEGUI_BUILD_XMLPARSER_XERCES "Specifies whether to build the Xerces-C++ based XMLParser module" ${XERCESC_FOUND} )
option( CEGUI_BUILD_XMLPARSER_LIBXML2 "Specifies whether to build the libxml2 based XMLParser module" ${LIBXML2_FOUND} )
option( CEGUI_BUILD_XMLPARSER_TINYXML2 "Specifies whether to build the TinyXML2 based XMLParser module" ${TINYXML2_FOUND} )

option( CEGUI_BUILD_IMAGECODEC_SILLY "Specifies whether to build the SILLY based ImageCodec module" ${SILLY_FOUND} )
option( CEGUI_BUILD_IMAGECODEC_DEVIL "Specifies whether to build the DevIL based ImageCodec module" ${DEVIL_FOUND} )
option( CEGUI_BUILD_IMAGECODEC_FREEIMAGE "Specifies whether to build the FreeImage based ImageCodec module" ${FREEIMAGE_FOUND} )
option( CEGUI_BUILD_IMAGECODEC_CORONA "Specifies whether to build the Corona based ImageCodec module" ${CORONA_FOUND} )
option( CEGUI_BUILD_IMAGECODEC_STB "Specifies whether to build the STB based ImageCodec module" FALSE )
option( CEGUI_BUILD_IMAGECODEC_TGA "Specifies whether to build the based TGA only ImageCodec module" FALSE )
option( CEGUI_BUILD_IMAGECODEC_PVR "Specifies whether to build the PVR only ImageCodec module" ${PVRTOOLS_FOUND} )
cegui_dependent_option( CEGUI_BUILD_IMAGECODEC_SDL2 "Specifies whether to build the SDL2 ImageCodec module" "SDL2_FOUND;SDL2IMAGE_FOUND" )

cegui_dependent_option( CEGUI_BUILD_RENDERER_OPENGL "Specifies whether to build the old OpenGL 1.2 (fixed pipeline) renderer module." "OPENGL_gl_LIBRARY;GLM_FOUND;GLEW_FOUND" )
cegui_dependent_option( CEGUI_BUILD_RENDERER_OPENGL3 "Specifies whether to build the renderer module that supports OpenGL 3.2 (core profile) and OpenGL ES 2.0." "OPENGL_gl_LIBRARY;GLM_FOUND;OPENGL_LOADER_FOUND")
option( CEGUI_BUILD_RENDERER_OGRE "Specifies whether to build the Ogre renderer module" ${OGRE_FOUND} )
option( CEGUI_BUILD_RENDERER_IRRLICHT "Specifies whether to build the Irrlicht renderer module" ${IRRLICHT_FOUND} )
option( CEGUI_BUILD_RENDERER_DIRECTFB "Specifies whether to build the DirectFB renderer module (not supported!)" FALSE )
cegui_dependent_option( CEGUI_BUILD_RENDERER_DIRECT3D11 "Specifies whether to build the Direct3D 11 renderer module" "DIRECTXSDK_FOUND;NOT DIRECTXSDK_MAX_D3D LESS 11" )
option( CEGUI_BUILD_RENDERER_NULL "Specifies whether to build the null renderer module" FALSE )
option( CEGUI_BUILD_RENDERER_OPENGLES "Specifies whether to build the OpenGL ES 1 renderer module" ${OPENGLES_FOUND} )
option( CEGUI_BUILD_RENDERER_OPENGLES2_ALTERNATE "Specifies whether to build the alternate OpenGL ES 2.0 renderer module" ${OPENGLES2_FOUND})
option( CEGUI_BUILD_RENDERER_OPENGLES2_ALTERNATE_WITH_GLES3_SUPPORT "Specifies whether to build build the alternate OpenGL ES 2.0 renderer module with OpenGL ES 3.0 features" ${OPENGLES3_FOUND})

# FIXME: defaulted to FALSE because the Lua module is outdated and doesn't compile
cegui_dependent_option( CEGUI_BUILD_LUA_MODULE "Specifies whether to build the Lua based script module" FALSE) #"LUA51_FOUND;TOLUAPP_FOUND" )
cegui_dependent_option( CEGUI_BUILD_LUA_GENERATOR "Specifies whether to build the custom tolua binding generator 'toluappcegui'" FALSE) #"LUA51_FOUND;TOLUAPP_FOUND" )

cegui_dependent_option( CEGUI_BUILD_PYTHON_MODULES_PYPLUSPLUS "Specifies whether to build the Python extension module(s) via boost and Py++" "PYTHONINTERP_FOUND;PYTHONLIBS_FOUND;Boost_PYTHON_FOUND" )
cegui_dependent_option( CEGUI_BUILD_PYTHON_MODULES_SWIG "Specifies whether to build the Python extension module(s) via SWIG" "PYTHON3LIBS_FOUND;SWIG_FOUND" )

option( CEGUI_OPTION_SAFE_LUA_MODULE "Specifies whether to enable extra validation in the Lua script module in non-debug builds" FALSE )

set( CEGUI_STRING_CLASS "UTF-32" CACHE STRING "CEGUI offers three CEGUI::String class configurations
UTF-8 : ASCII, UTF-8 and UTF-32 aware, minimal storage, slightly less efficient, stores code units as char (std::string) internally - mind this fact when manipulating the String
UTF-32 : ASCII, UTF-8 and UTF-32 aware, stores 32 bit per character, stores code points as char32_t (std::u32string) internally
std : regular std::string used as CEGUI::String, not Unicode aware"
)
set_property(CACHE CEGUI_STRING_CLASS PROPERTY STRINGS "UTF-8" "UTF-32" "std")

if (CEGUI_STRING_CLASS STREQUAL "UTF-8")
    set(CEGUI_STRING_CLASS 1)
elseif (CEGUI_STRING_CLASS STREQUAL "std")
    set(CEGUI_STRING_CLASS 3)
else()
    set(CEGUI_STRING_CLASS 2) # UTF-32 by default
endif()

# Text rendering, BIDI

option( CEGUI_USE_FREETYPE "Specifies whether to include Freetype font support" ${FREETYPE_FOUND} )
option( CEGUI_USE_FRIBIDI "Specifies whether bi-directional text will be supported via the fribidi library." OFF )
option( CEGUI_USE_MINIBIDI "Specifies whether bi-directional text will be supported via the embedded minibidi." OFF )
cegui_dependent_option( CEGUI_USE_RAQM "Specifies whether to include support for kerning and for complex text layout using in CEGUI. This requires libraqm, harfbuzz, freetype and fribidi" "RAQM_FOUND;HARFBUZZ_FOUND;FREETYPE_FOUND;FRIBIDI_FOUND" )

if (CEGUI_USE_RAQM AND NOT CEGUI_USE_FRIBIDI)
    set(CEGUI_USE_FRIBIDI ON)
endif()
if (CEGUI_USE_FRIBIDI AND NOT FRIBIDI_FOUND)
    message(WARNING "CEGUI_USE_FRIBIDI was disabled because Fribidi is not found.")
    set(CEGUI_USE_FRIBIDI OFF)
endif()
if (CEGUI_USE_MINIBIDI AND CEGUI_USE_FRIBIDI)
    message(WARNING "You may not enable both CEGUI_USE_FRIBIDI and CEGUI_USE_MINIBIDI. Only CEGUI_USE_FRIBIDI will be left.")
    set(CEGUI_USE_MINIBIDI OFF)
endif()
if (CEGUI_USE_FRIBIDI OR CEGUI_USE_MINIBIDI OR CEGUI_USE_RAQM)
    set(CEGUI_BIDI_SUPPORT 1)
endif()
if (CEGUI_USE_FRIBIDI)
    list(APPEND FRIBIDI_DEFINITIONS FRIBIDI_NO_DEPRECATED) # FIXME: fribidi doesn't check this through config.h
endif()

# Samples

set( CEGUI_SAMPLES_DESKTOP_OPENGL_CONTEXT_CREATION_API NONE )
if (CEGUI_BUILD_RENDERER_OPENGL OR CEGUI_BUILD_RENDERER_OPENGL3)
    if (GLEW_FOUND AND EPOXY_FOUND)
        option (CEGUI_USE_GLEW "Specifies whether to use GLEW as an OpenGL loading library." TRUE)
        option (CEGUI_USE_EPOXY "Specifies whether to use Epoxy as an OpenGL loading library." FALSE)
        if (CEGUI_USE_GLEW AND CEGUI_USE_EPOXY)
            message (FATAL_ERROR "You may not enable both \"CEGUI_USE_GLEW\" and \"CEGUI_USE_EPOXY\". Please choose only one of these options.")
        endif ()
    else ()
        set (CEGUI_USE_GLEW "${GLEW_FOUND}")
        set (CEGUI_USE_EPOXY "${EPOXY_FOUND}")
    endif ()
    if (CEGUI_BUILD_RENDERER_OPENGL AND NOT CEGUI_USE_GLEW)
        message (FATAL_ERROR "Building the old OpenGL 1.2 (fixed pipeline) renderer module is only supported using GLEW.")
    endif ()
    set( CEGUI_USE_GLFW_VER none )
    if (NOT ANDROID)
        set (CEGUI_GLFW_VER "" CACHE STRING "Specifies which major version of GLFW to use for the samples (2 or 3). The default is to look for both, giving priority to version 2.")
        set_property(CACHE CEGUI_GLFW_VER PROPERTY STRINGS "" "2" "3")

        if (((CEGUI_GLFW_VER STREQUAL "2") OR (CEGUI_GLFW_VER STREQUAL "")) AND GLFW_FOUND)
            set (CEGUI_USE_GLFW_VER "2")
        elseif (((CEGUI_GLFW_VER STREQUAL "3")  OR (CEGUI_GLFW_VER STREQUAL "")) AND GLFW3_FOUND)
            set (CEGUI_USE_GLFW_VER "3")
        endif ()
        if( CEGUI_USE_GLFW_VER STREQUAL 3 )
            set( CEGUI_SAMPLES_DESKTOP_OPENGL_CONTEXT_CREATION_API NATIVE CACHE STRING "In the sample framework, which API to use for desktop OpenGL context creation. Possible values are \"EGL\" and \"NATIVE\" (default). This variable is only used with GLFW >= 3.2." )
            mark_as_advanced( CEGUI_SAMPLES_DEKSTOP_OPENGL_CONTEXT_CREATION_API )
        endif()
    endif ()
endif ()

set( CEGUI_SAMPLES_OPENGL_ES_2_CONTEXT_CREATION_API NONE )
if (CEGUI_BUILD_RENDERER_OPENGL3 AND CEGUI_USE_EPOXY)
    set (CEGUI_SUPPORT_RENDERER_OPENGLES2 TRUE)
    set (CEGUI_OPENGL_VER_MAJOR_FORCE "" CACHE STRING "Run the samples as if a specific OpenGL (desktop or ES) major version is used. Useful for testing.")
    mark_as_advanced (CEGUI_OPENGL_VER_MAJOR_FORCE)
    set (CEGUI_OPENGL_VER_MINOR_FORCE "0" CACHE STRING "Run the samples as if a specific OpenGL (desktop or ES) minor version is used. Useful for testing.")
    mark_as_advanced (CEGUI_OPENGL_VER_MINOR_FORCE)
    if( CEGUI_GLFW_VER STREQUAL "3" )
        set( CEGUI_SAMPLES_OPENGL_ES_2_CONTEXT_CREATION_API EGL CACHE STRING "In the sample framework, which API to use for OpenGL ES 2.0 context creation. Possible values are \"EGL\" (default) and \"NATIVE\". This variable is only used with GLFW >= 3.2." )
        mark_as_advanced( CEGUI_SAMPLES_OPENGL_ES_2_CONTEXT_CREATION_API )
    endif()
else ()
    set (CEGUI_SUPPORT_RENDERER_OPENGLES2 FALSE)
endif ()

if (CEGUI_SUPPORT_RENDERER_OPENGLES2 AND ((CEGUI_USE_GLFW_VER STREQUAL "3") OR ANDROID))
    set (CEGUI_SAMPLES_SUPPORT_RENDERER_OPENGLES2 TRUE)
else ()
    set (CEGUI_SAMPLES_SUPPORT_RENDERER_OPENGLES2 FALSE)
endif ()

option (CEGUI_WARNINGS_AS_ERRORS "Specifies whether to treat warnings as errors. Recommended if you're developing CEGUI itself." OFF)

set (CEGUI_EXTRA_FLAGS "")

if (CMAKE_COMPILER_IS_GNUCXX OR (CMAKE_CXX_COMPILER_ID STREQUAL "Clang"))
    set (CEGUI_EXTRA_FLAGS "${CEGUI_EXTRA_FLAGS} -fsigned-char")
endif ()

if (CMAKE_COMPILER_IS_GNUCXX)
    set (CEGUI_EXTRA_FLAGS "${CEGUI_EXTRA_FLAGS} -Wall -Wextra -Wundef")
endif ()

if (CEGUI_WARNINGS_AS_ERRORS)
    if (CMAKE_COMPILER_IS_GNUCXX OR (CMAKE_CXX_COMPILER_ID STREQUAL "Clang"))
        set (CEGUI_EXTRA_FLAGS "${CEGUI_EXTRA_FLAGS} -Werror")
    elseif (MSVC)
        set (CEGUI_EXTRA_FLAGS "${CEGUI_EXTRA_FLAGS} /WX")
    else ()
        message (WARNING "You've specified \"CEGUI_WARNINGS_AS_ERRORS\" as \"ON\", but we don't know how to make it for your compiler. If you wish you can specifiy the required flags manually.")
    endif ()
endif ()

if( MINGW )
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-attributes")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-attributes")
endif()

set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${CEGUI_EXTRA_FLAGS}")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CEGUI_EXTRA_FLAGS}")
set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${CEGUI_EXTRA_FLAGS}")
set (CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} ${CEGUI_EXTRA_FLAGS}")
set (CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${CEGUI_EXTRA_FLAGS}")

# Here we check if Ogre is built to use boost, and if yes, if the necessary dependencies are available and found by CEGUI
set (CEGUI_OGRE_DEPS_AVAILABLE FALSE)
set (CEGUI_OGRE_SAMPLES_EXTRALIBS)
if (CEGUI_BUILD_RENDERER_OGRE)
    # Set the used version
    set(CEGUI_FOUND_OGRE_VERSION_MAJOR ${OGRE_VERSION_MAJOR})
    set(CEGUI_FOUND_OGRE_VERSION_MINOR ${OGRE_VERSION_MINOR})
    set(CEGUI_FOUND_OGRE_VERSION_PATCH ${OGRE_VERSION_PATCH})
endif()

if (CEGUI_BUILD_RENDERER_OGRE)
    if (NOT Boost_VERSION LESS 105000 AND "${OGRE_CONFIG_THREAD_PROVIDER}" STREQUAL "1") #Check additionally if we use boost as thread provider at all
        if (Boost_SYSTEM_FOUND)
            set (CEGUI_OGRE_SAMPLES_EXTRALIBS ${Boost_SYSTEM_LIBRARY})
            set (CEGUI_OGRE_DEPS_AVAILABLE TRUE)
        endif()
    else()
        set (CEGUI_OGRE_DEPS_AVAILABLE TRUE)
    endif()
endif()

# Activates Renderers in the SampleBrowser. By default all the Renderers that are built are activated. Some of them might require extra dependencies to run in the SampleBrowser and thus will be disabled if the deps are not given
if(CEGUI_BUILD_RENDERER_OPENGL OR CEGUI_BUILD_RENDERER_OPENGL3)
    set (CEGUI_SAMPLES_RENDERER_OPENGL_ACTIVE TRUE)
else()
    set (CEGUI_SAMPLES_RENDERER_OPENGL_ACTIVE FALSE)
endif()
set (CEGUI_SAMPLES_RENDERER_OGRE_ACTIVE ${CEGUI_BUILD_RENDERER_OGRE})
set (CEGUI_SAMPLES_RENDERER_IRRLICHT_ACTIVE ${CEGUI_BUILD_RENDERER_IRRLICHT})
set (CEGUI_SAMPLES_RENDERER_DIRECT3D11_ACTIVE ${CEGUI_BUILD_RENDERER_DIRECT3D11})
set (CEGUI_SAMPLES_RENDERER_DIRECTFB_ACTIVE ${CEGUI_BUILD_RENDERER_DIRECTFB})
set (CEGUI_SAMPLES_RENDERER_OPENGLES2_ALTERNATE_ACTIVE ${CEGUI_BUILD_RENDERER_OPENGLES2_ALTERNATE})

#Only build the opengles2 sample renderer for Android
if (NOT ANDROID)
    set (CEGUI_SAMPLES_RENDERER_OPENGLES2_ALTERNATE_ACTIVE FALSE)
endif()

# Ogre and OpenGL have dependencies for the sample browser, if these are not available then the samples will be OFF by default
if (CEGUI_BUILD_RENDERER_OGRE AND NOT CEGUI_OGRE_DEPS_AVAILABLE)
    set (_CEGUI_BUILD_SAMPLES_DEFAULT FALSE)
    if (${OGRE_VERSION} VERSION_LESS 1.10 AND NOT OIS_FOUND)
        message(WARNING "The SampleFramework is deactivated due to missing Ogre dependencies (OIS not found)")
    else()
        message(WARNING "The SampleFramework is deactivated due to missing Ogre dependencies (boost libraries)")
    endif()
endif()

if (CEGUI_SAMPLES_RENDERER_OPENGL_ACTIVE AND NOT (CEGUI_USE_GLFW_VER OR ANDROID))
    set (CEGUI_SAMPLES_RENDERER_OPENGL_ACTIVE FALSE)
    message(STATUS "Info: The OpenGL-, OpenGL3 and/or OpenGL ES 2 Renderers are disabled in the SampleBrowser because of missing dependencies (GLFW 2/3 library).  Please add the GFLW dependency and run 'Configure' in CMake again, in case you want to run the SampleBrowser using the OpenGL, OpenGL3 or OpenGL ES 2 Renderers.")
endif()

# If we use a Renderer then our samples should be ON by default
if (CEGUI_SAMPLES_RENDERER_OPENGL_ACTIVE OR CEGUI_SAMPLES_RENDERER_OGRE_ACTIVE OR CEGUI_SAMPLES_RENDERER_IRRLICHT_ACTIVE OR CEGUI_SAMPLES_RENDERER_DIRECT3D11_ACTIVE OR CEGUI_SAMPLES_RENDERER_DIRECTFB_ACTIVE OR CEGUI_SAMPLES_RENDERER_OPENGLES2_ALTERNATE_ACTIVE)
    set (_CEGUI_BUILD_SAMPLES_DEFAULT TRUE)
else()
    set (_CEGUI_BUILD_SAMPLES_DEFAULT FALSE)
endif()

# We set the Samples to the default value
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/samples")
    option( CEGUI_BUILD_SAMPLES "Specifies whether to build the CEGUI sample applications" ${_CEGUI_BUILD_SAMPLES_DEFAULT} )
else()
    unset(CEGUI_BUILD_SAMPLES CACHE)
endif()

if (UNIX AND NOT APPLE AND NOT WIN32)
    option( CEGUI_SAMPLES_USE_GTK2 "Specifies whether the sample applications will make use of the GTK2 UI for renderer selection." FALSE )
endif()

# Option wether to build application templates; default off
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/application_templates")
    option(CEGUI_BUILD_APPLICATION_TEMPLATES "Specifies whether to build the application templates." TRUE)
else()
    unset(CEGUI_BUILD_APPLICATION_TEMPLATES CACHE)
endif()

if (ANDROID)
    set(CEGUI_BUILD_STATIC_CONFIGURATION TRUE CACHE BOOL "Forcing static build for Android" FORCE)
    set(CEGUI_BUILD_STATIC_FACTORY_MODULE TRUE CACHE BOOL "Forcing static factory module for Android" FORCE)
    set(CEGUI_BUILD_DYNAMIC_CONFIGURATION FALSE CACHE BOOL "Disable dynamic build for Android" FORCE)
    set(CEGUI_BUILD_RENDERER_OPENGLES FALSE CACHE BOOL "Disable OpenGL ES 1 RenderSystem for Android" FORCE)
    if (CEGUI_BUILD_RENDERER_OPENGLES2_ALTERNATE AND CEGUI_BUILD_RENDERER_OGRE)
        message(SEND_ERROR "Either OpenGLES2 Renderer or Ogre Renderer can be built for android at once.  Please disable one of the renderers.")
    endif()
elseif (NOT APPLE AND CEGUI_BUILD_RENDERER_OPENGLES)
    find_package(EGL)
    set(CEGUI_BUILD_RENDERER_OPENGLES ${EGL_FOUND} CACHE BOOL "OpenGL ES 1 RenderSystem requires EGL on other systems than APPLE" FORCE)
endif()

if (WIN32 OR APPLE)
    set (CEGUI_BUILD_SUFFIX "_d" CACHE STRING "String holding a suffix appended to the name of output binaries (under CMake build, only used for debug).")
else()
    set (CEGUI_BUILD_SUFFIX "" CACHE STRING "String holding a suffix appended to the name of output binaries (under CMake build, only used for debug).")
endif()

if (CEGUI_BUILD_SUFFIX)
    set (CEGUI_HAS_BUILD_SUFFIX TRUE)
    set (CMAKE_DEBUG_POSTFIX ${CEGUI_BUILD_SUFFIX})
endif()

# Apple OS X specific options
if (APPLE)
    set( CEGUI_APPLE_DYLIB_INSTALL_PATH "@executable_path/../Frameworks" CACHE STRING "Specifies the install rpath for dylibs and frameworks." )
    set( CEGUI_APPLE_SYMLINK_DEPENDENCIES_TO_SAMPLE_APPS TRUE CACHE BOOL "Specifies how to reference the dylibs, frameworks and datafiles in the sample app bundles:
    TRUE: specifies that the files will be symlinked (saves space, but apps are not truly stand-alone)
    FALSE: specifies that actual copies of the files will be used" )
    option( CEGUI_APPLE_DYLIB_SET_VERSION_INFO "Specifies whether to set VERSION and SOVERSION for libraries." FALSE )
endif()

# unit tests
cmake_dependent_option( CEGUI_BUILD_TESTS "Specifies whether to build the unit tests." FALSE "Boost_UNIT_TEST_FRAMEWORK_FOUND" FALSE )

cmake_dependent_option( CEGUI_BUILD_PERFORMANCE_TESTS "Specifies whether to build the performance tests." FALSE "Boost_UNIT_TEST_FRAMEWORK_FOUND;Boost_TIMER_FOUND" FALSE )

# sanity check on unit tests
if ((CEGUI_BUILD_TESTS OR CEGUI_BUILD_PERFORMANCE_TESTS) AND NOT CEGUI_BUILD_RENDERER_NULL)
    message(SEND_ERROR "The CEGUI tests (option CEGUI_BUILD_TESTS or CEGUI_BUILD_PERFORMANCE_TESTS) require the null renderer (CEGUI_BUILD_RENDERER_NULL). Please enable it.")
endif()

# datafiles tests
cmake_dependent_option( CEGUI_BUILD_DATAFILES_TEST "Specifies whether to build the datafiles test." FALSE "PYTHONINTERP_FOUND" FALSE )

option( CEGUI_INSTALL_WITH_RPATH "Specifies whether to install with RPATH set to the install location (TRUE) or with no RPATH set (FALSE)." FALSE )

# custom install locations
set(CEGUI_INSTALL_INCLUDE_DIR "" CACHE PATH "Specifies custom location for CEGUI includes")
set(CEGUI_INSTALL_LIB_DIR "" CACHE PATH "Specifies custom location for CEGUI libraries")
set(CEGUI_INSTALL_DATA_DIR "" CACHE PATH "Specifies custom location for CEGUI datafiles")
set(CEGUI_INSTALL_DOC_DIR "" CACHE PATH "Specifies custom location for CEGUI docs")
set(CEGUI_INSTALL_PKGCONFIG_DIR "" CACHE PATH "Specifies custom location for CEGUI pkgconfig")
set(CEGUI_INSTALL_MODULE_DIR "" CACHE PATH "Specifies custom location for CEGUI module")
set(CEGUI_INSTALL_SAMPLE_DIR "" CACHE PATH "Specifies custom location for CEGUI samples") # TODO: put into /samples subdir!


################################################################################
# Define vars holding the names for all the libs we can build for CEGUI.
################################################################################
# Core lib name
cegui_set_library_name( CEGUI_BASE_LIBNAME CEGUIBase )

# Renderer module names.
cegui_set_library_name( CEGUI_OPENGL_RENDERER_LIBNAME CEGUIOpenGLRenderer )
cegui_set_library_name( CEGUI_OGRE_RENDERER_LIBNAME CEGUIOgreRenderer)
cegui_set_library_name( CEGUI_IRRLICHT_RENDERER_LIBNAME CEGUIIrrlichtRenderer )
cegui_set_library_name( CEGUI_DIRECT3D11_RENDERER_LIBNAME CEGUIDirect3D11Renderer )
cegui_set_library_name( CEGUI_NULL_RENDERER_LIBNAME CEGUINullRenderer )
cegui_set_library_name( CEGUI_OPENGLES_RENDERER_LIBNAME CEGUIOpenglEsRenderer )
cegui_set_library_name( CEGUI_OPENGLES2_RENDERER_ALTERNATE_LIBNAME CEGUIOpenglEs2RendererAlternate )
cegui_set_library_name( CEGUI_DIRECTFB_RENDERER_LIBNAME CEGUIDirectFBRenderer )

# XML parser module names
cegui_set_module_name( CEGUI_PUGIXML_PARSER_LIBNAME CEGUIPugiXMLParser )
cegui_set_module_name( CEGUI_EXPAT_PARSER_LIBNAME CEGUIExpatParser )
cegui_set_module_name( CEGUI_TINYXML2_PARSER_LIBNAME CEGUITinyXML2Parser )
cegui_set_module_name( CEGUI_XERCES_PARSER_LIBNAME CEGUIXercesParser )
cegui_set_module_name( CEGUI_LIBXML2_PARSER_LIBNAME CEGUILibXMLParser )

# ImageCodec module names
cegui_set_module_name( CEGUI_SILLY_IMAGECODEC_LIBNAME CEGUISILLYImageCodec )
cegui_set_module_name( CEGUI_DEVIL_IMAGECODEC_LIBNAME CEGUIDevILImageCodec )
cegui_set_module_name( CEGUI_FREEIMAGE_IMAGECODEC_LIBNAME CEGUIFreeImageImageCodec )
cegui_set_module_name( CEGUI_CORONA_IMAGECODEC_LIBNAME CEGUICoronaImageCodec )
cegui_set_module_name( CEGUI_TGA_IMAGECODEC_LIBNAME CEGUITGAImageCodec )
cegui_set_module_name( CEGUI_STB_IMAGECODEC_LIBNAME CEGUISTBImageCodec )
cegui_set_module_name( CEGUI_PVR_IMAGECODEC_LIBNAME CEGUIPVRImageCodec )
cegui_set_module_name( CEGUI_SDL2_IMAGECODEC_LIBNAME CEGUISDL2ImageCodec )

# WindowRenderer set module names
cegui_set_module_name( CEGUI_CORE_WR_LIBNAME CEGUICoreWindowRendererSet )

# Scripting module names and related items.
cegui_set_executable_name( CEGUI_TOLUAPP_GENERATOR_EXENAME toluappcegui )
cegui_set_library_name( CEGUI_LUA_SCRIPTMODULE_LIBNAME CEGUILuaScriptModule )

set( CEGUI_PYCEGUI_CORE_LIBNAME PyCEGUI )
set( CEGUI_PYCEGUI_OPENGL_RENDERER_LIBNAME PyCEGUIOpenGLRenderer )
set( CEGUI_PYCEGUI_OGRE_RENDERER_LIBNAME PyCEGUIOgreRenderer )
set( CEGUI_PYCEGUI_NULL_RENDERER_LIBNAME PyCEGUINullRenderer )

# SampleFramework executable related names
cegui_set_executable_name( CEGUI_SAMPLEFRAMEWORK_EXENAME CEGUISampleBrowser )

# Additional lib names
cegui_set_library_name( CEGUI_COMMON_DIALOGS_LIBNAME CEGUICommonDialogs )

################################################################################
# Select one of the XML parser modules to be the default, warning if none are
# available.
################################################################################
set(CEGUI_DEFAULT_XMLPARSER_TOOLTIP "Specifies the XMLParser module to use as the default")

if (CEGUI_BUILD_XMLPARSER_PUGIXML)
    set( CEGUI_OPTION_DEFAULT_XMLPARSER "PugiXMLParser" CACHE STRING "${CEGUI_DEFAULT_XMLPARSER_TOOLTIP}" )
    set( CEGUI_STATIC_XMLPARSER_MODULE ${CEGUI_PUGIXML_PARSER_LIBNAME} CACHE STRING "${CEGUI_DEFAULT_XMLPARSER_TOOLTIP}" )
elseif (CEGUI_BUILD_XMLPARSER_EXPAT)
    set( CEGUI_OPTION_DEFAULT_XMLPARSER "ExpatParser" CACHE STRING "${CEGUI_DEFAULT_XMLPARSER_TOOLTIP}" )
    set( CEGUI_STATIC_XMLPARSER_MODULE ${CEGUI_EXPAT_PARSER_LIBNAME} CACHE STRING "${CEGUI_DEFAULT_XMLPARSER_TOOLTIP}" )
elseif (CEGUI_BUILD_XMLPARSER_XERCES)
    set( CEGUI_OPTION_DEFAULT_XMLPARSER "XercesParser" CACHE STRING "${CEGUI_DEFAULT_XMLPARSER_TOOLTIP}" )
    set( CEGUI_STATIC_XMLPARSER_MODULE ${CEGUI_XERCES_PARSER_LIBNAME} CACHE STRING "${CEGUI_DEFAULT_XMLPARSER_TOOLTIP}" )
elseif (CEGUI_BUILD_XMLPARSER_LIBXML2)
    set( CEGUI_OPTION_DEFAULT_XMLPARSER "LibXMLParser" CACHE STRING "${CEGUI_DEFAULT_XMLPARSER_TOOLTIP}" )
    set( CEGUI_STATIC_XMLPARSER_MODULE ${CEGUI_LIBXML2_PARSER_LIBNAME} CACHE STRING "${CEGUI_DEFAULT_XMLPARSER_TOOLTIP}" )
elseif (CEGUI_BUILD_XMLPARSER_TINYXML2)
    set( CEGUI_OPTION_DEFAULT_XMLPARSER "TinyXML2Parser" CACHE STRING "${CEGUI_DEFAULT_XMLPARSER_TOOLTIP}" )
    set( CEGUI_STATIC_XMLPARSER_MODULE ${CEGUI_TINYXML2_PARSER_LIBNAME} CACHE STRING "${CEGUI_DEFAULT_XMLPARSER_TOOLTIP}" )
else()
    message(WARNING "None of the XML parser modules are going to be built.
You should ensure that CEGUI_OPTION_DEFAULT_XMLPARSER is set to something
appropriate.")
endif()

set_property(CACHE CEGUI_OPTION_DEFAULT_XMLPARSER PROPERTY STRINGS "PugiXMLParser" "ExpatParser" "XercesParser" "LibXMLParser" "TinyXML2Parser")
set_property(CACHE CEGUI_STATIC_XMLPARSER_MODULE PROPERTY STRINGS "${CEGUI_PUGIXML_PARSER_LIBNAME}" "${CEGUI_EXPAT_PARSER_LIBNAME}" "${CEGUI_XERCES_PARSER_LIBNAME}" "${CEGUI_LIBXML2_PARSER_LIBNAME}" "${CEGUI_TINYXML2_PARSER_LIBNAME}")

if(CEGUI_BUILD_DYNAMIC_CONFIGURATION)
    cegui_defaultmodule_sanity_test(CEGUI_OPTION_DEFAULT_XMLPARSER "PugiXMLParser" CEGUI_BUILD_XMLPARSER_PUGIXML)
    cegui_defaultmodule_sanity_test(CEGUI_OPTION_DEFAULT_XMLPARSER "ExpatParser" CEGUI_BUILD_XMLPARSER_EXPAT)
    cegui_defaultmodule_sanity_test(CEGUI_OPTION_DEFAULT_XMLPARSER "XercesParser" CEGUI_BUILD_XMLPARSER_XERCES)
    cegui_defaultmodule_sanity_test(CEGUI_OPTION_DEFAULT_XMLPARSER "LibXMLParser" CEGUI_BUILD_XMLPARSER_LIBXML2)
    cegui_defaultmodule_sanity_test(CEGUI_OPTION_DEFAULT_XMLPARSER "TinyXML2Parser" CEGUI_BUILD_XMLPARSER_TINYXML2)
endif()

if (CEGUI_BUILD_STATIC_CONFIGURATION)
    cegui_defaultmodule_sanity_test(CEGUI_STATIC_XMLPARSER_MODULE ${CEGUI_PUGIXML_PARSER_LIBNAME} CEGUI_BUILD_XMLPARSER_PUGIXML)
    cegui_defaultmodule_sanity_test(CEGUI_STATIC_XMLPARSER_MODULE ${CEGUI_EXPAT_PARSER_LIBNAME} CEGUI_BUILD_XMLPARSER_EXPAT)
    cegui_defaultmodule_sanity_test(CEGUI_STATIC_XMLPARSER_MODULE ${CEGUI_XERCES_PARSER_LIBNAME} CEGUI_BUILD_XMLPARSER_XERCES)
    cegui_defaultmodule_sanity_test(CEGUI_STATIC_XMLPARSER_MODULE ${CEGUI_LIBXML2_PARSER_LIBNAME} CEGUI_BUILD_XMLPARSER_LIBXML2)
    cegui_defaultmodule_sanity_test(CEGUI_STATIC_XMLPARSER_MODULE ${CEGUI_TINYXML2_PARSER_LIBNAME} CEGUI_BUILD_XMLPARSER_TINYXML2)
endif()

################################################################################
# Select one of the image codec modules to be the default, warning if none are
# available.
################################################################################
if (CEGUI_BUILD_IMAGECODEC_SILLY)
    set( CEGUI_OPTION_DEFAULT_IMAGECODEC "SILLYImageCodec" CACHE STRING "Specifies the ImageCodec module to use as the default" )
    set( CEGUI_STATIC_IMAGECODEC_MODULE ${CEGUI_SILLY_IMAGECODEC_LIBNAME} CACHE STRING "Specifies image codec library to link to samples in static builds." )
elseif (CEGUI_BUILD_IMAGECODEC_DEVIL)
    set( CEGUI_OPTION_DEFAULT_IMAGECODEC "DevILImageCodec" CACHE STRING "Specifies the ImageCodec module to use as the default" )
    set( CEGUI_STATIC_IMAGECODEC_MODULE ${CEGUI_DEVIL_IMAGECODEC_LIBNAME} CACHE STRING "Specifies image codec library to link to samples in static builds." )
elseif (CEGUI_BUILD_IMAGECODEC_FREEIMAGE)
    set( CEGUI_OPTION_DEFAULT_IMAGECODEC "FreeImageImageCodec" CACHE STRING "Specifies the ImageCodec module to use as the default" )
    set( CEGUI_STATIC_IMAGECODEC_MODULE ${CEGUI_FREEIMAGE_IMAGECODEC_LIBNAME} CACHE STRING "Specifies image codec library to link to samples in static builds." )
elseif (CEGUI_BUILD_IMAGECODEC_STB)
    set( CEGUI_OPTION_DEFAULT_IMAGECODEC "STBImageCodec" CACHE STRING "Specifies the ImageCodec module to use as the default" )
    set( CEGUI_STATIC_IMAGECODEC_MODULE ${CEGUI_STB_IMAGECODEC_LIBNAME} CACHE STRING "Specifies image codec library to link to samples in static builds." )
elseif (CEGUI_BUILD_IMAGECODEC_CORONA)
    set( CEGUI_OPTION_DEFAULT_IMAGECODEC "CoronaImageCodec" CACHE STRING "Specifies the ImageCodec module to use as the default" )
    set( CEGUI_STATIC_IMAGECODEC_MODULE ${CEGUI_CORONA_IMAGECODEC_LIBNAME} CACHE STRING "Specifies image codec library to link to samples in static builds." )
elseif (CEGUI_BUILD_IMAGECODEC_SDL2)
    set( CEGUI_OPTION_DEFAULT_IMAGECODEC "SDL2ImageCodec" CACHE STRING "Specifies the ImageCodec module to use as the default" )
    set( CEGUI_STATIC_IMAGECODEC_MODULE ${CEGUI_SDL2_IMAGECODEC_LIBNAME} CACHE STRING "Specifies image codec library to link to samples in static builds." )
elseif (CEGUI_BUILD_IMAGECODEC_TGA)
    set( CEGUI_OPTION_DEFAULT_IMAGECODEC "TGAImageCodec" CACHE STRING "Specifies the ImageCodec module to use as the default" )
    set( CEGUI_STATIC_IMAGECODEC_MODULE ${CEGUI_TGA_IMAGECODEC_LIBNAME} CACHE STRING "Specifies image codec library to link to samples in static builds." )
elseif (CEGUI_BUILD_IMAGECODEC_PVR)
    set( CEGUI_OPTION_DEFAULT_IMAGECODEC "PVRImageCodec" CACHE STRING "Specifies the ImageCodec module to use as the default" )
    set( CEGUI_STATIC_IMAGECODEC_MODULE ${CEGUI_PVR_IMAGECODEC_LIBNAME} CACHE STRING "Specifies image codec library to link to samples in static builds." )
else()
    message(WARNING "None of the image codec modules are going to be built.
You should ensure that CEGUI_OPTION_DEFAULT_IMAGECODEC is set to something
appropriate.")
endif()

set_property(CACHE CEGUI_OPTION_DEFAULT_IMAGECODEC PROPERTY STRINGS "SILLYImageCodec" "DevILImageCodec" "FreeImageImageCodec" "STBImageCodec" "CoronaImageCodec" "SDL2ImageCodec" "TGAImageCodec" "PVRImageCodec")
set_property(CACHE CEGUI_STATIC_IMAGECODEC_MODULE PROPERTY STRINGS "${CEGUI_SILLY_IMAGECODEC_LIBNAME}" "${CEGUI_DEVIL_IMAGECODEC_LIBNAME}" "${CEGUI_FREEIMAGE_IMAGECODEC_LIBNAME}" "${CEGUI_STB_IMAGECODEC_LIBNAME}" "${CEGUI_CORONA_IMAGECODEC_LIBNAME}" "${CEGUI_SDL2_IMAGECODEC_LIBNAME}" "${CEGUI_TGA_IMAGECODEC_LIBNAME}" "${CEGUI_PVR_IMAGECODEC_LIBNAME}")

if(CEGUI_BUILD_DYNAMIC_CONFIGURATION)
    cegui_defaultmodule_sanity_test(CEGUI_OPTION_DEFAULT_IMAGECODEC "SILLYImageCodec" CEGUI_BUILD_IMAGECODEC_SILLY)
    cegui_defaultmodule_sanity_test(CEGUI_OPTION_DEFAULT_IMAGECODEC "DevILImageCodec" CEGUI_BUILD_IMAGECODEC_DEVIL)
    cegui_defaultmodule_sanity_test(CEGUI_OPTION_DEFAULT_IMAGECODEC "FreeImageImageCodec" CEGUI_BUILD_IMAGECODEC_FREEIMAGE)
    cegui_defaultmodule_sanity_test(CEGUI_OPTION_DEFAULT_IMAGECODEC "STBImageCodec" CEGUI_BUILD_IMAGECODEC_STB)
    cegui_defaultmodule_sanity_test(CEGUI_OPTION_DEFAULT_IMAGECODEC "CoronaImageCodec" CEGUI_BUILD_IMAGECODEC_CORONA)
    cegui_defaultmodule_sanity_test(CEGUI_OPTION_DEFAULT_IMAGECODEC "TGAImageCodec" CEGUI_BUILD_IMAGECODEC_TGA)
    cegui_defaultmodule_sanity_test(CEGUI_OPTION_DEFAULT_IMAGECODEC "SDL2ImageCodec" CEGUI_BUILD_IMAGECODEC_SDL2)
endif()

if (CEGUI_BUILD_STATIC_CONFIGURATION)
    cegui_defaultmodule_sanity_test(CEGUI_STATIC_IMAGECODEC_MODULE ${CEGUI_SILLY_IMAGECODEC_LIBNAME} CEGUI_BUILD_IMAGECODEC_SILLY)
    cegui_defaultmodule_sanity_test(CEGUI_STATIC_IMAGECODEC_MODULE ${CEGUI_DEVIL_IMAGECODEC_LIBNAME} CEGUI_BUILD_IMAGECODEC_DEVIL)
    cegui_defaultmodule_sanity_test(CEGUI_STATIC_IMAGECODEC_MODULE ${CEGUI_FREEIMAGE_IMAGECODEC_LIBNAME} CEGUI_BUILD_IMAGECODEC_FREEIMAGE)
    cegui_defaultmodule_sanity_test(CEGUI_STATIC_IMAGECODEC_MODULE ${CEGUI_STB_IMAGECODEC_LIBNAME} CEGUI_BUILD_IMAGECODEC_STB)
    cegui_defaultmodule_sanity_test(CEGUI_STATIC_IMAGECODEC_MODULE ${CEGUI_CORONA_IMAGECODEC_LIBNAME} CEGUI_BUILD_IMAGECODEC_CORONA)
    cegui_defaultmodule_sanity_test(CEGUI_STATIC_IMAGECODEC_MODULE ${CEGUI_TGA_IMAGECODEC_LIBNAME} CEGUI_BUILD_IMAGECODEC_TGA)
    cegui_defaultmodule_sanity_test(CEGUI_STATIC_IMAGECODEC_MODULE ${CEGUI_SDL2_IMAGECODEC_LIBNAME} CEGUI_BUILD_IMAGECODEC_SDL2)
endif()

################################################################################
# Add main header locations (for everything we build)
################################################################################
include_directories(
    ${PROJECT_BINARY_DIR}/cegui/include
    ${PROJECT_SOURCE_DIR}/cegui/include
)

################################################################################
# Add GLM header locations (global dependency, required)
################################################################################
include_directories(
    ${GLM_H_PATH}
)

################################################################################
# Adjust configuration based on option settings
################################################################################
# Windows specific config
if (WIN32)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS -DNOMINMAX)

    if (MSVC)
        cegui_set_msvc_runtime_flags()
    endif()
endif()

# set build output locations
set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib")
set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY})
set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY})
set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY_MINSIZEREL ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY})
set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELWITHDEBINFO ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY})

if (WIN32)
    set (CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin")
else()
    set (CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib")
endif()
set (CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
set (CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
set (CMAKE_LIBRARY_OUTPUT_DIRECTORY_MINSIZEREL ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
set (CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELWITHDEBINFO ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin")
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

# set up install sub-directories
if (NOT CEGUI_INSTALL_LIB_DIR)
  if (CMAKE_SIZEOF_VOID_P EQUAL 8 AND EXISTS "${CMAKE_INSTALL_PREFIX}/lib64")
    set( CEGUI_INSTALL_LIB_DIR lib64 )
  elseif (CMAKE_SIZEOF_VOID_P EQUAL 4 AND EXISTS "${CMAKE_INSTALL_PREFIX}/lib32")
    set( CEGUI_INSTALL_LIB_DIR lib32 )
  else()
    set( CEGUI_INSTALL_LIB_DIR lib )
  endif()
endif()

set( CEGUI_VER_INSTALL_SUBDIR "/${CMAKE_PROJECT_NAME}-${CEGUI_VERSION_MAJOR}" )

if (NOT CEGUI_INSTALL_DATA_DIR)
  set( CEGUI_INSTALL_DATA_DIR "share${CEGUI_VER_INSTALL_SUBDIR}" )
endif()

if (NOT CEGUI_INSTALL_DOC_DIR)
  set( CEGUI_INSTALL_DOC_DIR "share/doc${CEGUI_VER_INSTALL_SUBDIR}" )
endif()

if (NOT CEGUI_INSTALL_PKGCONFIG_DIR)
  set( CEGUI_INSTALL_PKGCONFIG_DIR "${CEGUI_INSTALL_LIB_DIR}/pkgconfig" )
endif()

if (NOT CEGUI_INSTALL_MODULE_DIR)
  set( CEGUI_INSTALL_MODULE_DIR "${CEGUI_INSTALL_LIB_DIR}/${CMAKE_PROJECT_NAME}-${CEGUI_VERSION_MAJOR}.${CEGUI_VERSION_MINOR}" )
endif()

if (NOT CEGUI_INSTALL_SAMPLE_DIR)
  set( CEGUI_INSTALL_SAMPLE_DIR "${CEGUI_INSTALL_MODULE_DIR}" ) # TODO: put into /samples subdir!
endif()

if (NOT CEGUI_INSTALL_INCLUDE_DIR)
  set( CEGUI_INSTALL_INCLUDE_DIR "include${CEGUI_VER_INSTALL_SUBDIR}" )
endif()

################################################################################
# Deal with files we generate from template files
################################################################################
configure_file( cegui/include/CEGUI/Version.h.in cegui/include/CEGUI/Version.h )
configure_file( cegui/include/CEGUI/Config.h.in cegui/include/CEGUI/Config.h )
configure_file( cegui/include/CEGUI/ModuleConfig.h.in cegui/include/CEGUI/ModuleConfig.h )
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/samples")
    configure_file( samples/common/include/CEGUISamplesConfig.h.in samples/common/include/CEGUISamplesConfig.h )
endif()
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/doc")
    configure_file( doc/doxygen/doxyfile.in doc/doxygen/doxyfile )
endif()

################################################################################
# Copy dependency binaries to the target bin folder
################################################################################
if ((WIN32 OR APPLE) AND (CEGUI_BUILD_SAMPLES OR CEGUI_BUILD_APPLICATION_TEMPLATES))
    set(DEPS_BIN_PATH "${CEGUI_DEPENDENCIES_DIR}/bin/${CEGUI_ARCH_SUFFIX}")
    file(GLOB DEPS_BIN_FILES LIST_DIRECTORIES false RELATIVE "${DEPS_BIN_PATH}" "${DEPS_BIN_PATH}/*")
    foreach(_FILE_NAME ${DEPS_BIN_FILES})
        configure_file("${DEPS_BIN_PATH}/${_FILE_NAME}" "${CMAKE_BINARY_DIR}/bin/${_FILE_NAME}" COPYONLY)
    endforeach()
endif()

################################################################################
# Perhaps install pkg-config ("*.pc") files.
################################################################################
cegui_dependent_option( CEGUI_INSTALL_PKGCONFIG "Whether to install pkg-config (\"*.pc\") files." "CMAKE_HOST_UNIX;NOT CMAKE_HOST_APPLE" )
mark_as_advanced( CEGUI_INSTALL_PKGCONFIG )
if( CEGUI_INSTALL_PKGCONFIG )
    configure_file( cegui/CEGUI.pc.in cegui/CEGUI-${CEGUI_VERSION_MAJOR}.pc @ONLY )
    install(FILES ${PROJECT_BINARY_DIR}/cegui/CEGUI-${CEGUI_VERSION_MAJOR}.pc DESTINATION ${CEGUI_INSTALL_PKGCONFIG_DIR} COMPONENT cegui_pkgconfig)

    if (CEGUI_BUILD_RENDERER_NULL)
        configure_file( cegui/CEGUI-NULL.pc.in cegui/CEGUI-${CEGUI_VERSION_MAJOR}-NULL.pc @ONLY )
        install(FILES ${PROJECT_BINARY_DIR}/cegui/CEGUI-${CEGUI_VERSION_MAJOR}-NULL.pc DESTINATION ${CEGUI_INSTALL_PKGCONFIG_DIR} COMPONENT cegui_pkgconfig)
    endif()
    if (CEGUI_BUILD_RENDERER_IRRLICHT)
        configure_file( cegui/CEGUI-IRRLICHT.pc.in cegui/CEGUI-${CEGUI_VERSION_MAJOR}-IRRLICHT.pc @ONLY )
        install(FILES ${PROJECT_BINARY_DIR}/cegui/CEGUI-${CEGUI_VERSION_MAJOR}-IRRLICHT.pc DESTINATION ${CEGUI_INSTALL_PKGCONFIG_DIR} COMPONENT cegui_pkgconfig)
    endif()
    if (CEGUI_BUILD_RENDERER_OGRE)
        configure_file( cegui/CEGUI-OGRE.pc.in cegui/CEGUI-${CEGUI_VERSION_MAJOR}-OGRE.pc @ONLY )
        install(FILES ${PROJECT_BINARY_DIR}/cegui/CEGUI-${CEGUI_VERSION_MAJOR}-OGRE.pc DESTINATION ${CEGUI_INSTALL_PKGCONFIG_DIR} COMPONENT cegui_pkgconfig)
    endif()
    if (CEGUI_BUILD_RENDERER_OPENGL)
        configure_file( cegui/CEGUI-OPENGL.pc.in cegui/CEGUI-${CEGUI_VERSION_MAJOR}-OPENGL.pc @ONLY )
        install(FILES ${PROJECT_BINARY_DIR}/cegui/CEGUI-${CEGUI_VERSION_MAJOR}-OPENGL.pc DESTINATION ${CEGUI_INSTALL_PKGCONFIG_DIR} COMPONENT cegui_pkgconfig)
    endif()
    if (CEGUI_BUILD_RENDERER_OPENGL3)
        configure_file( cegui/CEGUI-OPENGL3.pc.in cegui/CEGUI-${CEGUI_VERSION_MAJOR}-OPENGL3.pc @ONLY )
        install(FILES ${PROJECT_BINARY_DIR}/cegui/CEGUI-${CEGUI_VERSION_MAJOR}-OPENGL3.pc DESTINATION ${CEGUI_INSTALL_PKGCONFIG_DIR} COMPONENT cegui_pkgconfig)
    endif()
    if (CEGUI_BUILD_LUA_MODULE)
        configure_file( cegui/CEGUI-LUA.pc.in cegui/CEGUI-${CEGUI_VERSION_MAJOR}-LUA.pc @ONLY )
        install(FILES ${PROJECT_BINARY_DIR}/cegui/CEGUI-${CEGUI_VERSION_MAJOR}-LUA.pc DESTINATION ${CEGUI_INSTALL_PKGCONFIG_DIR} COMPONENT cegui_pkgconfig)
    endif()
endif()

# Activating the default multi-processor build setting for all Visual Studio versions
if (MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
endif()

################################################################################
# descend into subdirs
################################################################################
add_subdirectory(cegui/src)
add_subdirectory(cegui/src/RendererModules)
add_subdirectory(cegui/src/XMLParserModules)
add_subdirectory(cegui/src/ImageCodecModules)
add_subdirectory(cegui/src/WindowRendererSets)
add_subdirectory(cegui/src/ScriptModules)

if(CEGUI_BUILD_COMMON_DIALOGS)
    add_subdirectory(cegui/src/CommonDialogs)
endif()

if (CEGUI_BUILD_SAMPLES)
    add_subdirectory(samples)
endif()

if (CEGUI_BUILD_APPLICATION_TEMPLATES)
    add_subdirectory(application_templates)
endif()

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/datafiles")
    add_subdirectory(datafiles)
endif()

if (DOXYGEN_FOUND AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/doc")
    add_custom_target(html
        "${DOXYGEN_EXECUTABLE}"
        WORKING_DIRECTORY "${PROJECT_BINARY_DIR}/doc/doxygen"
        COMMENT "Generating documentation" VERBATIM)
endif()

if (CEGUI_BUILD_TESTS OR CEGUI_BUILD_PERFORMANCE_TESTS OR CEGUI_BUILD_DATAFILES_TEST)
    enable_testing()

    add_subdirectory(tests)
endif()

################################################################################
# CPack (mostly for source tarballs)
################################################################################
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Crazy Eddie's Gui System")
set(CPACK_PACKAGE_VENDOR "CEGUI team")
set(CPACK_PACKAGE_DESCRIPTION_FILE "${PROJECT_SOURCE_DIR}/README.md")
set(CPACK_RESOURCE_FILE_LICENSE "${PROJECT_SOURCE_DIR}/COPYING")
set(CPACK_PACKAGE_VERSION_MAJOR "${CEGUI_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${CEGUI_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${CEGUI_VERSION_PATCH}")

set(CPACK_SOURCE_GENERATOR TBZ2 ZIP)

set(CPACK_SOURCE_PACKAGE_FILE_NAME "cegui-${CEGUI_VERSION}" CACHE INTERNAL "tarball basename")

set(CPACK_SOURCE_IGNORE_FILES
# repository history should not be in source tarballs
"\\\\.hg.*"
# most likely not needed in the tarball
"cppcheck-output"
"perform-cppcheck"

# the rest is modelled after .hgignore
"build/"
"~$"
"\\\\.orig$"

"Thumbs.db"
"\\\\.directory"

"\\\\.kdev4"
"\\\\.settings"
"\\\\.project"
"\\\\.cproject"
"\\\\.pydevproject"
"\\\\.ropeproject"
)

include(CPack)
