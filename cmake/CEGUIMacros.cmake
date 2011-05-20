################################################################################
# Macro definitions used by CEGUI's cmake build
################################################################################

#
# gather *.cpp and .h files for the current location
#
macro (cegui_gather_files)
    file (RELATIVE_PATH _REL_SRC_DIR "${CMAKE_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}")
    string (REPLACE src include _REL_INC_DIR ${_REL_SRC_DIR})
    set (_CURR_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/${_REL_INC_DIR}")

    file (GLOB CORE_SOURCE_FILES RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" *.cpp)
    file (GLOB CORE_HEADER_FILES RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "${_CURR_INCLUDE_DIR}/*.h")
endmacro()

#
# Set the MSVC runtime option according to CEGUI_MSVC_STATIC_RUNTIME
#
macro(cegui_set_msvc_runtime_flags)
    if (CEGUI_MSVC_STATIC_RUNTIME)
        foreach(_BUILD_CONFIG DEBUG RELEASE RELWITHDEBINFO MINSIZEREL)
            foreach(_BUILD_VARS CMAKE_CXX_FLAGS_${_BUILD_CONFIG} CMAKE_C_FLAGS_${_BUILD_CONFIG})
                string(REGEX REPLACE /MD /MT ${_BUILD_VARS} ${${_BUILD_VARS}})
            endforeach()
        endforeach()
    endif()
endmacro()

#
# Find a bunch of libraries and append them to a variable
#
macro (cegui_find_libraries _LIBS_VAR _LIB_LIST)
    foreach (_LIB_NAME ${_LIB_LIST})
        set( _LIB_PATH "LIB-NOTFOUND" CACHE INTERNAL "...")
        find_library(_LIB_PATH NAMES ${_LIB_NAME} PATH_SUFFIXES ${ARGN})

        if (_LIB_PATH)
            set( ${_LIBS_VAR} ${${_LIBS_VAR}} ${_LIB_PATH} )
        else()
            set( ${_LIBS_VAR} ${_LIB_NAME}-NOTFOUND )
            break()
        endif()
    endforeach()
endmacro()

#
# Add libs to a target, and correctly handles static versions of libs built by the project
#
macro (cegui_target_link_libraries _TARGET_NAME)
    target_link_libraries(${_TARGET_NAME} ${ARGN})

    get_target_property(_TARGET_EXISTS ${_TARGET_NAME}_Static TYPE)
    if (_TARGET_EXISTS)
        foreach(_LIB ${ARGN})
            if (${_LIB} STREQUAL optimized OR ${_LIB} STREQUAL debug OR ${_LIB} STREQUAL general)
                set (_BUILD ${_LIB})
            else()
                get_target_property(_LIB_IS_IN_PROJECT ${_LIB}_Static TYPE)

                if (_LIB_IS_IN_PROJECT)
                    target_link_libraries(${_TARGET_NAME}_Static ${_BUILD} ${_LIB}_Static)
                else()
                    target_link_libraries(${_TARGET_NAME}_Static ${_BUILD} ${_LIB})
                endif()
            endif()
        endforeach()
    endif()
endmacro()

#
# add dynamic dependency libraries to a target
#
macro (cegui_add_dependency_dynamic_libs _TARGET_NAME _DEP_NAME)
    get_target_property(_TARGET_EXISTS ${_TARGET_NAME} TYPE)

    if (_TARGET_EXISTS)
        if (${_DEP_NAME}_LIBRARIES)
            if (${_DEP_NAME}_LIBRARIES_DBG)
                foreach(_LIB ${${_DEP_NAME}_LIBRARIES})
                    target_link_libraries(${_TARGET_NAME} optimized ${_LIB})
                endforeach()
                foreach(_LIB ${${_DEP_NAME}_LIBRARIES_DBG})
                    target_link_libraries(${_TARGET_NAME} debug ${_LIB})
                endforeach()
            else()
                target_link_libraries(${_TARGET_NAME} ${${_DEP_NAME}_LIBRARIES})
            endif()
        endif()
    endif()
endmacro()

#
# add static dependency libraries to a target, falling  back to dynamic libraries
# if static do not exist)
#
macro (cegui_add_dependency_static_libs _TARGET_NAME _DEP_NAME)
    get_target_property(_TARGET_EXISTS ${_TARGET_NAME} TYPE)

    if (_TARGET_EXISTS)
        if (${_DEP_NAME}_LIBRARIES_STATIC)
            if (${_DEP_NAME}_LIBRARIES_STATIC_DBG)
                foreach(_LIB ${${_DEP_NAME}_LIBRARIES_STATIC})
                    target_link_libraries(${_TARGET_NAME} optimized ${_LIB})
                endforeach()
                foreach(_LIB ${${_DEP_NAME}_LIBRARIES_STATIC_DBG})
                    target_link_libraries(${_TARGET_NAME} debug ${_LIB})
                endforeach()
            else()
                target_link_libraries(${_TARGET_NAME} ${${_DEP_NAME}_LIBRARIES_STATIC})
            endif()
        else()
            cegui_add_dependency_dynamic_libs(${_TARGET_NAME} ${_DEP_NAME})            
        endif()
    endif()
endmacro()

#
# add a dependency to a target (and it's static equivalent, if it exists)
#
macro (cegui_add_dependency _TARGET_NAME _DEP_NAME)
    include_directories(${${_DEP_NAME}_INCLUDE_DIR})

    ###########################################################################
    #                    NON-STATIC VERSION OF TARGET
    ###########################################################################
    if (${_DEP_NAME}_DEFINITIONS)
        set_property( TARGET ${_TARGET_NAME} APPEND PROPERTY COMPILE_DEFINITIONS ${${_DEP_NAME}_DEFINITIONS} )
    endif()

    if (${_DEP_NAME}_DEFINITIONS_DYNAMIC AND NOT CEGUI_MSVC_STATIC_RUNTIME)
        set_property( TARGET ${_TARGET_NAME} APPEND PROPERTY COMPILE_DEFINITIONS ${${_DEP_NAME}_DEFINITIONS_DYNAMIC} )
    endif()

    if (${_DEP_NAME}_DEFINITIONS_STATIC AND CEGUI_MSVC_STATIC_RUNTIME)
        set_property( TARGET ${_TARGET_NAME} APPEND PROPERTY COMPILE_DEFINITIONS ${${_DEP_NAME}_DEFINITIONS_STATIC} )
    endif()

    if (CEGUI_MSVC_STATIC_RUNTIME)
        cegui_add_dependency_static_libs(${_TARGET_NAME} ${_DEP_NAME})
    else()
        cegui_add_dependency_dynamic_libs(${_TARGET_NAME} ${_DEP_NAME})
    endif()


    ###########################################################################
    #    ADD DEPENDENCY DEFS TO STATIC VERSION OF TARGET (if it exists)
    ###########################################################################
    get_target_property(_STATIC_EXISTS ${_TARGET_NAME}_Static TYPE)

    if (_STATIC_EXISTS)
        if (${_DEP_NAME}_DEFINITIONS)
            set_property( TARGET ${_TARGET_NAME}_Static APPEND PROPERTY COMPILE_DEFINITIONS ${${_DEP_NAME}_DEFINITIONS} )
        endif()

        if (${_DEP_NAME}_DEFINITIONS_DYNAMIC AND NOT CEGUI_MSVC_STATIC_RUNTIME)
            set_property( TARGET ${_TARGET_NAME}_Static APPEND PROPERTY COMPILE_DEFINITIONS ${${_DEP_NAME}_DEFINITIONS_DYNAMIC} )
        endif()

        if (${_DEP_NAME}_DEFINITIONS_STATIC AND CEGUI_MSVC_STATIC_RUNTIME)
            set_property( TARGET ${_TARGET_NAME}_Static APPEND PROPERTY COMPILE_DEFINITIONS ${${_DEP_NAME}_DEFINITIONS_STATIC} )
        endif()

        if (CEGUI_MSVC_STATIC_RUNTIME)
            cegui_add_dependency_static_libs(${_TARGET_NAME}_Static ${_DEP_NAME})
        else()
            cegui_add_dependency_dynamic_libs(${_TARGET_NAME}_Static ${_DEP_NAME})
        endif()
    endif()

endmacro()


###############################################################################
#
# Define a CEGUI library to be built and optionally installed.
#
# Note: this creates a regular lib and a static lib with the _Static suffix.
#
# Parameters:
#       _LIB_NAME           - Name of the library to create.
#       _SOURCE_FILES_VAR   - Variable holding the source file names.
#       _HEADER_FILES_VAR   - Variable holding the header file names.
#       _INSTALL_BIN        - For linux, TRUE if the lib should be installed
#       _INSTALL_HEADERS    - For linux, TRUE if the headers should be installed
#
###############################################################################
macro (cegui_add_library _LIB_NAME _SOURCE_FILES_VAR _HEADER_FILES_VAR _INSTALL_BIN _INSTALL_HEADERS)
    file (RELATIVE_PATH _REL_SRC_DIR "${CMAKE_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}")
    string (REPLACE src include _REL_INC_DIR ${_REL_SRC_DIR})
    string(TOUPPER ${_LIB_NAME} _CEGUI_EXPORT_DEFINE)

    include_directories("${CMAKE_SOURCE_DIR}/${_REL_INC_DIR}")

    ###########################################################################
    #                       STATIC LIBRARY SET UP
    ###########################################################################
    if (CEGUI_BUILD_STATIC_CONFIGURATION)
        add_library(${_LIB_NAME}_Static STATIC ${${_SOURCE_FILES_VAR}} ${${_HEADER_FILES_VAR}})
        set_property(TARGET ${_LIB_NAME}_Static APPEND PROPERTY COMPILE_DEFINITIONS CEGUI_STATIC)
    endif()

    ###########################################################################
    #                       SHARED LIBRARY SET UP
    ###########################################################################
    add_library(${_LIB_NAME} SHARED ${${_SOURCE_FILES_VAR}} ${${_HEADER_FILES_VAR}})

    if (NOT CEGUI_MSVC_STATIC_RUNTIME)
        set_target_properties(${_LIB_NAME} PROPERTIES
            LINK_INTERFACE_LIBRARIES ""
        )
    endif()

    if (APPLE)
        set_target_properties(${_LIB_NAME} PROPERTIES
            INSTALL_NAME_DIR ${CEGUI_APPLE_DYLIB_INSTALL_PATH}
            BUILD_WITH_INSTALL_RPATH TRUE
        )
    else()
        set_target_properties(${_LIB_NAME} PROPERTIES
            INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/${CEGUI_LIB_INSTALL_DIR}"
        )
    endif()

    if (NOT APPLE OR CEGUI_APPLE_DYLIB_SET_VERSION_INFO)
        set_target_properties(${_LIB_NAME} PROPERTIES
            VERSION ${CEGUI_ABI_VERSION}
            SOVERSION ${CEGUI_ABI_CURRENT}
            DEFINE_SYMBOL ${_CEGUI_EXPORT_DEFINE}_EXPORTS
        )
    endif()

    ###########################################################################
    #                           INSTALLATION
    ###########################################################################
    if (${_INSTALL_BIN} AND UNIX AND NOT APPLE AND NOT WIN32)
        install(TARGETS ${_LIB_NAME}
            RUNTIME DESTINATION bin
            LIBRARY DESTINATION ${CEGUI_LIB_INSTALL_DIR}
            ARCHIVE DESTINATION ${CEGUI_LIB_INSTALL_DIR}
        )

        if (CEGUI_BUILD_STATIC_CONFIGURATION)
            install(TARGETS ${_LIB_NAME}_Static
                RUNTIME DESTINATION bin
                LIBRARY DESTINATION ${CEGUI_LIB_INSTALL_DIR}
                ARCHIVE DESTINATION ${CEGUI_LIB_INSTALL_DIR}
            )
        endif()
    endif()

    if (${_INSTALL_HEADERS} AND UNIX AND NOT APPLE AND NOT WIN32)
        string (REPLACE "cegui/src" "" _REL_HEADER_DIR ${_REL_SRC_DIR})
        install(FILES ${${_HEADER_FILES_VAR}} DESTINATION "${CEGUI_INCLUDE_INSTALL_DIR}/${CMAKE_PROJECT_NAME}/${_REL_HEADER_DIR}")
    endif()
endmacro()

#
# Do setup for app bundle (Apple OS X)
#
macro (cegui_apple_app_setup _TARGET_NAME _STATIC)
    set_target_properties(${_TARGET_NAME} PROPERTIES
        MACOSX_BUNDLE TRUE
    )

    if (CEGUI_APPLE_SYMLINK_DEPENDENCIES_TO_SAMPLE_APPS)
        set (_ACTIONCMD ln -sf)
        set (_ACTIONMSG "symlinks to")
    else()
        set (_ACTIONCMD cp -Rf)
        set (_ACTIONMSG "copies of")
    endif()

    file (REMOVE_RECURSE
            ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${CEGUI_TARGET_NAME}.app/Contents/Frameworks
            ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${CEGUI_TARGET_NAME}.app/Contents/Resources
    )
    file (MAKE_DIRECTORY
            ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${CEGUI_TARGET_NAME}.app/Contents/Frameworks
            ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${CEGUI_TARGET_NAME}.app/Contents/Resources
    )

    if (NOT _STATIC)
        add_custom_command(TARGET ${CEGUI_TARGET_NAME} POST_BUILD 
            COMMAND ${_ACTIONCMD} ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/*.{dylib,framework} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${CEGUI_TARGET_NAME}.app/Contents/Frameworks/
            COMMENT "Creating ${_ACTIONMSG} built cegui libraries and frameworks in ${CEGUI_TARGET_NAME}.app")
    endif()

    add_custom_command(TARGET ${CEGUI_TARGET_NAME} POST_BUILD 
        COMMAND ${_ACTIONCMD} ${CMAKE_PREFIX_PATH}/lib/*.dylib ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${CEGUI_TARGET_NAME}.app/Contents/Frameworks/
        COMMENT "Creating ${_ACTIONMSG} dependency libraries in ${CEGUI_TARGET_NAME}.app")

    add_custom_command(TARGET ${CEGUI_TARGET_NAME} POST_BUILD 
        COMMAND ${_ACTIONCMD} ${CMAKE_SOURCE_DIR}/datafiles ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${CEGUI_TARGET_NAME}.app/Contents/Resources/
        COMMENT "Creating ${_ACTIONMSG} sample datafiles ${CEGUI_TARGET_NAME}.app")
endmacro()

#
# Define a CEGUI sample app
#
macro (cegui_add_sample _NAME)
    set (CEGUI_TARGET_NAME ${_NAME}${CEGUI_SLOT_VERSION})

    include_directories(${CMAKE_SOURCE_DIR}/Samples/common/include)

    if (CEGUI_SAMPLES_USE_DIRECT3D9 OR CEGUI_SAMPLES_USE_DIRECT3D10)
        link_directories(${DIRECTXSDK_LIBRARY_DIR})
    endif()

    cegui_gather_files()

    ###########################################################################
    #                     Statically Linked Executable
    ###########################################################################
    if (CEGUI_BUILD_STATIC_CONFIGURATION)
        add_executable(${CEGUI_TARGET_NAME}_Static ${CORE_SOURCE_FILES} ${CORE_HEADER_FILES})
        set_property(TARGET ${CEGUI_TARGET_NAME}_Static APPEND PROPERTY COMPILE_DEFINITIONS CEGUI_STATIC)

        # append the _d (or whatever) for debug builds as needed.
        if (CEGUI_HAS_BUILD_SUFFIX AND CEGUI_BUILD_SUFFIX)
            set_target_properties(${CEGUI_TARGET_NAME}_Static PROPERTIES
                OUTPUT_NAME_DEBUG "${CEGUI_TARGET_NAME}_Static${CEGUI_BUILD_SUFFIX}"
            )
        endif()

        # This works around an issue in the prebuilt deps and can be removed once those are fixed :)
        if (WIN32)
            set_target_properties(${CEGUI_TARGET_NAME}_Static PROPERTIES LINK_FLAGS_DEBUG /NODEFAULTLIB:freeglut.lib)
        endif()

        if (APPLE)
            cegui_apple_app_setup(${CEGUI_TARGET_NAME}_Static TRUE)
        endif()
    endif()

    ###########################################################################
    #                   Dynamically Linked Executable
    ###########################################################################
    add_executable(${CEGUI_TARGET_NAME} ${CORE_SOURCE_FILES} ${CORE_HEADER_FILES})

    # append the _d (or whatever) for debug builds as needed.
    if (CEGUI_HAS_BUILD_SUFFIX AND CEGUI_BUILD_SUFFIX)
        set_target_properties(${CEGUI_TARGET_NAME} PROPERTIES
            OUTPUT_NAME_DEBUG "${CEGUI_TARGET_NAME}${CEGUI_BUILD_SUFFIX}"
        )
    endif()

    if (NOT APPLE)
        set_target_properties(${CEGUI_TARGET_NAME} PROPERTIES
            INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/${CEGUI_LIB_INSTALL_DIR}"
        )
    endif()

    if (APPLE)
        # we add these to ensure the dynamically loaded modules we will be
        # using are built before the .app's post build step that copies
        # libs into the app bundle.
        add_dependencies(${CEGUI_TARGET_NAME} ${CEGUI_FALAGARD_WR_LIBNAME}
                                              CEGUI${CEGUI_OPTION_DEFAULT_XMLPARSER}
                                              CEGUI${CEGUI_OPTION_DEFAULT_IMAGECODEC}
        )

        cegui_apple_app_setup(${CEGUI_TARGET_NAME} FALSE)
    endif()

    
    ###########################################################################
    #                      LIBRARY LINK SETUP
    ###########################################################################
    cegui_target_link_libraries(${CEGUI_TARGET_NAME}
        ${CEGUI_BASE_LIBNAME}
        ${CEGUI_SAMPLEHELPER_LIBNAME}
    )

    if (CEGUI_BUILD_STATIC_CONFIGURATION)
        target_link_libraries(${CEGUI_TARGET_NAME}_Static
            ${CEGUI_STATIC_XMLPARSER_MODULE}_Static
            ${CEGUI_STATIC_IMAGECODEC_MODULE}_Static
            ${CEGUI_FALAGARD_WR_LIBNAME}_Static
        )
    endif()

    ###########################################################################
    #                           INSTALLATION
    ###########################################################################
    if (UNIX AND NOT APPLE AND NOT WIN32)
        install(TARGETS ${CEGUI_TARGET_NAME}
            RUNTIME DESTINATION bin
            LIBRARY DESTINATION ${CEGUI_LIB_INSTALL_DIR}
            ARCHIVE DESTINATION ${CEGUI_LIB_INSTALL_DIR}
    )

    if (CEGUI_BUILD_STATIC_CONFIGURATION)
            install(TARGETS ${CEGUI_TARGET_NAME}_Static
                RUNTIME DESTINATION bin
                LIBRARY DESTINATION ${CEGUI_LIB_INSTALL_DIR}
                ARCHIVE DESTINATION ${CEGUI_LIB_INSTALL_DIR}
        )
        endif()
    endif()

endmacro()

#
# Define a PyCEGUI* extension module
#
macro( cegui_add_python_module PYTHON_MODULE_NAME SOURCE_DIR EXTRA_LIBS )
    file( GLOB ${PYTHON_MODULE_NAME}_SOURCE_FILES ${SOURCE_DIR}/*.cpp )

    include_directories(BEFORE ${SOURCE_DIR})

    add_library(${PYTHON_MODULE_NAME} MODULE ${${PYTHON_MODULE_NAME}_SOURCE_FILES})
    target_link_libraries(${PYTHON_MODULE_NAME} ${CEGUI_BASE_LIBNAME} ${Boost_LIBRARIES} ${PYTHON_LIBRARIES} ${EXTRA_LIBS} )
    set_target_properties(${PYTHON_MODULE_NAME} PROPERTIES PREFIX "")

    # link the default modules when doing a static build.
    if (NOT BUILD_SHARED_LIBS)
        target_link_libraries(${PYTHON_MODULE_NAME}
            ${CEGUI_STATIC_XMLPARSER_MODULE}
            ${CEGUI_STATIC_IMAGECODEC_MODULE}
            ${CEGUI_FALAGARD_WR_LIBNAME}
        )
    endif()

    if (NOT APPLE)
        set_target_properties(${PYTHON_MODULE_NAME} PROPERTIES
            INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/${CEGUI_LIB_INSTALL_DIR}"
        )
    endif()

    if (CMAKE_COMPILER_IS_GNUCXX)
        set_target_properties(${PYTHON_MODULE_NAME} PROPERTIES COMPILE_FLAGS "-fvisibility=hidden")
    endif()

    if (UNIX AND NOT APPLE AND NOT WIN32)
        install(TARGETS ${PYTHON_MODULE_NAME} LIBRARY DESTINATION "${CEGUI_PYTHON_INSTALL_DIR}")
    endif()
endmacro()

