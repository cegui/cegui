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
    list (REMOVE_ITEM CORE_SOURCE_FILES ApplePBTextureTarget.cpp)
    list (REMOVE_ITEM CORE_HEADER_FILES ${CMAKE_SOURCE_DIR}/cegui/include/CEGUI/RendererModules/OpenGL/ApplePBTextureTarget.h)
endif()

if (NOT WIN32)
    list (REMOVE_ITEM CORE_SOURCE_FILES WGLPBTextureTarget.cpp)
    list (REMOVE_ITEM CORE_HEADER_FILES ${CMAKE_SOURCE_DIR}/cegui/include/CEGUI/RendererModules/OpenGL/WGLPBTextureTarget.h)
endif()

if (NOT UNIX OR APPLE)
    list (REMOVE_ITEM CORE_SOURCE_FILES GLXPBTextureTarget.cpp)
    list (REMOVE_ITEM CORE_HEADER_FILES ${CMAKE_SOURCE_DIR}/cegui/include/CEGUI/RendererModules/OpenGL/GLXPBTextureTarget.h)
endif()

cegui_add_library(${CEGUI_TARGET_NAME} CORE_SOURCE_FILES CORE_HEADER_FILES)
cegui_add_dependency(${CEGUI_TARGET_NAME} OPENGL)

if (NOT CEGUI_BUILD_EMBEDDED_GLEW)
    cegui_add_dependency(${CEGUI_TARGET_NAME} GLEW)
endif()

cegui_target_link_libraries(${CEGUI_TARGET_NAME} ${CEGUI_BASE_LIBNAME})

