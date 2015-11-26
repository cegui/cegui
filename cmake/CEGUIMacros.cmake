################################################################################
# Macro definitions used by CEGUI's cmake build
################################################################################
include(FindPackageHandleStandardArgs)

#
# gather *.cpp and .h files for the current location
#
macro (cegui_gather_files)
    file (RELATIVE_PATH _REL_SRC_DIR "${CMAKE_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}")
    string (REPLACE src include/CEGUI _REL_INC_DIR ${_REL_SRC_DIR})
    set (_CURR_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/${_REL_INC_DIR}")

    file (GLOB CORE_SOURCE_FILES RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" *.cpp)
    file (GLOB CORE_HEADER_FILES RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "${_CURR_INCLUDE_DIR}/*.h" "${CMAKE_CURRENT_SOURCE_DIR}" *.inl)
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
        elseif (${_DEP_NAME}_LIBRARIES_DBG)
                target_link_libraries(${_TARGET_NAME} ${${_DEP_NAME}_LIBRARIES_DBG})
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
        elseif (${_DEP_NAME}_LIBRARIES_STATIC_DBG)
            target_link_libraries(${_TARGET_NAME} ${${_DEP_NAME}_LIBRARIES_STATIC_DBG})
        else()
            cegui_add_dependency_dynamic_libs(${_TARGET_NAME} ${_DEP_NAME})
        endif()
    endif()
endmacro()

#
# add a dependency to a target (and it's static equivalent, if it exists).
#
# An optional "SCOPE" 3rd argument can be specified to determine the "scope"
# argument passed to "target_include_directories". This argument can be one of
# "INTERFACE", "PUBLIC" and "PRIVATE", the default being "PRIVATE". In general,
# u should use "PUBLIC" if every target that depends on "_TARGET_NAME" should also
# compile with "_DEP_NAME"'s include directories. Please refer to the
# documentation of "target_include_directories" for more details.
#
# An optional "IS_SYSTEM" 4th argument can be specified to determine whether to
# treat the headers of the dependency as system headers. This usually means that
# the compiler won't generate warnings for these headers. The default is
# "FALSE".
#
macro (cegui_add_dependency _TARGET_NAME _DEP_NAME)
# Optional additional arguments: "SCOPE" "IS_SYSTEM"
    get_target_property(_DYNAMIC_EXISTS ${_TARGET_NAME} TYPE)
    get_target_property(_STATIC_EXISTS ${_TARGET_NAME}_Static TYPE)
    if ("${ARGC}" GREATER 2)
        if (("${ARGC}" GREATER 3) AND "${ARGV3}")
            if (_DYNAMIC_EXISTS)
                target_include_directories(${_TARGET_NAME} SYSTEM "${ARGV2}" ${${_DEP_NAME}_INCLUDE_DIR})
            endif()
            if (_STATIC_EXISTS)
                target_include_directories(${_TARGET_NAME}_Static SYSTEM "${ARGV2}" ${${_DEP_NAME}_INCLUDE_DIR})
            endif()
        else ()
            if (_DYNAMIC_EXISTS)
                target_include_directories(${_TARGET_NAME} "${ARGV2}" ${${_DEP_NAME}_INCLUDE_DIR})
            endif()
            if (_STATIC_EXISTS)
                target_include_directories(${_TARGET_NAME}_Static "${ARGV2}" ${${_DEP_NAME}_INCLUDE_DIR})
            endif()
        endif ()
    else ()
        if (_DYNAMIC_EXISTS)
            target_include_directories(${_TARGET_NAME} PRIVATE ${${_DEP_NAME}_INCLUDE_DIR})
        endif()
        if (_STATIC_EXISTS)
            target_include_directories(${_TARGET_NAME}_Static PRIVATE ${${_DEP_NAME}_INCLUDE_DIR})
        endif()
    endif ()

    ###########################################################################
    #                    NON-STATIC VERSION OF TARGET
    ###########################################################################
    if (${_DEP_NAME}_DEFINITIONS)
        set_property( TARGET ${_TARGET_NAME} APPEND PROPERTY COMPILE_DEFINITIONS ${${_DEP_NAME}_DEFINITIONS} )
    endif()

    if (CEGUI_BUILD_SHARED_LIBS_WITH_STATIC_DEPENDENCIES)
        if (${_DEP_NAME}_DEFINITIONS_STATIC)
            set_property( TARGET ${_TARGET_NAME} APPEND PROPERTY COMPILE_DEFINITIONS ${${_DEP_NAME}_DEFINITIONS_STATIC} )
        endif()

        cegui_add_dependency_static_libs(${_TARGET_NAME} ${_DEP_NAME})
    else()
        if (${_DEP_NAME}_DEFINITIONS_DYNAMIC)
            set_property( TARGET ${_TARGET_NAME} APPEND PROPERTY COMPILE_DEFINITIONS ${${_DEP_NAME}_DEFINITIONS_DYNAMIC} )
        endif()

        cegui_add_dependency_dynamic_libs(${_TARGET_NAME} ${_DEP_NAME})
    endif()


    ###########################################################################
    #    ADD DEPENDENCY DEFS TO STATIC VERSION OF TARGET (if it exists)
    ###########################################################################
    if (_STATIC_EXISTS)
        if (${_DEP_NAME}_DEFINITIONS)
            set_property( TARGET ${_TARGET_NAME}_Static APPEND PROPERTY COMPILE_DEFINITIONS ${${_DEP_NAME}_DEFINITIONS} )
        endif()

        if (${_DEP_NAME}_DEFINITIONS_STATIC)
            set_property( TARGET ${_TARGET_NAME}_Static APPEND PROPERTY COMPILE_DEFINITIONS ${${_DEP_NAME}_DEFINITIONS_STATIC} )
        endif()

        cegui_add_dependency_static_libs(${_TARGET_NAME}_Static ${_DEP_NAME})
    endif()

endmacro()

###############################################################################
#
# Define a CEGUI library to be built and optionally installed.
#
# Note: this creates a regular lib (or module) and a static lib with the
#       _Static suffix.
#
# Parameters:
#       _LIB_NAME           - Name of the library to create.
#       _IS_MODULE          - Specifies whether to create a module or shared lib
#       _SOURCE_FILES_VAR   - Variable holding the source file names.
#       _HEADER_FILES_VAR   - Variable holding the header file names.
#       _INSTALL_BIN        - For linux, TRUE if the lib should be installed
#       _INSTALL_HEADERS    - For linux, TRUE if the headers should be installed
#
###############################################################################
macro (cegui_add_library_impl _LIB_NAME _IS_MODULE _SOURCE_FILES_VAR _HEADER_FILES_VAR _INSTALL_BIN _INSTALL_HEADERS)
    file (RELATIVE_PATH _REL_SRC_DIR "${CMAKE_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}")
    string (REPLACE src include _REL_INC_DIR ${_REL_SRC_DIR})
    string(REGEX MATCH "^[^-]*" _CEGUI_EXPORT_BASE ${_LIB_NAME})
    string(TOUPPER ${_CEGUI_EXPORT_BASE}_EXPORTS _CEGUI_EXPORT_DEFINE)

    include_directories("${CMAKE_SOURCE_DIR}/${_REL_INC_DIR}")

    set (_LIB_TYPE SHARED)

    ###########################################################################
    #                       STATIC LIBRARY SET UP
    ###########################################################################
    if (CEGUI_BUILD_STATIC_CONFIGURATION)
        add_library(${_LIB_NAME}_Static STATIC ${${_SOURCE_FILES_VAR}} ${${_HEADER_FILES_VAR}})
        set_property(TARGET ${_LIB_NAME}_Static APPEND PROPERTY COMPILE_DEFINITIONS CEGUI_STATIC)
        if (CEGUI_BUILD_STATIC_FACTORY_MODULE)
            set_property(TARGET ${_LIB_NAME}_Static APPEND PROPERTY COMPILE_DEFINITIONS CEGUI_BUILD_STATIC_FACTORY_MODULE)
        endif()
    endif()

    ###########################################################################
    #                       SHARED LIBRARY SET UP
    ###########################################################################
    add_library(${_LIB_NAME} ${_LIB_TYPE} ${${_SOURCE_FILES_VAR}} ${${_HEADER_FILES_VAR}})
    set_target_properties(${_LIB_NAME} PROPERTIES DEFINE_SYMBOL ${_CEGUI_EXPORT_DEFINE})

    if (NOT CEGUI_BUILD_SHARED_LIBS_WITH_STATIC_DEPENDENCIES)
        # Starting with CMake 2.8.12 LINK_INTERFACE_LIBRARIES was renamed to INTERFACE_LINK_LIBRARIES
        if (${CMAKE_VERSION} VERSION_GREATER 2.8.12 OR ${CMAKE_VERSION} VERSION_EQUAL 2.8.12)
            set_target_properties(${_LIB_NAME} PROPERTIES
                INTERFACE_LINK_LIBRARIES ""
            )
        else()
            set_target_properties(${_LIB_NAME} PROPERTIES
                LINK_INTERFACE_LIBRARIES ""
            )
        endif()
    endif()

    if (APPLE)
        set_target_properties(${_LIB_NAME} PROPERTIES
            INSTALL_NAME_DIR ${CEGUI_APPLE_DYLIB_INSTALL_PATH}
            BUILD_WITH_INSTALL_RPATH TRUE
        )

        # Force the somewhat standard .dylib extension for modules over the use of
        # the .so extension
        if (${_IS_MODULE})
            set_target_properties(${_LIB_NAME} PROPERTIES SUFFIX ".dylib")
        endif()

    elseif(CEGUI_INSTALL_WITH_RPATH)
        set_target_properties(${_LIB_NAME} PROPERTIES
            INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/${CEGUI_LIB_INSTALL_DIR}"
        )
    endif()

    # Do not version modules, since we dlopen these directly and need to know
    # the name is what we think it will be (and not rely on symlinks which will
    # not be installed always, but usually only as part of *-dev packages).
    if (NOT ${_IS_MODULE} AND NOT ANDROID)
        if (NOT APPLE OR CEGUI_APPLE_DYLIB_SET_VERSION_INFO)
            set_target_properties(${_LIB_NAME} PROPERTIES
                VERSION ${CEGUI_ABI_VERSION}
                SOVERSION ${CEGUI_SOVERSION}
            )
        endif()
    endif()

    ###########################################################################
    #                           INSTALLATION
    ###########################################################################
    if (${_INSTALL_BIN})
        if (${_IS_MODULE})
            set(_CEGUI_LIB_DEST ${CEGUI_MODULE_INSTALL_DIR})
        else()
            set(_CEGUI_LIB_DEST ${CEGUI_LIB_INSTALL_DIR})
        endif()

        install(TARGETS ${_LIB_NAME}
            RUNTIME DESTINATION bin
            LIBRARY DESTINATION ${_CEGUI_LIB_DEST}
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

    if (${_INSTALL_HEADERS})
        string (REPLACE "cegui/src/" "" _REL_HEADER_DIR ${_REL_SRC_DIR})
        install(FILES ${${_HEADER_FILES_VAR}} DESTINATION "${CEGUI_INCLUDE_INSTALL_DIR}/CEGUI/${_REL_HEADER_DIR}")
    endif()
endmacro()

#
# Define loadable module - this is loaded dynamically at runtime and not linked
# to (except in static builds)
#
macro (cegui_add_loadable_module _MODULE_NAME _SOURCE_FILES_VAR _HEADER_FILES_VAR)
    cegui_add_library_impl(${_MODULE_NAME} TRUE ${_SOURCE_FILES_VAR} ${_HEADER_FILES_VAR} TRUE TRUE)
endmacro()

#
# Define a regular library - this is usually linked to directly.
#
macro (cegui_add_library _LIB_NAME _SOURCE_FILES_VAR _HEADER_FILES_VAR)
    cegui_add_library_impl(${_LIB_NAME} FALSE ${_SOURCE_FILES_VAR} ${_HEADER_FILES_VAR} TRUE TRUE)
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
        set (_ACTIONCMD rsync -r)
        set (_ACTIONMSG "copies of")
    endif()

    # depending on the generator used, we need to examine different things to discover the build type
    if (CMAKE_GENERATOR STREQUAL "Unix Makefiles")
        set(_CEGUI_BUILD_CONFIG ${CMAKE_BUILD_TYPE})
    else()
        set(_CEGUI_BUILD_CONFIG "$(CONFIGURATION)")
    endif()

    add_custom_command(TARGET ${_TARGET_NAME} POST_BUILD 
        COMMAND if test x${_CEGUI_BUILD_CONFIG} == xDebug \; then ce_ext=\"${CEGUI_BUILD_SUFFIX}\"\; else ce_ext=\"\"\; fi\; rm -rf \"${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${_TARGET_NAME}$$ce_ext.app/Contents/Frameworks\"
        COMMENT "Removing old Frameworks in ${_TARGET_NAME}.app")
    add_custom_command(TARGET ${_TARGET_NAME} POST_BUILD 
        COMMAND if test x${_CEGUI_BUILD_CONFIG} == xDebug \; then ce_ext=\"${CEGUI_BUILD_SUFFIX}\"\; else ce_ext=\"\"\; fi\; rm -rf \"${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${_TARGET_NAME}$$ce_ext.app/Contents/Resources\"
        COMMENT "Removing old Resources in ${_TARGET_NAME}.app")

    add_custom_command(TARGET ${_TARGET_NAME} POST_BUILD 
        COMMAND if test x${_CEGUI_BUILD_CONFIG} == xDebug \; then ce_ext=\"${CEGUI_BUILD_SUFFIX}\"\; else ce_ext=\"\"\; fi\; mkdir -p \"${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${_TARGET_NAME}$$ce_ext.app/Contents/Frameworks\"
        COMMENT "Creating Frameworks directory ${_TARGET_NAME}.app")
    add_custom_command(TARGET ${_TARGET_NAME} POST_BUILD 
        COMMAND if test x${_CEGUI_BUILD_CONFIG} == xDebug \; then ce_ext=\"${CEGUI_BUILD_SUFFIX}\"\; else ce_ext=\"\"\; fi\; mkdir -p \"${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${_TARGET_NAME}$$ce_ext.app/Contents/Resources\"
        COMMENT "Creating Resources directory ${_TARGET_NAME}.app")

    if (NOT ${_STATIC})
        if (NOT CEGUI_BUILD_SHARED_LIBS_WITH_STATIC_DEPENDENCIES)
            add_custom_command(TARGET ${_TARGET_NAME} POST_BUILD 
                COMMAND if test x${_CEGUI_BUILD_CONFIG} == xDebug \; then ce_ext=\"${CEGUI_BUILD_SUFFIX}\"\; ce_libglob=\"*${CEGUI_BUILD_SUFFIX}.dylib\"\; else ce_ext=\"\"\; shopt -s extglob\; ce_libglob=\"!\(*${CEGUI_BUILD_SUFFIX}\).dylib\"\; fi\; ${_ACTIONCMD} ${CMAKE_PREFIX_PATH}/lib/dynamic/$$ce_libglob \"${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${_TARGET_NAME}$$ce_ext.app/Contents/Frameworks/\"
                COMMENT "Creating ${_ACTIONMSG} dependency libraries in ${_TARGET_NAME}.app")
        endif()

        add_custom_command(TARGET ${_TARGET_NAME} POST_BUILD 
            COMMAND if test x${_CEGUI_BUILD_CONFIG} == xDebug \; then ce_ext=\"${CEGUI_BUILD_SUFFIX}\"\; ce_libglob=\"*${CEGUI_BUILD_SUFFIX}.dylib\"\; else ce_ext=\"\"\; shopt -s extglob\; ce_libglob=\"!\(*${CEGUI_BUILD_SUFFIX}\).dylib\"\; fi\; ${_ACTIONCMD} ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/$$ce_libglob \"${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${_TARGET_NAME}$$ce_ext.app/Contents/Frameworks/\"
            COMMENT "Creating ${_ACTIONMSG} built cegui libraries in ${_TARGET_NAME}.app")
    endif()

    add_custom_command(TARGET ${_TARGET_NAME} POST_BUILD 
        COMMAND if test x${_CEGUI_BUILD_CONFIG} == xDebug \; then ce_ext=\"${CEGUI_BUILD_SUFFIX}\"\; else ce_ext=\"\"\; fi\; ${_ACTIONCMD} ${CMAKE_SOURCE_DIR}/datafiles \"${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${_TARGET_NAME}$$ce_ext.app/Contents/Resources/\"
        COMMENT "Creating ${_ACTIONMSG} sample datafiles ${_TARGET_NAME}.app")

    # When CEGUI_APPLE_SYMLINK_DEPENDENCIES_TO_SAMPLE_APPS is TRUE, this
    # command symlinks from the build dir back into the source dir, which is
    # not ideal, however under that scenario there is no other option. Do
    # not whine about it, if you don't want that, then set
    # CEGUI_APPLE_SYMLINK_DEPENDENCIES_TO_SAMPLE_APPS to false and the thing
    # will be copied to the output app bundle instead.
    add_custom_command(TARGET ${_TARGET_NAME} POST_BUILD 
        COMMAND if test x${_CEGUI_BUILD_CONFIG} == xDebug \; then ce_ext=\"${CEGUI_BUILD_SUFFIX}\"\; else ce_ext=\"\"\; fi\; ${_ACTIONCMD} ${CMAKE_BINARY_DIR}/datafiles/samples \"${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${_TARGET_NAME}$$ce_ext.app/Contents/Resources/datafiles\"
        COMMENT "Creating ${_ACTIONMSG} samples.xml ${_TARGET_NAME}.app")
endmacro()

#
# Define a CEGUI sample module
#
macro (cegui_add_sample _NAME)
    set (CEGUI_TARGET_NAME ${_NAME})

    cegui_gather_files()

	set(CORE_HEADER_FILES ${CORE_HEADER_FILES}
		${CMAKE_SOURCE_DIR}/samples/common/include/Sample.h
		${CMAKE_SOURCE_DIR}/samples/common/include/SampleBase.h
	)
    
    # Each demo will become a dynamically linked library as plugin (module)
    cegui_add_library_impl(${CEGUI_TARGET_NAME} TRUE CORE_SOURCE_FILES CORE_HEADER_FILES FALSE FALSE)

    # Setup custom install location
    install(TARGETS ${CEGUI_TARGET_NAME}
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION ${CEGUI_SAMPLE_INSTALL_DIR}
        ARCHIVE DESTINATION ${CEGUI_SAMPLE_INSTALL_DIR}
    )

    if (CEGUI_BUILD_STATIC_CONFIGURATION)
        install(TARGETS ${CEGUI_TARGET_NAME}_Static
            RUNTIME DESTINATION bin
            LIBRARY DESTINATION ${CEGUI_SAMPLE_INSTALL_DIR}
            ARCHIVE DESTINATION ${CEGUI_SAMPLE_INSTALL_DIR}
        )
    endif()

    add_dependencies(${CEGUI_SAMPLEFRAMEWORK_EXENAME} ${CEGUI_TARGET_NAME})

    include_directories(${CMAKE_SOURCE_DIR}/samples/common/include)

    #Library link setup
    cegui_target_link_libraries(${CEGUI_TARGET_NAME}
        ${CEGUI_BASE_LIBNAME}
    )

    # Add the MetaData chunk of the sample to the final xml
    file(READ ${CMAKE_CURRENT_SOURCE_DIR}/SampleMetaData.xml DEMO_META_DATA)
    file(APPEND ${CMAKE_BINARY_DIR}/datafiles/samples/samples.xml "${DEMO_META_DATA}")
endmacro()

#
# Define a PyCEGUI* extension module
#
macro( cegui_add_python_module PYTHON_MODULE_NAME SOURCE_DIR EXTRA_LIBS )
    file( GLOB ${PYTHON_MODULE_NAME}_SOURCE_FILES ${SOURCE_DIR}/*.cpp )

    include_directories(BEFORE ${SOURCE_DIR})

    add_library(${PYTHON_MODULE_NAME} MODULE ${${PYTHON_MODULE_NAME}_SOURCE_FILES})
    target_link_libraries(${PYTHON_MODULE_NAME} ${CEGUI_BASE_LIBNAME} ${Boost_PYTHON_LIBRARY} ${PYTHON_LIBRARIES} ${EXTRA_LIBS} )
    set_target_properties(${PYTHON_MODULE_NAME} PROPERTIES PREFIX "")

    if (WIN32)
        set_target_properties(${PYTHON_MODULE_NAME} PROPERTIES SUFFIX ".pyd")
    endif()

    if (NOT APPLE AND CEGUI_INSTALL_WITH_RPATH)
        set_target_properties(${PYTHON_MODULE_NAME} PROPERTIES
            INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/${CEGUI_LIB_INSTALL_DIR}"
        )
    endif()

    if (CMAKE_COMPILER_IS_GNUCXX)
        set_target_properties(${PYTHON_MODULE_NAME} PROPERTIES COMPILE_FLAGS "-fvisibility=hidden")
    endif()

    install(TARGETS ${PYTHON_MODULE_NAME} LIBRARY DESTINATION "${CEGUI_PYTHON_INSTALL_DIR}")
endmacro()

#
# Define a CEGUI test executable
#
macro (cegui_add_test_executable _NAME)
    set (CEGUI_TARGET_NAME ${_NAME}-${CEGUI_VERSION_MAJOR}.${CEGUI_VERSION_MINOR})

    cegui_gather_files()

    include_directories(${Boost_INCLUDE_DIR})

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

    add_definitions(-DBOOST_ALL_NO_LIB)
    # set boost to use dynamic linking
    if (NOT CEGUI_BUILD_STATIC_CONFIGURATION)
        add_definitions( -DBOOST_TEST_DYN_LINK )
    endif()

    if (NOT APPLE AND CEGUI_INSTALL_WITH_RPATH)
        set_target_properties(${CEGUI_TARGET_NAME} PROPERTIES
            INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/${CEGUI_LIB_INSTALL_DIR}"
        )
    endif()

    ###########################################################################
    #                      LIBRARY LINK SETUP
    ###########################################################################
    cegui_target_link_libraries(${CEGUI_TARGET_NAME}
        ${CEGUI_BASE_LIBNAME}
        ${CEGUI_NULL_RENDERER_LIBNAME}
        ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
    )

    if (CEGUI_BUILD_PERFORMANCE_TESTS)
        cegui_target_link_libraries(${CEGUI_TARGET_NAME}
            ${Boost_TIMER_LIBRARY}
            ${Boost_SYSTEM_LIBRARY}
        )
    endif()

    if (CEGUI_BUILD_STATIC_CONFIGURATION)
        target_link_libraries(${CEGUI_TARGET_NAME}_Static
            ${CEGUI_NULL_RENDERER_LIBNAME}_Static
            ${CEGUI_STATIC_XMLPARSER_MODULE}_Static
            ${CEGUI_STATIC_IMAGECODEC_MODULE}_Static
            ${CEGUI_CORE_WR_LIBNAME}_Static
        )
    endif()

    ###########################################################################
    #                           INSTALLATION
    ###########################################################################
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

    add_test(NAME ${_NAME} COMMAND ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${CEGUI_TARGET_NAME})
endmacro()

################################################################################
# Creates a cmake option and initializes it to a default value depending on
# other options.
#
# OPTIONAME: The name of the option to create
# DESC: The option's description
# DEPS: Dependencies of the option, specified as a list of boolean expressions.
#       If one of the dependencies evaluates to FALSE, then the option's
#       default will be set to FALSE, otherwise it will be set to TRUE.
#       Example for DEPS: "NOT FOO;BAR"
################################################################################
macro (cegui_dependent_option OPTIONNAME DESC DEPS)
    set (${OPTIONNAME}_DEFAULT TRUE)
    foreach (DEP ${DEPS})
        string(REGEX REPLACE " +" ";" _DEP "${DEP}")
        # Don't use NOT here, because conditions like 'NOT NOT foo' will break
        if (${_DEP})
        else()
            set (${OPTIONNAME}_DEFAULT FALSE)
        endif()
    endforeach()
    option (${OPTIONNAME} "${DESC}" ${${OPTIONNAME}_DEFAULT})
endmacro()

################################################################################
# Checks a set of variables and determines whether a package was 'found' or not.
#
# This is used to try and handle the plethora of dependency options that are
# created when using the CEGUI dependencies package in combination with cmake
# settings and options used here in the main CEGUI build.
#
# NOTE: On platforms where the dependency pack is not intended to be used, the
# extra library configurations are NOT checked, on those platforms we will use
# the base library only. Also note that on those platforms, if only release
# libraries are available we'll always use them (regardless of
# "CMAKE_CONFIGURATION_TYPES" and "CMAKE_BUILD_TYPE"), and if only debug
# libraries are available we'll always use them.
#
# _PKGNAME: The name of package we're checking for.
# _LIBBASENAMEVAR: name of the library base name variable.  This name will be
#                  used in the construction of other variable names which should
#                  be set for various configurations (eg. BASENAMEVAR_DBG for
#                  dynamic debug config, BASENAMEVAR_STATIC for non debug static)
# Optional other args: all other args will be tested verbatim (they're passed
#                      along to find_package_handle_standard_args).
################################################################################
macro (cegui_find_package_handle_standard_args _PKGNAME _LIBBASENAMEVAR)
    unset(_FPHSA_LIBS)

    if (WIN32 OR APPLE)
        if (CMAKE_CONFIGURATION_TYPES)
            if (CMAKE_CONFIGURATION_TYPES MATCHES "Debug")
                set (_WANT_DBG_LIBS TRUE)
            else()
                unset (_WANT_DBG_LIBS)
            endif()
            if (CMAKE_CONFIGURATION_TYPES MATCHES "Release" OR CMAKE_CONFIGURATION_TYPES MATCHES "Rel[A-Z]")
                set (_WANT_REL_LIBS TRUE)
            else()
                unset (_WANT_REL_LIBS)
            endif()
        elseif (NOT CMAKE_BUILD_TYPE OR NOT CMAKE_BUILD_TYPE STREQUAL Debug)
            set (_WANT_REL_LIBS TRUE)
            unset (_WANT_DBG_LIBS)
        else()
            set (_WANT_DBG_LIBS TRUE)
            unset (_WANT_REL_LIBS)
        endif()

        if (CEGUI_BUILD_SHARED_LIBS_WITH_STATIC_DEPENDENCIES OR CEGUI_BUILD_STATIC_CONFIGURATION)
            if ((_WANT_REL_LIBS AND ${_LIBBASENAMEVAR}_STATIC) OR NOT ${_LIBBASENAMEVAR}_STATIC_DBG)
                list(APPEND _FPHSA_LIBS ${_LIBBASENAMEVAR}_STATIC)
            endif()
            if ((_WANT_DBG_LIBS AND ${_LIBBASENAMEVAR}_STATIC_DBG) OR NOT ${_LIBBASENAMEVAR}_STATIC)
                list(APPEND _FPHSA_LIBS ${_LIBBASENAMEVAR}_STATIC_DBG)
            endif()
        endif ()
        if (NOT CEGUI_BUILD_SHARED_LIBS_WITH_STATIC_DEPENDENCIES)
            if ((_WANT_REL_LIBS AND ${_LIBBASENAMEVAR}) OR NOT ${_LIBBASENAMEVAR}_DBG)
                list(APPEND _FPHSA_LIBS ${_LIBBASENAMEVAR})
            endif()
            if ((_WANT_DBG_LIBS AND ${_LIBBASENAMEVAR}_DBG) OR NOT ${_LIBBASENAMEVAR})
                list(APPEND _FPHSA_LIBS ${_LIBBASENAMEVAR}_DBG)
            endif()
        endif()
    else()
        set(_FPHSA_LIBS ${_LIBBASENAMEVAR})
    endif()

    find_package_handle_standard_args(${_PKGNAME} DEFAULT_MSG ${_FPHSA_LIBS} ${ARGN})
endmacro()

################################################################################
# Declare a library (not loadable module) name
#
# This is used to name things in a consistent way and allow us to change the way
# things are named without editing lots of stuff (and forgetting some things or
# changing some other things that should not be changed)
################################################################################
macro(cegui_set_library_name _VARIABLE _LIBBASENAME)
    set(${_VARIABLE} ${_LIBBASENAME}-${CEGUI_VERSION_MAJOR})
endmacro()

################################################################################
# Declare an executable name
#
# This is used to name things in a consistent way and allow us to change the way
# things are named without editing lots of stuff (and maybe forgetting some)
################################################################################
macro(cegui_set_executable_name _VARIABLE _EXEBASENAME)
    set(${_VARIABLE} ${_EXEBASENAME}-${CEGUI_VERSION_MAJOR}.${CEGUI_VERSION_MINOR})
endmacro()

################################################################################
# Declare a loadable module name
#
# This is used to name things in a consistent way and allow us to change the way
# things are named without editing lots of stuff (and maybe forgetting some)
################################################################################
macro(cegui_set_module_name _VARIABLE _MODULEBASENAME)
    set(${_VARIABLE} ${_MODULEBASENAME})
endmacro()

################################################################################
# Sanity check for Default ImageCodec (i.e. is it going to be built?)
################################################################################
macro(cegui_defaultmodule_sanity_test _DEFAULTVAR _MODNAME _BUILDVAR)
    if (${_DEFAULTVAR} STREQUAL "${_MODNAME}")
        if (NOT ${_BUILDVAR})
            message(SEND_ERROR "${_DEFAULTVAR} is set to ${_MODNAME} but this module is not going to be built (see ${_BUILDVAR})")
        endif()
    endif()
endmacro()
