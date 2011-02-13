set( CEGUI_TARGET_NAME ${CEGUI_LUA_SCRIPTMODULE_LIBNAME} )

cegui_gather_files()
cegui_add_library(TRUE TRUE)
cegui_add_dependency(TOLUAPP)
cegui_add_dependency(LUA51)

target_link_libraries(${CEGUI_TARGET_NAME} ${CEGUI_BASE_LIBNAME})

