set (CEGUI_TARGET_NAME ${CEGUI_RAPIDXML_PARSER_LIBNAME})

cegui_gather_files()
cegui_add_library(TRUE FALSE)
include_directories(${RAPIDXML_INCLUDE_DIR})

target_link_libraries(${CEGUI_TARGET_NAME} ${CEGUI_BASE_LIBNAME})

