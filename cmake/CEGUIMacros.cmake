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
# Define a cegui library to be built and optionally installed
#
macro (cegui_add_library INSTALL_BIN INSTALL_HEADERS)
    file (RELATIVE_PATH _REL_SRC_DIR "${CMAKE_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}")
    string (REPLACE src include _REL_INC_DIR ${_REL_SRC_DIR})

    string(TOUPPER ${CEGUI_TARGET_NAME} _CEGUI_EXPORT_DEFINE)

    add_library(${CEGUI_TARGET_NAME} ${CEGUI_LIBRARY_TYPE} ${CORE_SOURCE_FILES} ${CORE_HEADER_FILES})

    set_target_properties(${CEGUI_TARGET_NAME} PROPERTIES
        LINK_INTERFACE_LIBRARIES ""
    )

    if (APPLE)
        set_target_properties(${CEGUI_TARGET_NAME} PROPERTIES
            INSTALL_NAME_DIR ${CEGUI_APPLE_DYLIB_INSTALL_PATH}
            BUILD_WITH_INSTALL_RPATH TRUE
        )
    endif()

    if (NOT APPLE OR CEGUI_APPLE_DYLIB_SET_VERSION_INFO)
        set_target_properties(${CEGUI_TARGET_NAME} PROPERTIES
            VERSION ${CEGUI_ABI_VERSION}
            SOVERSION ${CEGUI_ABI_CURRENT}
            DEFINE_SYMBOL ${_CEGUI_EXPORT_DEFINE}_EXPORTS
        )
    endif()

    if (CEGUI_LIBRARY_EXTRA_BUILD_FLAGS)
        set_target_properties(${CEGUI_TARGET_NAME} PROPERTIES
            COMPILE_FLAGS ${CEGUI_LIBRARY_EXTRA_BUILD_FLAGS}
        )
    endif()

    include_directories("${CMAKE_SOURCE_DIR}/${_REL_INC_DIR}")

    if (${INSTALL_BIN} AND UNIX AND NOT APPLE AND NOT WIN32)
        install(TARGETS ${CEGUI_TARGET_NAME}
            RUNTIME DESTINATION bin
            LIBRARY DESTINATION lib
            ARCHIVE DESTINATION lib
        )
    endif()

    if (${INSTALL_HEADERS} AND UNIX AND NOT APPLE AND NOT WIN32)
        string (REPLACE "cegui/src" "" _REL_HEADER_DIR ${_REL_SRC_DIR})
        install(FILES ${CORE_HEADER_FILES} DESTINATION include/${CMAKE_PROJECT_NAME}/${_REL_HEADER_DIR})
    endif()
endmacro()

#
# Define a CEGUI sample app
#
macro (cegui_add_sample _NAME)
    set (CEGUI_TARGET_NAME ${_NAME})

    cegui_gather_files()

    if (CEGUI_SAMPLES_USE_DIRECT3D9 OR CEGUI_SAMPLES_USE_DIRECT3D10)
        link_directories(${DIRECTXSDK_LIBRARY_DIR})
    endif()

    add_executable(${CEGUI_TARGET_NAME} ${CORE_SOURCE_FILES} ${CORE_HEADER_FILES})

    # append the _d (or whatever) for debug builds as needed.
    if (CEGUI_HAS_BUILD_SUFFIX AND CEGUI_BUILD_SUFFIX)
        set_target_properties(${CEGUI_TARGET_NAME} PROPERTIES
            OUTPUT_NAME_DEBUG "${CEGUI_TARGET_NAME}${CEGUI_BUILD_SUFFIX}"
        )
    endif()

    if (APPLE)
        set_target_properties(${CEGUI_TARGET_NAME} PROPERTIES
            MACOSX_BUNDLE TRUE
        )
    endif()

    include_directories(${CMAKE_SOURCE_DIR}/Samples/common/include)

    target_link_libraries(${CEGUI_TARGET_NAME}
        ${CEGUI_BASE_LIBNAME}
        ${CEGUI_SAMPLEHELPER_LIBNAME}
    )

    if (NOT BUILD_SHARED_LIBS)
        target_link_libraries(${CEGUI_TARGET_NAME}
            "CEGUI${CEGUI_OPTION_DEFAULT_XMLPARSER}"
            "CEGUI${CEGUI_OPTION_DEFAULT_IMAGECODEC}"
            ${CEGUI_FALAGARD_WR_LIBNAME}
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

        add_custom_command(TARGET ${CEGUI_TARGET_NAME} POST_BUILD 
            COMMAND ${_ACTIONCMD} ${CMAKE_PREFIX_PATH}/lib/*.dylib ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${CEGUI_TARGET_NAME}.app/Contents/Frameworks/
            COMMENT "Creating ${_ACTIONMSG} dependency libraries in ${CEGUI_TARGET_NAME}.app")
        add_custom_command(TARGET ${CEGUI_TARGET_NAME} POST_BUILD 
            COMMAND ${_ACTIONCMD} ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/*.{dylib,framework} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${CEGUI_TARGET_NAME}.app/Contents/Frameworks/
            COMMENT "Creating ${_ACTIONMSG} built cegui libraries and frameworks in ${CEGUI_TARGET_NAME}.app")
        add_custom_command(TARGET ${CEGUI_TARGET_NAME} POST_BUILD 
            COMMAND ${_ACTIONCMD} ${CMAKE_SOURCE_DIR}/datafiles ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${CEGUI_TARGET_NAME}.app/Contents/Resources/
            COMMENT "Creating ${_ACTIONMSG} sample datafiles ${CEGUI_TARGET_NAME}.app")
    endif()

    if (UNIX AND NOT APPLE AND NOT WIN32)
        install(TARGETS ${CEGUI_TARGET_NAME}
            RUNTIME DESTINATION bin
            LIBRARY DESTINATION lib
            ARCHIVE DESTINATION lib
    )
    endif()

endmacro()

#
# add a dependency to the current target
#
macro (cegui_add_dependency _DEPNAME)
    include_directories(${${_DEPNAME}_INCLUDE_DIR})
    if (${_DEPNAME}_LIBRARIES_DBG)
        target_link_libraries(${CEGUI_TARGET_NAME} optimized ${${_DEPNAME}_LIBRARIES})
        target_link_libraries(${CEGUI_TARGET_NAME} debug ${${_DEPNAME}_LIBRARIES_DBG})
    else()
        target_link_libraries(${CEGUI_TARGET_NAME} ${${_DEPNAME}_LIBRARIES})
    endif()
endmacro()

