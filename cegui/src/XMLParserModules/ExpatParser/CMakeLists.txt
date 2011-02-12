set (CEGUI_TARGET_NAME ${CEGUI_EXPAT_PARSER_LIBNAME})

cegui_gather_files()
cegui_add_library(TRUE FALSE)
cegui_add_dependency(EXPAT)

target_link_libraries(${CEGUI_TARGET_NAME} ${CEGUI_BASE_LIBNAME})

if (WIN32)
    add_definitions(-DXML_STATIC)
endif()

