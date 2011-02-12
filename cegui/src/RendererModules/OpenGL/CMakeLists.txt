set (CEGUI_TARGET_NAME ${CEGUI_OPENGL_RENDERER_LIBNAME})

cegui_gather_files()

if (CEGUI_BUILD_EMBEDDED_GLEW)
    list (APPEND CORE_SOURCE_FILES
        GLEW/src/glew.c
    )
    include_directories("./GLEW")
    add_definitions(-DGLEW_STATIC)
endif()

if (NOT APPLE)
    list (REMOVE_ITEM CORE_SOURCE_FILES CEGUIOpenGLApplePBTextureTarget.cpp)
    list (REMOVE_ITEM CORE_HEADER_FILES ${CMAKE_SOURCE_DIR}/cegui/include/RendererModules/OpenGL/CEGUIOpenGLApplePBTextureTarget.h)
endif()

if (NOT WIN32)
    list (REMOVE_ITEM CORE_SOURCE_FILES CEGUIOpenGLWGLPBTextureTarget.cpp)
    list (REMOVE_ITEM CORE_HEADER_FILES ${CMAKE_SOURCE_DIR}/cegui/include/RendererModules/OpenGL/CEGUIOpenGLWGLPBTextureTarget.h)
endif()

if (NOT UNIX OR APPLE)
    list (REMOVE_ITEM CORE_SOURCE_FILES CEGUIOpenGLGLXPBTextureTarget.cpp)
    list (REMOVE_ITEM CORE_HEADER_FILES ${CMAKE_SOURCE_DIR}/cegui/include/RendererModules/OpenGL/CEGUIOpenGLGLXPBTextureTarget.h)
endif()

cegui_add_library(TRUE TRUE)
cegui_add_dependency(OPENGL)

if (NOT CEGUI_BUILD_EMBEDDED_GLEW)
    cegui_add_dependency(GLEW)
endif()

target_link_libraries(${CEGUI_TARGET_NAME} ${CEGUI_BASE_LIBNAME})

