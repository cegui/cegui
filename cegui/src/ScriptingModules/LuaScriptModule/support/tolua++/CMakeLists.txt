set( CEGUI_TARGET_NAME ${CEGUI_TOLUAPP_LIBNAME} )

set( CORE_HEADER_FILES
    ${CMAKE_SOURCE_DIR}/cegui/include/ScriptingModules/LuaScriptModule/support/tolua++/tolua++.h
)

set( CORE_SOURCE_FILES
    tolua_event.c
    tolua_is.c
    tolua_map.c
    tolua_push.c
    tolua_to.c
)

include_directories(${CMAKE_SOURCE_DIR}/cegui/include/ScriptingModules/LuaScriptModule/support/tolua++)

add_library(${CEGUI_TARGET_NAME} ${CORE_SOURCE_FILES})
set_target_properties(${CEGUI_TARGET_NAME} PROPERTIES
    DEFINE_SYMBOL TOLUA_EXPORTS
    VERSION ${CEGUI_ABI_VERSION}
    SOVERSION ${CEGUI_ABI_CURRENT}
)

if (CEGUI_BUILD_STATIC_CONFIGURATION)
    add_library(${CEGUI_TARGET_NAME}_Static ${CORE_SOURCE_FILES})
    set_target_properties(${CEGUI_TARGET_NAME}_Static PROPERTIES DEFINE_SYMBOL TOLUA_STATIC)
endif()

cegui_add_dependency(${CEGUI_TARGET_NAME} LUA51)

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

    install(FILES ${CORE_HEADER_FILES} DESTINATION include/${CMAKE_PROJECT_NAME}/ScriptingModules/LuaScriptModule/support/tolua++)
endif()

