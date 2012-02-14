set (CEGUI_TARGET_NAME ${CEGUI_OPENGL3_RENDERER_LIBNAME})

cegui_gather_files()

cegui_add_library(${CEGUI_TARGET_NAME} CORE_SOURCE_FILES CORE_HEADER_FILES)
cegui_add_dependency(${CEGUI_TARGET_NAME} OPENGL)
cegui_add_dependency(${CEGUI_TARGET_NAME} GLM)

cegui_add_dependency(${CEGUI_TARGET_NAME} GLEW)

cegui_target_link_libraries(${CEGUI_TARGET_NAME} ${CEGUI_BASE_LIBNAME})
