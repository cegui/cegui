set (CEGUI_TARGET_NAME ${CEGUI_TINYXML_PARSER_LIBNAME})

cegui_gather_files()

if (CEGUI_BUILD_EMBEDDED_TINYXML)
    file (GLOB TINYXML_SOURCE_FILES ceguitinyxml/*.cpp)
    list (APPEND CORE_SOURCE_FILES ${TINYXML_SOURCE_FILES})
endif()

cegui_add_library(TRUE FALSE)

if (NOT CEGUI_BUILD_EMBEDDED_TINYXML)
    cegui_add_dependency(TINYXML)
endif()

target_link_libraries(${CEGUI_TARGET_NAME} ${CEGUI_BASE_LIBNAME})

