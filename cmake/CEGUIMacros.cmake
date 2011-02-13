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
        VERSION ${CEGUI_ABI_VERSION}
        SOVERSION ${CEGUI_ABI_CURRENT}
        DEFINE_SYMBOL ${_CEGUI_EXPORT_DEFINE}_EXPORTS
    )
    
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

    add_executable(${CEGUI_TARGET_NAME} ${CORE_SOURCE_FILES})
    set_target_properties(${CEGUI_TARGET_NAME} PROPERTIES VERSION ${CEGUI_VERSION})

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

