set (CEGUI_TARGET_NAME ${CEGUI_NULL_RENDERER_LIBNAME})

cegui_gather_files()
cegui_add_library(TRUE TRUE)

target_link_libraries(${CEGUI_TARGET_NAME} ${CEGUI_BASE_LIBNAME})

