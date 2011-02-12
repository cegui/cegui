set (CEGUI_TARGET_NAME ${CEGUI_LIBXML2_PARSER_LIBNAME})

cegui_gather_files()
cegui_add_library(TRUE FALSE)
cegui_add_dependency(LIBXML2)

target_link_libraries(${CEGUI_TARGET_NAME} ${CEGUI_BASE_LIBNAME})

