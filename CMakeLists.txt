################################################################################
# Top-level CMakeLists.txt file for Crazy Eddie's GUI System
################################################################################
cmake_minimum_required(VERSION 2.8.11)
if (POLICY CMP0017)
    cmake_policy(SET CMP0017 OLD)
endif()

if (POLICY CMP0045)
    cmake_policy(SET CMP0045 OLD)
endif()

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

set (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake")
include(CEGUIMacros)
include(CMakeDependentOption)

################################################################################
# Set up version information
################################################################################
set( CEGUI_VERSION_MAJOR 0)
set( CEGUI_VERSION_MINOR 8)
set( CEGUI_VERSION_PATCH 7)
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


################################################################################
# Check for libs and other packages we might use.
################################################################################
# be sure to search the provided dependencies on Win32 and Mac OS X platforms
if ((WIN32 OR APPLE) AND NOT CMAKE_PREFIX_PATH)
    set (CMAKE_PREFIX_PATH "${PROJECT_SOURCE_DIR}/dependencies" CACHE PATH "Path to dependencies")
    # prefer the dylib deps in the dependencies over some other framework
    set (CMAKE_FIND_FRAMEWORK LAST)
endif()

if ((CMAKE_CXX_COMPILER_ID STREQUAL "GNU") OR (CMAKE_CXX_COMPILER_ID STREQUAL "Clang"))
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++03")
elseif (NOT CMAKE_VERSION VERSION_LESS "3.1")
    set (CMAKE_C_STANDARD_REQUIRED TRUE)
    set (CMAKE_C_STANDARD "99")
endif ()

# Look for packages
find_package(PCRE)
find_package(Freetype)
find_package(Minizip)
find_package(Fribidi)
if (NOT WIN32)
    find_package(Iconv REQUIRED)
endif()

find_package(OpenGL)
find_package(GLEW)
find_package(GLM)
find_package(GLFW)
find_package(GLFW3)
find_package(SDL2)
find_package(SDL2_image)
find_package(DirectXSDK)
find_package(D3DX11Effects)
find_package(Irrlicht)
find_package(Ogre)
find_package(OIS)
find_package(DirectFB)
find_package(OpenGLES)
find_package(Epoxy)

find_package(EXPAT)
find_package(XercesC)
find_package(LibXml2)
find_package(TinyXML)
find_package(RapidXML)

find_package(DevIL)
find_package(FreeImage)
find_package(SILLY)
find_package(Corona)
find_package(PVRTools)

find_package(Lua51)
find_package(TOLUAPP)
find_package(PythonInterp)

## ensure python3 not picked up as default
find_package(PythonVersion)
if (UNIX AND PYTHON_VERSION_FOUND)
  find_package(PythonLibs ${PYTHON_VERSION_STRING} EXACT)
else()
  find_package(PythonLibs)
endif()
find_package(Boost 1.36.0 COMPONENTS python unit_test_framework system timer)

find_package(Doxygen)
find_package(GTK2 COMPONENTS gtk)
# set for consistency with other deps
set (GTK2_INCLUDE_DIR ${GTK2_INCLUDE_DIRS})

## Search for ccache
find_program(CCACHE_FOUND ccache)
if(CCACHE_FOUND)
    set(ENV(CCACHE_CPP2) "yes")
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK ccache)
endif(CCACHE_FOUND)

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
option( CEGUI_BUILD_STATIC_CONFIGURATION "Specifies whether the static configs will be built.  (NB: Shared configs are always built)" FALSE)
option( CEGUI_BUILD_STATIC_FACTORY_MODULE "For static CEGUI builds, specifies whether to include the getWindoFactoryModule method.  When false, the developer must provide a custom implementation of the method" FALSE)
# sanity check on static build option for factory method
if (CEGUI_BUILD_STATIC_FACTORY_MODULE AND NOT CEGUI_BUILD_STATIC_CONFIGURATION)
  message(SEND_ERROR "You may not enable CEGUI_BUILD_STATIC_FACTORY_MODULE without CEGUI_BUILD_STATIC_CONFIGURATION enabled.  CEGUI needs to be built statically if building the static factory method.")
endif()
option( CEGUI_CUSTOM_ALLOCATORS "If checked, CEGUI can use custom allocation. (for experienced developers only!)" FALSE )
cmake_dependent_option( CEGUI_CUSTOM_ALLOCATORS_DEBUG "If checked, CEGUI allocations will pass debug info to allocators. (only used if CEGUI_CUSTOM_ALLOCATORS is checked)" FALSE "CEGUI_CUSTOM_ALLOCATORS" FALSE )
set( CEGUI_CUSTOM_ALLOCATORS_INCLUDE "CEGUI/MemoryStdAllocator.h" CACHE STRING "Which include file contains memory allocators and allocation configuration. (only used if CEGUI_CUSTOM_ALLOCATORS is checked)
We have bundled 2 allocators, mostly for demonstrational purposes.
CEGUI/MemoryStdAllocator.h contains malloc allocator, use it as a starting point for your own allocators
CEGUI/MemoryOgreAllocator.h delegates allocation to Ogre, which may be useful to Ogre users (you will have to manually change linking options of CEGUIBase!)" )

set( CEGUI_STRING_CLASS 1 CACHE INT "Which string class should CEGUI use
1 - utf8 and utf32 aware inbuilt string,
2 - std::string without custom allocation,
3 - std::basic_string allocated according to allocator config" )

# sanity check std::basic_string and allocator and allocators actually being used
if (${CEGUI_STRING_CLASS} MATCHES 3 AND NOT CEGUI_CUSTOM_ALLOCATORS)
    message(SEND_ERROR "You requested std::basic_string allocated according to allocator as CEGUI::String even though CEGUI is not being built with custom allocators enabled!")
endif()

option( CEGUI_HAS_FREETYPE "Specifies whether to include Freetype font support" ${FREETYPE_FOUND} )
option( CEGUI_HAS_PCRE_REGEX "Specifies whether to include PCRE regexp matching for editbox string validation" ${PCRE_FOUND} )
option( CEGUI_HAS_MINIZIP_RESOURCE_PROVIDER "Specifies whether to build the minizip based resource provider" ${MINIZIP_FOUND} )
option( CEGUI_HAS_DEFAULT_LOGGER "Specifies whether to build the DefaultLogger implementation" TRUE)

option( CEGUI_BUILD_XMLPARSER_EXPAT "Specifies whether to build the Expat based XMLParser module" ${EXPAT_FOUND} )
option( CEGUI_BUILD_XMLPARSER_XERCES "Specifies whether to build the Xerces-C++ based XMLParser module" ${XERCESC_FOUND} )
option( CEGUI_BUILD_XMLPARSER_LIBXML2 "Specifies whether to build the libxml2 based XMLParser module" ${LIBXML2_FOUND} )
option( CEGUI_BUILD_XMLPARSER_RAPIDXML "Specifies whether to build the RapidXML based XMLParser module" ${RAPIDXML_FOUND} )
option( CEGUI_BUILD_XMLPARSER_TINYXML "Specifies whether to build the TinyXML based XMLParser module" ${TINYXML_FOUND} )

option( CEGUI_BUILD_IMAGECODEC_SILLY "Specifies whether to build the SILLY based ImageCodec module" ${SILLY_FOUND} )
option( CEGUI_BUILD_IMAGECODEC_DEVIL "Specifies whether to build the DevIL based ImageCodec module" ${IL_FOUND} )
option( CEGUI_BUILD_IMAGECODEC_FREEIMAGE "Specifies whether to build the FreeImage based ImageCodec module" ${FREEIMAGE_FOUND} )
option( CEGUI_BUILD_IMAGECODEC_CORONA "Specifies whether to build the Corona based ImageCodec module" ${CORONA_FOUND} )
option( CEGUI_BUILD_IMAGECODEC_STB "Specifies whether to build the STB based ImageCodec module" FALSE )
option( CEGUI_BUILD_IMAGECODEC_TGA "Specifies whether to build the based TGA only ImageCodec module" FALSE )
option( CEGUI_BUILD_IMAGECODEC_PVR "Specifies whether to build the PVR only ImageCodec module" ${PVRTOOLS_FOUND} )
cegui_dependent_option( CEGUI_BUILD_IMAGECODEC_SDL2 "Specifies whether to build the SDL2 ImageCodec module" "SDL2_FOUND;SDL2IMAGE_FOUND" )

cegui_dependent_option( CEGUI_BUILD_RENDERER_OPENGL "Specifies whether to build the old OpenGL renderer module" "OPENGL_gl_LIBRARY;GLM_FOUND;GLEW_FOUND" )
cegui_dependent_option( CEGUI_BUILD_RENDERER_OPENGL3 "Specifies whether to build the renderer module that supports OpenGL 3.2 (core profile) and OpenGL ES 2.0." "OPENGL_gl_LIBRARY;GLM_FOUND;OPENGL_LOADER_FOUND")
option( CEGUI_BUILD_RENDERER_OGRE "Specifies whether to build the Ogre renderer module" ${OGRE_FOUND} )
option( CEGUI_BUILD_RENDERER_IRRLICHT "Specifies whether to build the Irrlicht renderer module" ${IRRLICHT_FOUND} )
option( CEGUI_BUILD_RENDERER_DIRECTFB "Specifies whether to build the DirectFB renderer module (not supported!)" FALSE )
cegui_dependent_option( CEGUI_BUILD_RENDERER_DIRECT3D9 "Specifies whether to build the Direct3D 9 renderer module" "DIRECTXSDK_FOUND;NOT DIRECTXSDK_MAX_D3D LESS 9" )
cegui_dependent_option( CEGUI_BUILD_RENDERER_DIRECT3D10 "Specifies whether to build the Direct3D 10 renderer module" "DIRECTXSDK_FOUND;NOT DIRECTXSDK_MAX_D3D LESS 10" )
cegui_dependent_option( CEGUI_BUILD_RENDERER_DIRECT3D11 "Specifies whether to build the Direct3D 11 renderer module" "DIRECTXSDK_FOUND;D3DX11EFFECTS_FOUND;NOT DIRECTXSDK_MAX_D3D LESS 11" )
option( CEGUI_BUILD_RENDERER_NULL "Specifies whether to build the null renderer module" FALSE )
option( CEGUI_BUILD_RENDERER_OPENGLES "Specifies whether to build the OpenGL ES 1 renderer module" ${OPENGLES_FOUND} )
cegui_dependent_option( CEGUI_BUILD_LUA_MODULE "Specifies whether to build the Lua based script module" "LUA51_FOUND;TOLUAPP_FOUND" )
cegui_dependent_option( CEGUI_BUILD_LUA_GENERATOR "Specifies whether to build the custom tolua binding generator 'toluappcegui'" "LUA51_FOUND;TOLUAPP_FOUND" )

cegui_dependent_option( CEGUI_BUILD_PYTHON_MODULES "Specifies whether to build the Python extension module(s)" "PYTHONINTERP_FOUND;PYTHONLIBS_FOUND;Boost_PYTHON_FOUND" )

option( CEGUI_OPTION_SAFE_LUA_MODULE "Specifies whether to enable extra validation in the Lua script module in non-debug builds" FALSE )

cegui_dependent_option( CEGUI_USE_FRIBIDI "Specifies whether bi-directional text will be supported via the fribidi library." "FRIBIDI_FOUND" )
cegui_dependent_option( CEGUI_USE_MINIBIDI "Specifies whether bi-directional text will be supported via the embedded minibidi." FALSE )

# sanity check on bidi options
if (CEGUI_USE_MINIBIDI AND CEGUI_USE_FRIBIDI)
    message(SEND_ERROR "You may not enable both CEGUI_USE_FRIBIDI and CEGUI_USE_MINIBIDI. Please choose only one of these options.")
endif()

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
        message (FATAL_ERROR "Building the old OpenGL renderer module is only supported using GLEW.")
    endif ()
    if (NOT ANDROID)
        set (CEGUI_GLFW_VER "" CACHE STRING "Specifies which major version of GLFW to use for the samples (2 or 3). The default is to look for both, giving priority to version 2.")

        if (((CEGUI_GLFW_VER STREQUAL "2") OR (CEGUI_GLFW_VER STREQUAL "")) AND GLFW_FOUND)
            set (CEGUI_USE_GLFW_VER "2")
        elseif (((CEGUI_GLFW_VER STREQUAL "3")  OR (CEGUI_GLFW_VER STREQUAL "")) AND GLFW3_FOUND)
            set (CEGUI_USE_GLFW_VER "3")
        endif ()
    endif ()
endif ()

if (CEGUI_BUILD_RENDERER_OPENGL3 AND CEGUI_USE_EPOXY)
    set (CEGUI_SUPPORT_RENDERER_OPENGLES2 TRUE)
    set (CEGUI_OPENGL_VER_MAJOR_FORCE "" CACHE STRING "Run the samples as if a specific OpenGL (desktop or ES) major version is used. Useful for testing.")
    mark_as_advanced (CEGUI_OPENGL_VER_MAJOR_FORCE)
    set (CEGUI_OPENGL_VER_MINOR_FORCE "0" CACHE STRING "Run the samples as if a specific OpenGL (desktop or ES) minor version is used. Useful for testing.")
    mark_as_advanced (CEGUI_OPENGL_VER_MINOR_FORCE)
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

if ((CMAKE_CXX_COMPILER_ID STREQUAL "GNU") OR (CMAKE_CXX_COMPILER_ID STREQUAL "Clang"))
    set (CEGUI_EXTRA_FLAGS "${CEGUI_EXTRA_FLAGS} -fsigned-char")
endif ()

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set (CEGUI_EXTRA_FLAGS "${CEGUI_EXTRA_FLAGS} -Wall -Wextra")
endif ()

if (CEGUI_WARNINGS_AS_ERRORS)
    if ((CMAKE_CXX_COMPILER_ID STREQUAL "GNU") OR (CMAKE_CXX_COMPILER_ID STREQUAL "Clang"))
        set (CEGUI_EXTRA_FLAGS "${CEGUI_EXTRA_FLAGS} -Werror")
    elseif (MSVC)
        set (CEGUI_EXTRA_FLAGS "${CEGUI_EXTRA_FLAGS} /WX")
    else ()
        message (WARNING "You've specified \"CEGUI_WARNINGS_AS_ERRORS\" as \"ON\", but we don't know how to make it for your compiler. If you wish you can specifiy the required flags manually.")
    endif ()
endif ()

set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${CEGUI_EXTRA_FLAGS}")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CEGUI_EXTRA_FLAGS}")
set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${CEGUI_EXTRA_FLAGS}")
set (CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} ${CEGUI_EXTRA_FLAGS}")
set (CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${CEGUI_EXTRA_FLAGS}")

# Here we check if Ogre is built to use boost, and if yes, if the necessary dependencies are available and found by CEGUI
set (CEGUI_OGRE_DEPS_AVAILABLE FALSE)
set (CEGUI_OGRE_SAMPLES_EXTRALIBS)
if (CEGUI_BUILD_RENDERER_OGRE AND OIS_FOUND)
    if (NOT Boost_VERSION LESS 105000 AND "${OGRE_THREAD_PROVIDER}" STREQUAL "1") #Check additionally if we use boost as thread provider at all
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
set (CEGUI_SAMPLES_RENDERER_DIRECT3D9_ACTIVE ${CEGUI_BUILD_RENDERER_DIRECT3D9})
set (CEGUI_SAMPLES_RENDERER_DIRECT3D10_ACTIVE ${CEGUI_BUILD_RENDERER_DIRECT3D10})
set (CEGUI_SAMPLES_RENDERER_DIRECT3D11_ACTIVE ${CEGUI_BUILD_RENDERER_DIRECT3D11})
set (CEGUI_SAMPLES_RENDERER_DIRECTFB_ACTIVE ${CEGUI_BUILD_RENDERER_DIRECTFB})

# Ogre and OpenGL have dependencies for the sample browser, if these are not available then the SampleBrowser won't support the respective Renderers and the option to choose them won't show up
if (CEGUI_BUILD_RENDERER_OGRE AND NOT CEGUI_OGRE_DEPS_AVAILABLE)
    if (NOT OIS_FOUND)
        message(STATUS "Info: The OgreRenderer is disabled in the SampleBrowser because of missing dependencies (OIS not found). Please add the dependencies and run 'Configure' in CMake again, in case you want to run the SampleBrowser using OgreRenderer.")
    else()
        message(STATUS "Info: The OgreRenderer is disabled in the SampleBrowser because of missing dependencies (boost libraries). Please add the dependencies and run 'Configure' in CMake again, in case you want to run the SampleBrowser using OgreRenderer.")
    endif()
    set (CEGUI_SAMPLES_RENDERER_OGRE_ACTIVE FALSE)
endif()

if (CEGUI_SAMPLES_RENDERER_OPENGL_ACTIVE AND NOT (CEGUI_USE_GLFW_VER OR ANDROID))
    set (CEGUI_SAMPLES_RENDERER_OPENGL_ACTIVE FALSE)
    message(STATUS "Info: The OpenGL-, OpenGL3 and/or OpenGL ES 2 Renderers are disabled in the SampleBrowser because of missing dependencies (GLFW 2/3 library).  Please add the GFLW dependency and run 'Configure' in CMake again, in case you want to run the SampleBrowser using the OpenGL, OpenGL3 or OpenGL ES 2 Renderers.")
endif()

# If we use a Renderer then our samples should be ON by default
if (CEGUI_SAMPLES_RENDERER_OPENGL_ACTIVE OR CEGUI_SAMPLES_RENDERER_OGRE_ACTIVE OR CEGUI_SAMPLES_RENDERER_IRRLICHT_ACTIVE OR CEGUI_SAMPLES_RENDERER_DIRECT3D9_ACTIVE OR CEGUI_SAMPLES_RENDERER_DIRECT3D10_ACTIVE OR CEGUI_SAMPLES_RENDERER_DIRECT3D11_ACTIVE OR CEGUI_SAMPLES_RENDERER_DIRECTFB_ACTIVE)
    set (_CEGUI_SAMPLES_ENABLED_DEFAULT TRUE)
else()
    set (_CEGUI_SAMPLES_ENABLED_DEFAULT FALSE)
endif()

# We set the Samples to the default value
option( CEGUI_SAMPLES_ENABLED "Specifies whether to build the CEGUI sample applications" ${_CEGUI_SAMPLES_ENABLED_DEFAULT} )

if (UNIX AND NOT APPLE AND NOT WIN32)
    option( CEGUI_SAMPLES_USE_GTK2 "Specifies whether the sample applications will make use of the GTK2 UI for renderer selection." FALSE )
endif()

# Option wether to build application templates; default off
option(CEGUI_BUILD_APPLICATION_TEMPLATES "Specifies whether to build the application templates." TRUE)

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

################################################################################
# Define vars holding the names for all the libs we can build for CEGUI.
################################################################################
# Core lib name
cegui_set_library_name( CEGUI_BASE_LIBNAME CEGUIBase )

# Renderer module names.
cegui_set_library_name( CEGUI_OPENGL_RENDERER_LIBNAME CEGUIOpenGLRenderer )
cegui_set_library_name( CEGUI_OGRE_RENDERER_LIBNAME CEGUIOgreRenderer)
cegui_set_library_name( CEGUI_IRRLICHT_RENDERER_LIBNAME CEGUIIrrlichtRenderer )
cegui_set_library_name( CEGUI_DIRECT3D9_RENDERER_LIBNAME CEGUIDirect3D9Renderer )
cegui_set_library_name( CEGUI_DIRECT3D10_RENDERER_LIBNAME CEGUIDirect3D10Renderer )
cegui_set_library_name( CEGUI_DIRECT3D11_RENDERER_LIBNAME CEGUIDirect3D11Renderer )
cegui_set_library_name( CEGUI_NULL_RENDERER_LIBNAME CEGUINullRenderer )
cegui_set_library_name( CEGUI_OPENGLES_RENDERER_LIBNAME CEGUIOpenGLESRenderer )
cegui_set_library_name( CEGUI_DIRECTFB_RENDERER_LIBNAME CEGUIDirectFBRenderer )

# XML parser module names
cegui_set_module_name( CEGUI_EXPAT_PARSER_LIBNAME CEGUIExpatParser )
cegui_set_module_name( CEGUI_TINYXML_PARSER_LIBNAME CEGUITinyXMLParser )
cegui_set_module_name( CEGUI_XERCES_PARSER_LIBNAME CEGUIXercesParser )
cegui_set_module_name( CEGUI_RAPIDXML_PARSER_LIBNAME CEGUIRapidXMLParser )
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
cegui_set_executable_name( CEGUI_SAMPLEFRAMEWORK_EXENAME CEGUISampleFramework )

# Additional lib names
cegui_set_library_name( CEGUI_COMMON_DIALOGS_LIBNAME CEGUICommonDialogs )

################################################################################
# Select one of the XML parser modules to be the default, warning if none are
# available.
################################################################################
if (CEGUI_BUILD_XMLPARSER_EXPAT)
    set( CEGUI_OPTION_DEFAULT_XMLPARSER "ExpatParser" CACHE STRING "Specifies the XMLParser module to use as the default" )
    set( CEGUI_STATIC_XMLPARSER_MODULE ${CEGUI_EXPAT_PARSER_LIBNAME} CACHE STRING "Specifies xml parser library to link to samples in static builds." )
elseif (CEGUI_BUILD_XMLPARSER_XERCES)
    set( CEGUI_OPTION_DEFAULT_XMLPARSER "XercesParser" CACHE STRING "Specifies the XMLParser module to use as the default" )
    set( CEGUI_STATIC_XMLPARSER_MODULE ${CEGUI_XERCES_PARSER_LIBNAME} CACHE STRING "Specifies xml parser library to link to samples in static builds." )
elseif (CEGUI_BUILD_XMLPARSER_LIBXML2)
    set( CEGUI_OPTION_DEFAULT_XMLPARSER "LibXMLParser" CACHE STRING "Specifies the XMLParser module to use as the default" )
    set( CEGUI_STATIC_XMLPARSER_MODULE ${CEGUI_LIBXML2_PARSER_LIBNAME} CACHE STRING "Specifies xml parser library to link to samples in static builds." )
elseif (CEGUI_BUILD_XMLPARSER_RAPIDXML)
    set( CEGUI_OPTION_DEFAULT_XMLPARSER "RapidXMLParser" CACHE STRING "Specifies the XMLParser module to use as the default" )
    set( CEGUI_STATIC_XMLPARSER_MODULE ${CEGUI_RAPIDXML_PARSER_LIBNAME} CACHE STRING "Specifies xml parser library to link to samples in static builds." )
elseif (CEGUI_BUILD_XMLPARSER_TINYXML)
    set( CEGUI_OPTION_DEFAULT_XMLPARSER "TinyXMLParser" CACHE STRING "Specifies the XMLParser module to use as the default" )
    set( CEGUI_STATIC_XMLPARSER_MODULE ${CEGUI_TINYXML_PARSER_LIBNAME} CACHE STRING "Specifies xml parser library to link to samples in static builds." )
else()
    message(WARNING "None of the XML parser modules are going to be built.
You should ensure that CEGUI_OPTION_DEFAULT_XMLPARSER is set to something
appropriate.")
endif()

cegui_defaultmodule_sanity_test(CEGUI_OPTION_DEFAULT_XMLPARSER "ExpatParser" CEGUI_BUILD_XMLPARSER_EXPAT)
cegui_defaultmodule_sanity_test(CEGUI_OPTION_DEFAULT_XMLPARSER "XercesParser" CEGUI_BUILD_XMLPARSER_XERCES)
cegui_defaultmodule_sanity_test(CEGUI_OPTION_DEFAULT_XMLPARSER "LibXMLParser" CEGUI_BUILD_XMLPARSER_LIBXML2)
cegui_defaultmodule_sanity_test(CEGUI_OPTION_DEFAULT_XMLPARSER "RapidXMLParser" CEGUI_BUILD_XMLPARSER_RAPIDXML)
cegui_defaultmodule_sanity_test(CEGUI_OPTION_DEFAULT_XMLPARSER "TinyXMLParser" CEGUI_BUILD_XMLPARSER_TINYXML)

if (CEGUI_BUILD_STATIC_CONFIGURATION)
    cegui_defaultmodule_sanity_test(CEGUI_STATIC_XMLPARSER_MODULE ${CEGUI_EXPAT_PARSER_LIBNAME} CEGUI_BUILD_XMLPARSER_EXPAT)
    cegui_defaultmodule_sanity_test(CEGUI_STATIC_XMLPARSER_MODULE ${CEGUI_XERCES_PARSER_LIBNAME} CEGUI_BUILD_XMLPARSER_XERCES)
    cegui_defaultmodule_sanity_test(CEGUI_STATIC_XMLPARSER_MODULE ${CEGUI_LIBXML2_PARSER_LIBNAME} CEGUI_BUILD_XMLPARSER_LIBXML2)
    cegui_defaultmodule_sanity_test(CEGUI_STATIC_XMLPARSER_MODULE ${CEGUI_RAPIDXML_PARSER_LIBNAME} CEGUI_BUILD_XMLPARSER_RAPIDXML)
    cegui_defaultmodule_sanity_test(CEGUI_STATIC_XMLPARSER_MODULE ${CEGUI_TINYXML_PARSER_LIBNAME} CEGUI_BUILD_XMLPARSER_TINYXML)
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
else()
message(WARNING "None of the image codec modules are going to be built.
You should ensure that CEGUI_OPTION_DEFAULT_IMAGECODEC is set to something
appropriate.")
endif()

cegui_defaultmodule_sanity_test(CEGUI_OPTION_DEFAULT_IMAGECODEC "SILLYImageCodec" CEGUI_BUILD_IMAGECODEC_SILLY)
cegui_defaultmodule_sanity_test(CEGUI_OPTION_DEFAULT_IMAGECODEC "DevILImageCodec" CEGUI_BUILD_IMAGECODEC_DEVIL)
cegui_defaultmodule_sanity_test(CEGUI_OPTION_DEFAULT_IMAGECODEC "FreeImageImageCodec" CEGUI_BUILD_IMAGECODEC_FREEIMAGE)
cegui_defaultmodule_sanity_test(CEGUI_OPTION_DEFAULT_IMAGECODEC "STBImageCodec" CEGUI_BUILD_IMAGECODEC_STB)
cegui_defaultmodule_sanity_test(CEGUI_OPTION_DEFAULT_IMAGECODEC "CoronaImageCodec" CEGUI_BUILD_IMAGECODEC_CORONA)
cegui_defaultmodule_sanity_test(CEGUI_OPTION_DEFAULT_IMAGECODEC "TGAImageCodec" CEGUI_BUILD_IMAGECODEC_TGA)
cegui_defaultmodule_sanity_test(CEGUI_OPTION_DEFAULT_IMAGECODEC "SDL2ImageCodec" CEGUI_BUILD_IMAGECODEC_SDL2)

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
if (CMAKE_SIZEOF_VOID_P EQUAL 8 AND EXISTS "${CMAKE_INSTALL_PREFIX}/lib64")
    set( CEGUI_LIB_INSTALL_DIR lib64 )
elseif (CMAKE_SIZEOF_VOID_P EQUAL 4 AND EXISTS "${CMAKE_INSTALL_PREFIX}/lib32")
    set( CEGUI_LIB_INSTALL_DIR lib32 )
else()
    set( CEGUI_LIB_INSTALL_DIR lib )
endif()

set( CEGUI_VER_INSTALL_SUBDIR "/${CMAKE_PROJECT_NAME}-${CEGUI_VERSION_MAJOR}" )
set( CEGUI_DATA_INSTALL_DIR "share${CEGUI_VER_INSTALL_SUBDIR}" )
set( CEGUI_DOC_INSTALL_DIR "share/doc${CEGUI_VER_INSTALL_SUBDIR}" )

set( CEGUI_PKGCONFIG_INSTALL_DIR "${CEGUI_LIB_INSTALL_DIR}/pkgconfig" )
set( CEGUI_MODULE_INSTALL_DIR "${CEGUI_LIB_INSTALL_DIR}/${CMAKE_PROJECT_NAME}-${CEGUI_VERSION_MAJOR}.${CEGUI_VERSION_MINOR}" )
set( CEGUI_SAMPLE_INSTALL_DIR "${CEGUI_MODULE_INSTALL_DIR}" ) # TODO: put into /samples subdir!
set( CEGUI_INCLUDE_INSTALL_DIR "include${CEGUI_VER_INSTALL_SUBDIR}" )

################################################################################
# Deal with files we generate from template files
################################################################################
configure_file( cegui/include/CEGUI/Version.h.in cegui/include/CEGUI/Version.h )
configure_file( cegui/include/CEGUI/Config.h.in cegui/include/CEGUI/Config.h )
configure_file( cegui/include/CEGUI/ModuleConfig.h.in cegui/include/CEGUI/ModuleConfig.h )
configure_file( samples/common/include/CEGUISamplesConfig.h.in samples/common/include/CEGUISamplesConfig.h )
configure_file( doc/doxygen/doxyfile.in doc/doxygen/doxyfile )

if (UNIX AND NOT APPLE)
    configure_file( cegui/CEGUI.pc.in cegui/CEGUI-${CEGUI_VERSION_MAJOR}.pc @ONLY )
    install(FILES ${PROJECT_BINARY_DIR}/cegui/CEGUI-${CEGUI_VERSION_MAJOR}.pc DESTINATION ${CEGUI_PKGCONFIG_INSTALL_DIR})

    if (CEGUI_BUILD_RENDERER_NULL)
        configure_file( cegui/CEGUI-NULL.pc.in cegui/CEGUI-${CEGUI_VERSION_MAJOR}-NULL.pc @ONLY )
        install(FILES ${PROJECT_BINARY_DIR}/cegui/CEGUI-${CEGUI_VERSION_MAJOR}-NULL.pc DESTINATION ${CEGUI_PKGCONFIG_INSTALL_DIR})
    endif()
    if (CEGUI_BUILD_RENDERER_IRRLICHT)
        configure_file( cegui/CEGUI-IRRLICHT.pc.in cegui/CEGUI-${CEGUI_VERSION_MAJOR}-IRRLICHT.pc @ONLY )
        install(FILES ${PROJECT_BINARY_DIR}/cegui/CEGUI-${CEGUI_VERSION_MAJOR}-IRRLICHT.pc DESTINATION ${CEGUI_PKGCONFIG_INSTALL_DIR})
    endif()
    if (CEGUI_BUILD_RENDERER_OGRE)
        configure_file( cegui/CEGUI-OGRE.pc.in cegui/CEGUI-${CEGUI_VERSION_MAJOR}-OGRE.pc @ONLY )
        install(FILES ${PROJECT_BINARY_DIR}/cegui/CEGUI-${CEGUI_VERSION_MAJOR}-OGRE.pc DESTINATION ${CEGUI_PKGCONFIG_INSTALL_DIR})
    endif()
    if (CEGUI_BUILD_RENDERER_OPENGL)
        configure_file( cegui/CEGUI-OPENGL.pc.in cegui/CEGUI-${CEGUI_VERSION_MAJOR}-OPENGL.pc @ONLY )
        install(FILES ${PROJECT_BINARY_DIR}/cegui/CEGUI-${CEGUI_VERSION_MAJOR}-OPENGL.pc DESTINATION ${CEGUI_PKGCONFIG_INSTALL_DIR})
    endif()
    if (CEGUI_BUILD_RENDERER_OPENGL3)
        configure_file( cegui/CEGUI-OPENGL3.pc.in cegui/CEGUI-${CEGUI_VERSION_MAJOR}-OPENGL3.pc @ONLY )
        install(FILES ${PROJECT_BINARY_DIR}/cegui/CEGUI-${CEGUI_VERSION_MAJOR}-OPENGL3.pc DESTINATION ${CEGUI_PKGCONFIG_INSTALL_DIR})
    endif()
    if (CEGUI_BUILD_LUA_MODULE)
        configure_file( cegui/CEGUI-LUA.pc.in cegui/CEGUI-${CEGUI_VERSION_MAJOR}-LUA.pc @ONLY )
        install(FILES ${PROJECT_BINARY_DIR}/cegui/CEGUI-${CEGUI_VERSION_MAJOR}-LUA.pc DESTINATION ${CEGUI_PKGCONFIG_INSTALL_DIR})
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

add_subdirectory(cegui/src/CommonDialogs)

if (CEGUI_SAMPLES_ENABLED)
    add_subdirectory(samples_framework)
    add_subdirectory(samples)
endif()

if (CEGUI_BUILD_APPLICATION_TEMPLATES)
    add_subdirectory(application_templates)
endif()

add_subdirectory(datafiles)

if (DOXYGEN_FOUND)
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
