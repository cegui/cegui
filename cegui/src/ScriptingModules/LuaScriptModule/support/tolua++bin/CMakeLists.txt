set( CEGUI_TARGET_NAME tolua++cegui$)

set( CORE_SOURCE_FILES
    toluabind.c
    tolua.c
)

add_executable(${CEGUI_TARGET_NAME} ${CORE_SOURCE_FILES})

set_target_properties(${CEGUI_TARGET_NAME} PROPERTIES VERSION ${CEGUI_VERSION})

if (CEGUI_HAS_BUILD_SUFFIX AND CEGUI_BUILD_SUFFIX)
    set_target_properties(${CEGUI_TARGET_NAME} PROPERTIES
        OUTPUT_NAME_DEBUG "${CEGUI_TARGET_NAME}${CEGUI_BUILD_SUFFIX}"
    )
endif()

cegui_add_dependency(TOLUAPP)
cegui_add_dependency(LUA51)

if (UNIX AND NOT APPLE AND NOT WIN32)
    install(TARGETS ${CEGUI_TARGET_NAME}
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
    )
endif()

