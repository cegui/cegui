set (CEGUI_TARGET_NAME ${CEGUI_OGRE_RENDERER_LIBNAME})

cegui_gather_files()
cegui_add_library(TRUE TRUE)
cegui_add_dependency(OGRE)

target_link_libraries(${CEGUI_TARGET_NAME} ${CEGUI_BASE_LIBNAME})

