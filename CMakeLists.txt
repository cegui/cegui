################################################################################
# Top-level CMakeLists.txt file for Crazy Eddie's GUI System
################################################################################
cmake_minimum_required(VERSION 2.8)
if (POLICY CMP0017)
    cmake_policy(SET CMP0017 OLD)
endif()

set (CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
include(CEGUIMacros)
include(CMakeDependentOption)

if (APPLE)
    set( CMAKE_OSX_ARCHITECTURES i386;x86_64 CACHE STRING "Build architectures for Mac OS X")
endif()

# default to Release build (it's what most people will use, except cegui devs)
if (UNIX)
    set( CMAKE_BUILD_TYPE Release CACHE STRING "Sets the configuration to build (Release, Debug, etc...)")
endif()

################################################################################
# Start of main Project definitions
################################################################################
project(CEGUI)

################################################################################
# Set up version information
################################################################################
set( CEGUI_VERSION_MAJOR 0)
set( CEGUI_VERSION_MINOR 8)
set( CEGUI_VERSION_PATCH 9090)
set( CEGUI_VERSION ${CEGUI_VERSION_MAJOR}.${CEGUI_VERSION_MINOR}.${CEGUI_VERSION_PATCH} )

set( CEGUI_ABI_CURRENT 2)
set( CEGUI_ABI_REVISION 0)
set( CEGUI_ABI_AGE 0)
set( CEGUI_ABI_VERSION ${CEGUI_ABI_CURRENT}.${CEGUI_ABI_REVISION}.${CEGUI_ABI_AGE} )


################################################################################
# Check for libs and other packages we might use.
################################################################################
# be sure to search the provided dependencies on Win32 and Mac OS X platforms
if (WIN32 OR APPLE)
    set (CMAKE_PREFIX_PATH "${CMAKE_SOURCE_DIR}/dependencies" CACHE PATH "Path to dependencies")
    # prefer the dylib deps in the dependencies over some other framework
    set (CMAKE_FIND_FRAMEWORK LAST)
endif()

# Look for packages
find_package(PCRE)
find_package(Freetype)
find_package(Minizip)

find_package(OpenGL)
find_package(GLEW)
find_package(GLM)
find_package(GLFW)
find_package(DirectXSDK)
find_package(D3DX11Effects)
find_package(Irrlicht)
find_package(Ogre)
find_package(OIS)
find_package(DirectFB)
find_package(OpenGLES)

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
find_package(PythonLibs)
find_package(Boost 1.36.0 COMPONENTS python)

find_package(Doxygen)
find_package(GTK2 COMPONENTS gtk)
# set for consistency with other deps
set (GTK2_INCLUDE_DIR ${GTK2_INCLUDE_DIRS})


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
option( CEGUI_CUSTOM_ALLOCATORS "If checked, CEGUI can use custom allocation. (for experienced developers only!)" FALSE )
cmake_dependent_option( CEGUI_CUSTOM_ALLOCATORS_DEBUG "If checked, CEGUI allocations will pass debug info to allocators. (only used if CEGUI_CUSTOM_ALLOCATORS is checked)" FALSE "CEGUI_CUSTOM_ALLOCATORS" FALSE )
set( CEGUI_CUSTOM_ALLOCATORS_INCLUDE "CEGUIMemoryStdAllocator.h" CACHE STRING "Which include file contains memory allocators and allocation configuration. (only used if CEGUI_CUSTOM_ALLOCATORS is checked)
We have bundled 2 allocators, mostly for demonstrational purposes.
CEGUIMemoryStdAllocator.h contains malloc allocator, use it as a starting point for your own allocators
CEGUIMemoryOgreAllocator.h delegates allocation to Ogre, which may be useful to Ogre users (you will have to manually change linking options of CEGUIBase!)" )

set( CEGUI_STRING_CLASS 1 CACHE INT "Which string class should CEGUI use
1 - utf8 and utf32 aware inbuilt string,
2 - std::string without custom allocation,
3 - std::basic_string allocated according to allocator config" )

cmake_dependent_option( CEGUI_HAS_FREETYPE "Specifies whether to include Freetype font support" TRUE "FREETYPE_FOUND" FALSE )
cmake_dependent_option( CEGUI_HAS_PCRE_REGEX "Specifies whether to include PCRE regexp matching for editbox string validation" TRUE "PCRE_FOUND" FALSE )
cmake_dependent_option( CEGUI_HAS_MINIZIP_RESOURCE_PROVIDER "Specifies whether to build the minizip based resource provider" TRUE "MINIZIP_FOUND" FALSE )
option( CEGUI_HAS_EXPRESSION_DIM "Specifies whether to build the ExpressionDim support for Falagard" FALSE )
option( CEGUI_HAS_DEFAULT_LOGGER "Specifies whether to build the DefaultLogger implementation" TRUE)

cmake_dependent_option( CEGUI_BUILD_XMLPARSER_EXPAT "Specifies whether to build the Expat based XMLParser module" TRUE "EXPAT_FOUND" FALSE)
cmake_dependent_option( CEGUI_BUILD_XMLPARSER_XERCES "Specifies whether to build the Xerces-C++ based XMLParser module" TRUE "XERCESC_FOUND" FALSE )
cmake_dependent_option( CEGUI_BUILD_XMLPARSER_LIBXML2 "Specifies whether to build the libxml2 based XMLParser module" TRUE "LIBXML2_FOUND" FALSE )
cmake_dependent_option( CEGUI_BUILD_XMLPARSER_RAPIDXML "Specifies whether to build the RapidXML based XMLParser module" TRUE "RAPIDXML_FOUND" FALSE )
cmake_dependent_option( CEGUI_BUILD_XMLPARSER_TINYXML "Specifies whether to build the TinyXML based XMLParser module" TRUE "TINYXML_FOUND" FALSE )

cmake_dependent_option( CEGUI_BUILD_IMAGECODEC_SILLY "Specifies whether to build the SILLY based ImageCodec module" TRUE "SILLY_FOUND" FALSE )
cmake_dependent_option( CEGUI_BUILD_IMAGECODEC_DEVIL "Specifies whether to build the DevIL based ImageCodec module" TRUE "IL_FOUND" FALSE )
cmake_dependent_option( CEGUI_BUILD_IMAGECODEC_FREEIMAGE "Specifies whether to build the FreeImage based ImageCodec module" TRUE "FREEIMAGE_FOUND" FALSE )
cmake_dependent_option( CEGUI_BUILD_IMAGECODEC_CORONA "Specifies whether to build the Corona based ImageCodec module" TRUE "CORONA_FOUND" FALSE )
option( CEGUI_BUILD_IMAGECODEC_STB "Specifies whether to build the STB based ImageCodec module" TRUE )
option( CEGUI_BUILD_IMAGECODEC_TGA "Specifies whether to build the based TGA only ImageCodec module" TRUE )
cmake_dependent_option( CEGUI_BUILD_IMAGECODEC_PVR "Specifies whether to build the PVR only ImageCodec module" TRUE "PVRTOOLS_FOUND" FALSE )

cmake_dependent_option( CEGUI_BUILD_RENDERER_OPENGL "Specifies whether to build the OpenGL renderer module" TRUE "OPENGL_FOUND" FALSE )
cmake_dependent_option( CEGUI_BUILD_RENDERER_OPENGL3 "Specifies whether to build the OpenGL 3+ (core profile) renderer module" TRUE "OPENGL_FOUND;GLM_FOUND" FALSE )
cmake_dependent_option( CEGUI_BUILD_RENDERER_OGRE "Specifies whether to build the Ogre renderer module" TRUE "OGRE_FOUND" FALSE )
cmake_dependent_option( CEGUI_BUILD_RENDERER_IRRLICHT "Specifies whether to build the Irrlicht renderer module" TRUE "IRRLICHT_FOUND" FALSE )
cmake_dependent_option( CEGUI_BUILD_RENDERER_DIRECTFB "Specifies whether to build the DirectFB renderer module" FALSE "DIRECTFB_FOUND" FALSE )
cmake_dependent_option( CEGUI_BUILD_RENDERER_DIRECT3D9 "Specifies whether to build the Direct3D 9 renderer module" TRUE "DIRECTXSDK_FOUND;NOT DIRECTXSDK_MAX_D3D LESS 9" FALSE )
cmake_dependent_option( CEGUI_BUILD_RENDERER_DIRECT3D10 "Specifies whether to build the Direct3D 10 renderer module" TRUE "DIRECTXSDK_FOUND;NOT DIRECTXSDK_MAX_D3D LESS 10" FALSE )
cmake_dependent_option( CEGUI_BUILD_RENDERER_DIRECT3D11 "Specifies whether to build the Direct3D 11 renderer module" TRUE "DIRECTXSDK_FOUND;D3DX11EFFECTS_FOUND;NOT DIRECTXSDK_MAX_D3D LESS 11" FALSE )
option( CEGUI_BUILD_RENDERER_NULL "Specifies whether to build the null renderer module" TRUE )
cmake_dependent_option( CEGUI_BUILD_RENDERER_OPENGLES "Specifies whether to build the OpenGLES renderer module" TRUE "OPENGLES_FOUND" FALSE )

cmake_dependent_option( CEGUI_BUILD_LUA_MODULE "Specifies whether to build the Lua based script module" TRUE "LUA51_FOUND" FALSE )
cmake_dependent_option( CEGUI_BUILD_LUA_GENERATOR "Specifies whether to build the custom tolua binding generator 'toluappcegui'" FALSE "LUA51_FOUND" FALSE )

cmake_dependent_option( CEGUI_BUILD_PYTHON_MODULES "Specifies whether to build the Python extension module(s)" TRUE "PYTHONINTERP_FOUND;PYTHONLIBS_FOUND;Boost_FOUND" FALSE )

cmake_dependent_option( CEGUI_BUILD_EMBEDDED_GLEW "Specifies whether to use embedded GLEW when external library was found" FALSE "GLEW_FOUND" TRUE )
cmake_dependent_option( CEGUI_BUILD_EMBEDDED_TOLUAPP "Specifies whether to use embedded tolua++ when external library was found" FALSE "LUA51_FOUND;TOLUAPP_FOUND" ${LUA51_FOUND} )

cmake_dependent_option( CEGUI_OPTION_SAFE_LUA_MODULE "Specifies whether to enable extra validation in the Lua script module in non-debug builds" FALSE "CEGUI_BUILD_LUA_MODULE" FALSE )

set( CEGUI_BIDI_SUPPORT "0" CACHE STRING "Select whether bi-directional text is enabled and which library should be used:
    0: Disabled.
    1: Use integrated minibidi library.
    2: Use external fribidi library."
)

cmake_dependent_option( CEGUI_SAMPLES_USE_OPENGL "Specifies whether the sample applications will use the OpenGL renderer" TRUE "CEGUI_BUILD_RENDERER_OPENGL;GLFW_FOUND" FALSE )
cmake_dependent_option( CEGUI_SAMPLES_USE_OPENGL3 "Specifies whether the sample applications will use the OpenGL 3 renderer" TRUE "CEGUI_BUILD_RENDERER_OPENGL3;GLFW_FOUND" FALSE )
cmake_dependent_option( CEGUI_SAMPLES_USE_OGRE "Specifies whether the sample applications will use the Ogre renderer" TRUE "CEGUI_BUILD_RENDERER_OGRE;OIS_FOUND" FALSE )

cmake_dependent_option( CEGUI_SAMPLES_USE_IRRLICHT "Specifies whether the sample applications will use the Irrlicht renderer" TRUE "CEGUI_BUILD_RENDERER_IRRLICHT" FALSE )
cmake_dependent_option( CEGUI_SAMPLES_USE_DIRECTFB "Specifies whether the sample applications will use the DirectFB renderer" TRUE "CEGUI_BUILD_RENDERER_DIRECTFB" FALSE )
cmake_dependent_option( CEGUI_SAMPLES_USE_DIRECT3D9 "Specifies whether the sample applications will use the Direct3D 9 renderer" TRUE "CEGUI_BUILD_RENDERER_DIRECT3D9" FALSE )
cmake_dependent_option( CEGUI_SAMPLES_USE_DIRECT3D10 "Specifies whether the sample applications will use the Direct3D 10 renderer" TRUE "CEGUI_BUILD_RENDERER_DIRECT3D10" FALSE )

if (CEGUI_SAMPLES_USE_OPENGL OR CEGUI_SAMPLES_USE_OGRE OR CEGUI_SAMPLES_USE_IRRLICHT OR CEGUI_SAMPLES_USE_DIRECT3D9 OR CEGUI_SAMPLES_USE_DIRECT3D10 OR CEGUI_SAMPLES_USE_DIRECTFB)
    set (_CEGUI_SAMPLES_ENABLED_DEFAULT TRUE)
else()
    set (_CEGUI_SAMPLES_ENABLED_DEFAULT FALSE)
endif()
cmake_dependent_option( CEGUI_SAMPLES_ENABLED "Specifies whether to build the CEGUI sample applications" TRUE "_CEGUI_SAMPLES_ENABLED_DEFAULT" FALSE )

if (UNIX AND NOT APPLE AND NOT WIN32)
    cmake_dependent_option( CEGUI_SAMPLES_USE_GTK2 "Specifies whether the sample applications will make use of the GTK2 UI for renderer selection." FALSE "GTK2_FOUND;CEGUI_SAMPLES_ENABLED" FALSE )
endif()

option( CEGUI_HAS_VERSION_SUFFIX "Specifies whether to append a version number to the name of output binaries." FALSE )
set (CEGUI_VERSION_SUFFIX ${CEGUI_VERSION} CACHE STRING "Specifies the version suffix to be appended to the name of output binaries when CEGUI_HAS_VERSION_SUFFIX is enabled." )

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

# regression and performance tests
cmake_dependent_option( CEGUI_BUILD_TESTS "Specifies whether to build the regression and performance tests." FALSE "Boost_FOUND" FALSE )


################################################################################
# Define vars holding the names for all the libs we can build for CEGUI.
################################################################################
# Core lib name
set( CEGUI_BASE_LIBNAME CEGUIBase )

# Renderer module names.
set( CEGUI_OPENGL_RENDERER_LIBNAME CEGUIOpenGLRenderer )
set( CEGUI_OPENGL3_RENDERER_LIBNAME CEGUIOpenGL3Renderer )
set( CEGUI_OGRE_RENDERER_LIBNAME CEGUIOgreRenderer)
set( CEGUI_IRRLICHT_RENDERER_LIBNAME CEGUIIrrlichtRenderer )
set( CEGUI_DIRECT3D9_RENDERER_LIBNAME CEGUIDirect3D9Renderer )
set( CEGUI_DIRECT3D10_RENDERER_LIBNAME CEGUIDirect3D10Renderer )
set( CEGUI_DIRECT3D11_RENDERER_LIBNAME CEGUIDirect3D11Renderer )
set( CEGUI_NULL_RENDERER_LIBNAME CEGUINullRenderer )
set( CEGUI_OPENGLES_RENDERER_LIBNAME CEGUIOpenGLESRenderer )

# XML parser module names
set( CEGUI_EXPAT_PARSER_LIBNAME CEGUIExpatParser )
set( CEGUI_TINYXML_PARSER_LIBNAME CEGUITinyXMLParser )
set( CEGUI_XERCES_PARSER_LIBNAME CEGUIXercesParser )
set( CEGUI_RAPIDXML_PARSER_LIBNAME CEGUIRapidXMLParser )
set( CEGUI_LIBXML2_PARSER_LIBNAME CEGUILibXMLParser )

# ImageCodec module names
set( CEGUI_SILLY_IMAGECODEC_LIBNAME CEGUISILLYImageCodec )
set( CEGUI_DEVIL_IMAGECODEC_LIBNAME CEGUIDevILImageCodec )
set( CEGUI_FREEIMAGE_IMAGECODEC_LIBNAME CEGUIFreeImageImageCodec )
set( CEGUI_CORONA_IMAGECODEC_LIBNAME CEGUICoronaImageCodec )
set( CEGUI_TGA_IMAGECODEC_LIBNAME CEGUITGAImageCodec )
set( CEGUI_STB_IMAGECODEC_LIBNAME CEGUISTBImageCodec )
set( CEGUI_PVR_IMAGECODEC_LIBNAME CEGUIPVRImageCodec )

# WindowRenderer set module names
set( CEGUI_FALAGARD_WR_LIBNAME CEGUIFalagardWRBase )

# Scripting module names and related items.
set( CEGUI_TOLUAPP_LIBNAME CEGUItoluapp )
set( CEGUI_TOLUAPP_GENERATOR_EXENAME toluappcegui )
set( CEGUI_LUA_SCRIPTMODULE_LIBNAME CEGUILuaScriptModule )

set( CEGUI_PYCEGUI_CORE_LIBNAME PyCEGUI )
set( CEGUI_PYCEGUI_OPENGL_RENDERER_LIBNAME PyCEGUIOpenGLRenderer )
set( CEGUI_PYCEGUI_OGRE_RENDERER_LIBNAME PyCEGUIOgreRenderer )
set( CEGUI_PYCEGUI_NULL_RENDERER_LIBNAME PyCEGUINullRenderer )

# sample related module names
set( CEGUI_SAMPLEHELPER_LIBNAME CEGUISampleHelper )

# Additional lib names
set( CEGUI_COMMON_DIALOGS_LIBNAME CEGUICommonDialogs )

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
    set( CEGUI_OPTION_DEFAULT_XMLPARSER "LibxmlParser" CACHE STRING "Specifies the XMLParser module to use as the default" )
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
elseif (CEGUI_BUILD_IMAGECODEC_TGA)
    set( CEGUI_OPTION_DEFAULT_IMAGECODEC "TGAImageCodec" CACHE STRING "Specifies the ImageCodec module to use as the default" )
    set( CEGUI_STATIC_IMAGECODEC_MODULE ${CEGUI_TGA_IMAGECODEC_LIBNAME} CACHE STRING "Specifies image codec library to link to samples in static builds." )
else()
message(WARNING "None of the image codec modules are going to be built.
You should ensure that CEGUI_OPTION_DEFAULT_IMAGECODEC is set to something
appropriate.")
endif()


################################################################################
# Add main header locations (for everything we build)
################################################################################
include_directories(
    ${CMAKE_CURRENT_BINARY_DIR}/cegui/include
    ${CMAKE_CURRENT_SOURCE_DIR}/cegui/include
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
set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY})
set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY})
set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY_MINSIZEREL ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY})
set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELWITHDEBINFO ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY})

set (CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set (CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
set (CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
set (CMAKE_LIBRARY_OUTPUT_DIRECTORY_MINSIZEREL ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
set (CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELWITHDEBINFO ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

# Set up Bidi variables based on option selected
if ( CEGUI_BIDI_SUPPORT EQUAL 1 )
    set( CEGUI_USE_MINIBIDI TRUE)
    set( CEGUI_USE_FRIBIDI FALSE)
elseif ( CEGUI_BIDI_SUPPORT EQUAL 2 )
    set( CEGUI_USE_MINIBIDI FALSE)
    set( CEGUI_USE_FRIBIDI TRUE)
else ()
    set( CEGUI_USE_MINIBIDI FALSE)
    set( CEGUI_USE_FRIBIDI FALSE)
endif ()

# Point TOLUAPP vars to location of embedded lib if using that option
if (CEGUI_BUILD_EMBEDDED_TOLUAPP)
    set (TOLUAPP_LIBRARIES ${CEGUI_TOLUAPP_LIBNAME})
    set (TOLUAPP_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/cegui/include/CEGUI/ScriptModules/Lua/support/tolua++)
endif()

# set up install sub-directories
if (UNIX AND NOT APPLE AND NOT WIN32)
    if (CMAKE_SIZEOF_VOID_P EQUAL 8 AND EXISTS "${CMAKE_INSTALL_PREFIX}/lib64")
        set( CEGUI_LIB_INSTALL_DIR lib64 )
    elseif (CMAKE_SIZEOF_VOID_P EQUAL 4 AND EXISTS "${CMAKE_INSTALL_PREFIX}/lib32")
        set( CEGUI_LIB_INSTALL_DIR lib32 )
    else()
        set( CEGUI_LIB_INSTALL_DIR lib )
    endif()

    option( CEGUI_SLOTTED_INSTALLATION "Specifies whether CEGUI should be installed into version specific sub-directories." TRUE )

    if (CEGUI_SLOTTED_INSTALLATION)
        set( CEGUI_SLOT_VERSION "-${CEGUI_VERSION_MAJOR}.${CEGUI_VERSION_MINOR}" )
        set( CEGUI_SLOT_INSTALL_SUBDIR "/${CMAKE_PROJECT_NAME}${CEGUI_SLOT_VERSION}" )

        set( CEGUI_DATA_INSTALL_DIR "share${CEGUI_SLOT_INSTALL_SUBDIR}" )
        set( CEGUI_DOC_INSTALL_DIR "share/doc${CEGUI_SLOT_INSTALL_SUBDIR}" )
    else()
        set( CEGUI_DATA_INSTALL_DIR "share/${CMAKE_PROJECT_NAME}" )
        set( CEGUI_DOC_INSTALL_DIR "share/doc/${CMAKE_PROJECT_NAME}" )
    endif()

    set( CEGUI_PKGCONFIG_INSTALL_DIR "${CEGUI_LIB_INSTALL_DIR}/pkgconfig" )
    set( CEGUI_LIB_INSTALL_DIR "${CEGUI_LIB_INSTALL_DIR}${CEGUI_SLOT_INSTALL_SUBDIR}" )
    set( CEGUI_INCLUDE_INSTALL_DIR "include${CEGUI_SLOT_INSTALL_SUBDIR}" )
endif()

################################################################################
# Deal with files we generate from template files
################################################################################
configure_file( cegui/include/CEGUI/Version.h.in cegui/include/CEGUI/Version.h )
configure_file( cegui/include/CEGUI/Config.h.in cegui/include/CEGUI/Config.h )
configure_file( Samples/common/include/CEGUISamplesConfig.h.in Samples/common/include/CEGUISamplesConfig.h )

if (UNIX AND NOT APPLE)
    configure_file( cegui/CEGUI.pc.in cegui/CEGUI${CEGUI_SLOT_VERSION}.pc @ONLY )
    install(FILES ${CMAKE_BINARY_DIR}/cegui/CEGUI${CEGUI_SLOT_VERSION}.pc DESTINATION ${CEGUI_PKGCONFIG_INSTALL_DIR})

    if (CEGUI_BUILD_RENDERER_NULL)
        configure_file( cegui/CEGUI-NULL.pc.in cegui/CEGUI-NULL${CEGUI_SLOT_VERSION}.pc @ONLY )
        install(FILES ${CMAKE_BINARY_DIR}/cegui/CEGUI-NULL${CEGUI_SLOT_VERSION}.pc DESTINATION ${CEGUI_PKGCONFIG_INSTALL_DIR})
    endif()
    if (CEGUI_BUILD_RENDERER_IRRLICHT)
        configure_file( cegui/CEGUI-IRRLICHT.pc.in cegui/CEGUI-IRRLICHT${CEGUI_SLOT_VERSION}.pc @ONLY )
        install(FILES ${CMAKE_BINARY_DIR}/cegui/CEGUI-IRRLICHT${CEGUI_SLOT_VERSION}.pc DESTINATION ${CEGUI_PKGCONFIG_INSTALL_DIR})
    endif()
    if (CEGUI_BUILD_RENDERER_OGRE)
        configure_file( cegui/CEGUI-OGRE.pc.in cegui/CEGUI-OGRE${CEGUI_SLOT_VERSION}.pc @ONLY )
        install(FILES ${CMAKE_BINARY_DIR}/cegui/CEGUI-OGRE${CEGUI_SLOT_VERSION}.pc DESTINATION ${CEGUI_PKGCONFIG_INSTALL_DIR})
    endif()
    if (CEGUI_BUILD_RENDERER_OPENGL)
        configure_file( cegui/CEGUI-OPENGL.pc.in cegui/CEGUI-OPENGL${CEGUI_SLOT_VERSION}.pc @ONLY )
        install(FILES ${CMAKE_BINARY_DIR}/cegui/CEGUI-OPENGL${CEGUI_SLOT_VERSION}.pc DESTINATION ${CEGUI_PKGCONFIG_INSTALL_DIR})
    endif()
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
    add_subdirectory(Samples)
endif()

add_subdirectory(datafiles)
#add_subdirectory(doc)

if (CEGUI_BUILD_TESTS)
    add_subdirectory(tests)
endif()

################################################################################
# CPack (mostly for source tarballs)
################################################################################
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Crazy Eddie's Gui System")
set(CPACK_PACKAGE_VENDOR "CEGUI team")
set(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_SOURCE_DIR}/doc/README")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/doc/COPYING")
set(CPACK_PACKAGE_VERSION_MAJOR "${CEGUI_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${CEGUI_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${CEGUI_VERSION_PATCH}")

set(CPACK_SOURCE_GENERATOR TBZ2 ZIP)

set(CPACK_SOURCE_PACKAGE_FILE_NAME "cegui-${CEGUI_VERSION}" CACHE INTERNAL "tarball basename")

set(CPACK_SOURCE_IGNORE_FILES
# repository history should not be in source tarballs
"\\\\.hg.*"
# most likely not needed in the tarball
"promo/"
"cppcheck-output"
"perform-cppcheck"

# the rest is modelled after .hgignore
"build/"
"~$"

"Thumbs.db"
"\\\\.directory"

"\\\\.kdev4"
"\\\\.settings"
"\\\\.project"
"\\\\.cproject"
"\\\\.pydevproject"
)

include(CPack)
