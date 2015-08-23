cegui_add_sample(CEGUIGLESTest)

cegui_target_link_libraries(${CEGUI_TARGET_NAME} ${CEGUI_OPENGLES_RENDERER_LIBNAME})
cegui_add_dependency(${CEGUI_TARGET_NAME} OPENGLES)
