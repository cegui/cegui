set (CEGUI_TARGET_NAME ${CEGUI_OPENGLES_RENDERER_LIBNAME})

cegui_gather_files()

#if (CEGUI_BUILD_EMBEDDED_GLEW)
#    list (APPEND CORE_SOURCE_FILES
#        GLEW/src/glew.c
#    )
#    include_directories("./GLEW")
#    add_definitions(-DGLEW_STATIC)
#endif()

cegui_add_library(${CEGUI_TARGET_NAME} CORE_SOURCE_FILES CORE_HEADER_FILES)
cegui_add_dependency(${CEGUI_TARGET_NAME} OPENGLES)

#if (NOT CEGUI_BUILD_EMBEDDED_GLEW)
#    cegui_add_dependency(${CEGUI_TARGET_NAME} GLEW)
#endif()

cegui_target_link_libraries(${CEGUI_TARGET_NAME} ${CEGUI_BASE_LIBNAME})

