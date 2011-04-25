cmake_minimum_required(VERSION 2.8)
if (POLICY CMP0017)
    cmake_policy(SET CMP0017 OLD)
endif()

set( CEGUI_VERSION_MAJOR 0)
set( CEGUI_VERSION_MINOR 7)
set( CEGUI_VERSION_PATCH 9090)
set( CEGUI_VERSION ${CEGUI_VERSION_MAJOR}.${CEGUI_VERSION_MINOR}.${CEGUI_VERSION_PATCH} )

set( CEGUI_ABI_CURRENT 2)
set( CEGUI_ABI_REVISION 0)
set( CEGUI_ABI_AGE 0)
set( CEGUI_ABI_VERSION ${CEGUI_ABI_CURRENT}.${CEGUI_ABI_REVISION}.${CEGUI_ABI_AGE} )

set (CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
include(CEGUIMacros)

project(CEGUI)

set( BUILD_SHARED_LIBS TRUE CACHE BOOL "Whether to build shared or static libs" )

# search the provided dependencies on Win32 and Mac OS X platforms
if (WIN32 OR APPLE)
    set (CMAKE_PREFIX_PATH "${CMAKE_SOURCE_DIR}/dependencies" CACHE PATH "Path to dependencies")

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
endif()

if (NOT BUILD_SHARED_LIBS)
    add_definitions(-DCEGUI_STATIC)
    set (CEGUI_LIBRARY_TYPE STATIC)
else()
    set (CEGUI_LIBRARY_TYPE SHARED)
endif()

if (WIN32)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS -DNOMINMAX)

    if (BUILD_SHARED_LIBS)
        set (CMAKE_LIBRARY_PATH "${CMAKE_PREFIX_PATH}/lib/dynamic")
    else()
        set (CMAKE_LIBRARY_PATH "${CMAKE_PREFIX_PATH}/lib/static")
    endif()
endif()

# mac specifics
if (APPLE)
    set (CMAKE_OSX_ARCHITECTURES i386 CACHE STRING "Build architectures for Mac OS X" FORCE)
    find_library(CARBON_FWK NAMES Carbon)
endif()

#
# These check for libs and packages we might use.
#
find_package(PCRE)
find_package(Freetype)
find_package(ZLIB)

find_package(OpenGL)
find_package(GLEW)
find_package(GLUT)
find_package(DirectXSDK)
find_package(D3DX11Effects)
find_package(Irrlicht)
find_package(Ogre)
find_package(OIS)
find_package(DirectFB)

find_package(EXPAT)
find_package(XercesC)
find_package(LibXml2)
find_package(TinyXML)
find_package(RapidXML)

find_package(DevIL)
find_package(FreeImage)
find_package(SILLY)
find_package(Corona)

find_package(Lua51)
find_package(TOLUAPP)
find_package(PythonInterp)
find_package(PythonLibs)
find_package(Boost 1.36.0 COMPONENTS python)

find_package(Doxygen)
find_package(GTK2 COMPONENTS gtk)
# set for consistency with other deps
set (GTK2_INCLUDE_DIR ${GTK2_INCLUDE_DIRS})

# These define the configurable options
#
set( CEGUI_CUSTOM_ALLOCATORS 0 CACHE BOOL "If checked, CEGUI can use custom allocation. (for experienced developers only!)" )
set( CEGUI_CUSTOM_ALLOCATORS_DEGUG 0 CACHE BOOL "If checked, CEGUI allocations will pass debug info to allocators. (only used if CEGUI_CUSTOM_ALLOCATORS is checked)" )
set( CEGUI_CUSTOM_ALLOCATORS_INCLUDE "CEGUIMemoryStdAllocator.h" CACHE STRING "Which include file contains memory allocators and allocation configuration. (only used if CEGUI_CUSTOM_ALLOCATORS is checked)
We have bundled 2 allocators, mostly for demonstrational purposes.
CEGUIMemoryStdAllocator.h contains malloc allocator, use it as a starting point for your own allocators
CEGUIMemoryOgreAllocator.h delegates allocation to Ogre, which may be useful to Ogre users (you will have to manually change linking options of CEGUIBase!)" )

set( CEGUI_STRING_CLASS 1 CACHE INT "Which string class should CEGUI use
1 - utf8 and utf32 aware inbuilt string,
2 - std::string without custom allocation,
3 - std::basic_string allocated according to allocator config" )

set( CEGUI_HAS_EXPRESSION_DIM FALSE CACHE BOOL "Build the ExpressionDim support" )
set( CEGUI_HAS_DEFAULT_LOGGER TRUE CACHE BOOL "Include the default logger implementation" )
set( CEGUI_HAS_FREETYPE ${FREETYPE_FOUND} CACHE BOOL "Include Freetype font support" )
set( CEGUI_HAS_PCRE_REGEX ${PCRE_FOUND} CACHE BOOL "Include PCRE regexp matching for editbox string validation" )
set( CEGUI_HAS_MINIZIP_RESOURCE_PROVIDER ${ZLIB_FOUND} CACHE BOOL "Build the minizip based resource provider" )

set( CEGUI_BUILD_XMLPARSER_LIBXML2 ${LIBXML2_FOUND} CACHE BOOL "Build the libxml2 based XMLParser module" )
set( CEGUI_BUILD_XMLPARSER_XERCES ${XERCESC_FOUND} CACHE BOOL "Build the Xerces-C++ based XMLParser module" )
set( CEGUI_BUILD_XMLPARSER_EXPAT ${EXPAT_FOUND} CACHE BOOL "Build the Expat based XMLParser module" )
set( CEGUI_BUILD_XMLPARSER_TINYXML TRUE CACHE BOOL "Build the TinyXML based XMLParser module" )
set( CEGUI_BUILD_XMLPARSER_RAPIDXML FALSE CACHE BOOL "Build the RapidXML based XMLParser module" )

set( CEGUI_BUILD_IMAGECODEC_CORONA ${CORONA_FOUND} CACHE BOOL "Build the Corona based ImageCodec module" )
set( CEGUI_BUILD_IMAGECODEC_DEVIL ${IL_FOUND} CACHE BOOL "Build the DevIL based ImageCodec module" )
set( CEGUI_BUILD_IMAGECODEC_FREEIMAGE ${FREEIMAGE_FOUND} CACHE BOOL "Build the FreeImage based ImageCodec module" )
set( CEGUI_BUILD_IMAGECODEC_SILLY ${SILLY_FOUND} CACHE BOOL "Build the SILLY based ImageCodec module" )
set( CEGUI_BUILD_IMAGECODEC_TGA TRUE CACHE BOOL "Build the based TGA only ImageCodec module" )
set( CEGUI_BUILD_IMAGECODEC_STB TRUE CACHE BOOL "Build the STB based ImageCodec module" )

set( CEGUI_BUILD_RENDERER_OPENGL ${OPENGL_FOUND} CACHE BOOL "Build the OpenGL renderer module" )
set( CEGUI_BUILD_RENDERER_OGRE ${OGRE_FOUND} CACHE BOOL "Build the Ogre renderer module" )
set( CEGUI_BUILD_RENDERER_IRRLICHT ${IRRLICHT_FOUND} CACHE BOOL "Build the Irrlicht renderer module" )
set( CEGUI_BUILD_RENDERER_NULL TRUE CACHE BOOL "Build the null renderer module" )
set( CEGUI_BUILD_RENDERER_DIRECTFB FALSE CACHE BOOL "Build the DirectFB renderer module" )

if (DIRECTXSDK_FOUND AND NOT (DIRECTSDK_MAX_D3D LESS 9))
    set( CEGUI_BUILD_RENDERER_DIRECT3D9 TRUE CACHE BOOL "Build the Direct3D 9 renderer module" )
else()
    set( CEGUI_BUILD_RENDERER_DIRECT3D9 FALSE CACHE BOOL "Build the Direct3D 9 renderer module" )
endif()

if (DIRECTXSDK_FOUND AND NOT (DIRECTSDK_MAX_D3D LESS 10))
    set( CEGUI_BUILD_RENDERER_DIRECT3D10 TRUE CACHE BOOL "Build the Direct3D 10 renderer module" )
else()
    set( CEGUI_BUILD_RENDERER_DIRECT3D10 FALSE CACHE BOOL "Build the Direct3D 10 renderer module" )
endif()

if (DIRECTXSDK_FOUND AND D3DX11EFFECTS_FOUND AND NOT (DIRECTSDK_MAX_D3D LESS 11))
    set( CEGUI_BUILD_RENDERER_DIRECT3D11 TRUE CACHE BOOL "Build the Direct3D 11 renderer module" )
else()
    set( CEGUI_BUILD_RENDERER_DIRECT3D11 FALSE CACHE BOOL "Build the Direct3D 11 renderer module" )
endif()

set( CEGUI_BUILD_LUA_MODULE ${LUA51_FOUND} CACHE BOOL "Build the Lua based script module" )
set( CEGUI_BUILD_LUA_GENERATOR FALSE CACHE BOOL "Build ceguitolua, the custom tolua binding generator" )

if (PYTHONINTERP_FOUND AND PYHONLIBS_FOUND AND Boost_FOUND)
    set( CEGUI_BUILD_PYTHON_MODULES TRUE CACHE BOOL "Specifies whether the Python extension modules will be built." )
else()
    set( CEGUI_BUILD_PYTHON_MODULES FALSE CACHE BOOL "Specifies whether the Python extension modules will be built." )
endif()

if (TINYXML_FOUND)
    set( CEGUI_BUILD_EMBEDDED_TINYXML FALSE CACHE BOOL "Specifies whether to use internal copy of TinyXML even if external library is available" )
else()
    set( CEGUI_BUILD_EMBEDDED_TINYXML TRUE CACHE BOOL "Specifies whether to use internal copy of TinyXML even if external library is available" )
endif()

if (GLEW_FOUND)
    set( CEGUI_BUILD_EMBEDDED_GLEW FALSE CACHE BOOL "Specifies whether to use internal copy of GLEW even if external library is available" )
else ()
    set( CEGUI_BUILD_EMBEDDED_GLEW TRUE CACHE BOOL "Specifies whether to use internal copy of GLEW even if external library is available" )
endif()

if (LUA51_FOUND)
    if (TOLUAPP_FOUND)
        set( CEGUI_BUILD_EMBEDDED_TOLUAPP FALSE CACHE BOOL "Specifies whether to use internal copy of tolua++ even if external library is available" )
    else()
        set( CEGUI_BUILD_EMBEDDED_TOLUAPP TRUE CACHE BOOL "Specifies whether to use internal copy of tolua++ even if external library is available" )
    endif()
endif()

set( CEGUI_OPTION_SAFE_LUA_MODULE FALSE CACHE BOOL "Specifies whether to enable extra validation in the Lua script module in non-debug builds" )

set( CEGUI_BIDI_SUPPORT "0" CACHE STRING "Select whether bi-directional text is enabled and which library should be used:
    0: Disabled.
    1: Use integrated minibidi library.
    2: Use external fribidi library."
)

if (OPENGL_FOUND AND GLUT_FOUND)
    set( CEGUI_SAMPLES_USE_OPENGL TRUE CACHE BOOL "Specifies whether the sample applications will make use of the OpenGL renderer" )
else()
    set( CEGUI_SAMPLES_USE_OPENGL FALSE CACHE BOOL "Specifies whether the sample applications will make use of the OpenGL renderer" )
endif()

if (OGRE_FOUND AND OIS_FOUND)
    set( CEGUI_SAMPLES_USE_OGRE TRUE CACHE BOOL "Specifies whether the sample applications will make use of the Ogre renderer" )
else()
    set( CEGUI_SAMPLES_USE_OGRE FALSE CACHE BOOL "Specifies whether the sample applications will make use of the Ogre renderer" )
endif()

set( CEGUI_SAMPLES_USE_IRRLICHT ${IRRLICHT_FOUND} CACHE BOOL "Specifies whether the sample applications will make use of the Irrlicht renderer" )
set( CEGUI_SAMPLES_USE_DIRECTFB ${DIRECTFB_FOUND} CACHE BOOL "Specifies whether the sample applications will make use of the DirectFB renderer" )
set( CEGUI_SAMPLES_USE_DIRECT3D9 ${CEGUI_BUILD_RENDERER_DIRECT3D9} CACHE BOOL "Specifies whether the sample applications will make use of the Direct3D 9 renderer" )
set( CEGUI_SAMPLES_USE_DIRECT3D10 ${CEGUI_BUILD_RENDERER_DIRECT3D10} CACHE BOOL "Specifies whether the sample applications will make use of the Direct3D 10 renderer" )

if (CEGUI_SAMPLES_USE_OPENGL OR CEGUI_SAMPLES_USE_OGRE OR CEGUI_SAMPLES_USE_IRRLICHT OR CEGUI_SAMPLES_USE_DIRECT3D9 OR CEGUI_SAMPLES_USE_DIRECT3D10 OR CEGUI_SAMPLES_USE_DIRECTFB)
    set (CEGUI_SAMPLES_ENABLED TRUE CACHE BOOL "Build the CEGUI samples")
else()
    set (CEGUI_SAMPLES_ENABLED FALSE CACHE BOOL "Build the CEGUI samples")
endif()

set( CEGUI_SAMPLES_USE_GTK2 ${GTK2_FOUND} CACHE BOOL "Specifies whether the sample applications will make use of the GTK2 UI for renderer selection" )

set (CEGUI_HAS_VERSION_SUFFIX FALSE CACHE BOOL "Whether to append some version number to the output binaries.")
set (CEGUI_VERSION_SUFFIX ${CEGUI_VERSION} CACHE STRING "String holding a version suffix to optionally be appended to the name of output binaries.")

if (WIN32)
    set (CEGUI_BUILD_SUFFIX "_d" CACHE STRING "String holding a suffix appended to the name of output binaries (under CMake build, only used for debug).")
else()
    set (CEGUI_BUILD_SUFFIX "" CACHE STRING "String holding a suffix appended to the name of output binaries (under CMake build, only used for debug).")
endif()

if (CEGUI_BUILD_SUFFIX)
    set (CEGUI_HAS_BUILD_SUFFIX TRUE)
    set (CMAKE_DEBUG_POSTFIX ${CEGUI_BUILD_SUFFIX})
endif()

# Set up Bidi variables based on option selected
#
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

#
# Select one of the XML parser modules to be the default, warning if none are
# available.
#
if (CEGUI_BUILD_XMLPARSER_EXPAT)
    set( CEGUI_OPTION_DEFAULT_XMLPARSER "ExpatParser" CACHE STRING "Specifies the XMLParser module to use as the default" FORCE)
elseif (CEGUI_BUILD_XMLPARSER_XERCES)
    set( CEGUI_OPTION_DEFAULT_XMLPARSER "XercesParser" CACHE STRING "Specifies the XMLParser module to use as the default" FORCE)
elseif (CEGUI_BUILD_XMLPARSER_LIBXML2)
    set( CEGUI_OPTION_DEFAULT_XMLPARSER "LibxmlParser" CACHE STRING "Specifies the XMLParser module to use as the default" FORCE)
elseif (CEGUI_BUILD_XMLPARSER_RAPIDXML)
    set( CEGUI_OPTION_DEFAULT_XMLPARSER "RapidXMLParser" CACHE STRING "Specifies the XMLParser module to use as the default" FORCE)
elseif (CEGUI_BUILD_XMLPARSER_TINYXML)
    set( CEGUI_OPTION_DEFAULT_XMLPARSER "TinyXMLParser" CACHE STRING "Specifies the XMLParser module to use as the default" FORCE)
else()
    message(WARNING "None of the XML parser modules are going to be built.
You should ensure that CEGUI_OPTION_DEFAULT_XMLPARSER is set to something
appropriate.")
endif()


#
# Select one of the image codec modules to be the default, warning if none are
# available.
#
if (CEGUI_BUILD_IMAGECODEC_SILLY)
    set( CEGUI_OPTION_DEFAULT_IMAGECODEC "SILLYImageCodec" CACHE STRING "Specifies the ImageCodec module to use as the default" FORCE)
elseif (CEGUI_BUILD_IMAGECODEC_DEVIL)
    set( CEGUI_OPTION_DEFAULT_IMAGECODEC "DevILImageCodec" CACHE STRING "Specifies the ImageCodec module to use as the default" FORCE)
elseif (CEGUI_BUILD_IMAGECODEC_FREEIMAGE)
    set( CEGUI_OPTION_DEFAULT_IMAGECODEC "FreeImageImageCodec" CACHE STRING "Specifies the ImageCodec module to use as the default" FORCE)
elseif (CEGUI_BUILD_IMAGECODEC_STB)
    set( CEGUI_OPTION_DEFAULT_IMAGECODEC "STBImageCodec" CACHE STRING "Specifies the ImageCodec module to use as the default" FORCE)
elseif (CEGUI_BUILD_IMAGECODEC_CORONA)
    set( CEGUI_OPTION_DEFAULT_IMAGECODEC "CoronaImageCodec" CACHE STRING "Specifies the ImageCodec module to use as the default" FORCE)
elseif (CEGUI_BUILD_IMAGECODEC_TGA)
    set( CEGUI_OPTION_DEFAULT_IMAGECODEC "TGAImageCodec" CACHE STRING "Specifies the ImageCodec module to use as the default" FORCE)
else()
message(WARNING "None of the image codec modules are going to be built.
You should ensure that CEGUI_OPTION_DEFAULT_IMAGECODEC is set to something
appropriate.")
endif()

#
# add main header locations
#
include_directories(
    ${CMAKE_CURRENT_BINARY_DIR}/cegui/include
    ${CMAKE_CURRENT_SOURCE_DIR}/cegui/include
)

#
# Here we define vars to hold names for all the libs we can build for CEGUI.
#
# Core lib name
set( CEGUI_BASE_LIBNAME CEGUIBase )

# Renderer module names.
set( CEGUI_OPENGL_RENDERER_LIBNAME CEGUIOpenGLRenderer )
set( CEGUI_OGRE_RENDERER_LIBNAME CEGUIOgreRenderer )
set( CEGUI_IRRLICHT_RENDERER_LIBNAME CEGUIIrrlichtRenderer )
set( CEGUI_DIRECT3D9_RENDERER_LIBNAME CEGUIDirect3D9Renderer )
set( CEGUI_DIRECT3D10_RENDERER_LIBNAME CEGUIDirect3D10Renderer )
set( CEGUI_DIRECT3D11_RENDERER_LIBNAME CEGUIDirect3D11Renderer )
set( CEGUI_NULL_RENDERER_LIBNAME CEGUINullRenderer )

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

# WindowRenderer set module names
set( CEGUI_FALAGARD_WR_LIBNAME CEGUIFalagardWRBase )

# Scripting module names.
set( CEGUI_LUA_SCRIPTMODULE_LIBNAME CEGUILuaScriptModule )

set( CEGUI_SAMPLEHELPER_LIBNAME CEGUISampleHelper )
set( CEGUI_TOLUAPP_LIBNAME CEGUItoluapp )

#
# make some adjustments if we will be building and using the bundled tolua++ lib
#
if (CEGUI_BUILD_EMBEDDED_TOLUAPP)
    set (TOLUAPP_LIBRARIES ${CEGUI_TOLUAPP_LIBNAME})
    set (TOLUAPP_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/cegui/include/ScriptingModules/LuaScriptModule/support/tolua++)
endif()

#
# Deal with files we generate from template files
#
configure_file( cegui/include/CEGUIVersion.h.in cegui/include/CEGUIVersion.h )
configure_file( cegui/include/CEGUIConfig.h.in cegui/include/CEGUIConfig.h )
configure_file( Samples/common/include/CEGUISamplesConfig.h.in Samples/common/include/CEGUISamplesConfig.h )

# descend into subdirs
add_subdirectory(cegui/src)
add_subdirectory(cegui/src/RendererModules)
add_subdirectory(cegui/src/XMLParserModules)
add_subdirectory(cegui/src/ImageCodecModules)
add_subdirectory(cegui/src/WindowRendererSets)
add_subdirectory(cegui/src/ScriptingModules)

if (CEGUI_SAMPLES_ENABLED)
    add_subdirectory(Samples)
endif()

add_subdirectory(datafiles)
#add_subdirectory(doc)

