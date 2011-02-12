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

add_library(${CEGUI_TARGET_NAME} ${CORE_SOURCE_FILES})

set_target_properties(${CEGUI_TARGET_NAME} PROPERTIES
    VERSION ${CEGUI_ABI_VERSION}
    SOVERSION ${CEGUI_ABI_CURRENT}
)

if (BUILD_SHARED_LIBS)
    set_target_properties(${CEGUI_TARGET_NAME} PROPERTIES
        DEFINE_SYMBOL TOLUA_EXPORTS
    )
else()
    add_definitions(-DTOLUA_STATIC)
endif()

include_directories(${CMAKE_SOURCE_DIR}/cegui/include/ScriptingModules/LuaScriptModule/support/tolua++)

cegui_add_dependency(LUA51)

if (UNIX AND NOT APPLE AND NOT WIN32)
    install(TARGETS ${CEGUI_TARGET_NAME}
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
    )

    install(FILES ${CORE_HEADER_FILES} DESTINATION include/${CMAKE_PROJECT_NAME}/ScriptingModules/LuaScriptModule/support/tolua++)
endif()

