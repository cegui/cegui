#if (CEGUI_BUILD_RENDERER_OPENGLES)
#    list (APPEND SAMPLES_SOURCES GLESTest.cpp)
#
#    cegui_target_link_libraries(${CEGUI_TARGET_NAME} ${CEGUI_OPENGLES_RENDERER_LIBNAME})
#    cegui_add_dependency(${CEGUI_TARGET_NAME} OPENGLES)
#endif ()