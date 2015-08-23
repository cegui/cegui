set (CEGUI_TARGET_NAME ${CEGUI_OPENGL_RENDERER_LIBNAME})

cegui_gather_files()

if (CEGUI_BUILD_SHARED_LIBS_WITH_STATIC_DEPENDENCIES AND CEGUI_USE_GLEW)
    add_definitions( -DGLEW_STATIC )
endif()

if (NOT APPLE OR NOT CEGUI_BUILD_RENDERER_OPENGL)
    list (REMOVE_ITEM CORE_SOURCE_FILES ApplePBTextureTarget.cpp)
    list (REMOVE_ITEM CORE_HEADER_FILES ${CMAKE_SOURCE_DIR}/cegui/include/CEGUI/RendererModules/OpenGL/ApplePBTextureTarget.h)
endif()

if (NOT WIN32 OR NOT CEGUI_BUILD_RENDERER_OPENGL)
    list (REMOVE_ITEM CORE_SOURCE_FILES WGLPBTextureTarget.cpp)
    list (REMOVE_ITEM CORE_HEADER_FILES ${CMAKE_SOURCE_DIR}/cegui/include/CEGUI/RendererModules/OpenGL/WGLPBTextureTarget.h)
endif()

if (NOT UNIX OR APPLE OR NOT CEGUI_BUILD_RENDERER_OPENGL)
    list (REMOVE_ITEM CORE_SOURCE_FILES GLXPBTextureTarget.cpp)
    list (REMOVE_ITEM CORE_HEADER_FILES ${CMAKE_SOURCE_DIR}/cegui/include/CEGUI/RendererModules/OpenGL/GLXPBTextureTarget.h)
endif()

if (NOT CEGUI_BUILD_RENDERER_OPENGL)
    list (REMOVE_ITEM CORE_SOURCE_FILES GLRenderer.cpp
                                        GLGeometryBuffer.cpp
                                        GLFBOTextureTarget.cpp)
    list (REMOVE_ITEM CORE_HEADER_FILES ${CMAKE_SOURCE_DIR}/cegui/include/CEGUI/RendererModules/OpenGL/GLRenderer.h
                                        ${CMAKE_SOURCE_DIR}/cegui/include/CEGUI/RendererModules/OpenGL/GLGeometryBuffer.h
                                        ${CMAKE_SOURCE_DIR}/cegui/include/CEGUI/RendererModules/OpenGL/GLFBOTextureTarget.h)
endif()

if (NOT CEGUI_BUILD_RENDERER_OPENGL3)
    list (REMOVE_ITEM CORE_SOURCE_FILES GL3Renderer.cpp
                                        GL3GeometryBuffer.cpp
                                        GL3FBOTextureTarget.cpp
                                        Shader.cpp
                                        ShaderManager.cpp
                                        StateChangeWrapper.cpp)
    list (REMOVE_ITEM CORE_HEADER_FILES ${CMAKE_SOURCE_DIR}/cegui/include/CEGUI/RendererModules/OpenGL/GL3Renderer.h
                                        ${CMAKE_SOURCE_DIR}/cegui/include/CEGUI/RendererModules/OpenGL/GL3GeometryBuffer.h
                                        ${CMAKE_SOURCE_DIR}/cegui/include/CEGUI/RendererModules/OpenGL/GL3FBOTextureTarget.h
                                        ${CMAKE_SOURCE_DIR}/cegui/include/CEGUI/RendererModules/OpenGL/Shader.h
                                        ${CMAKE_SOURCE_DIR}/cegui/include/CEGUI/RendererModules/OpenGL/ShaderManager.h
                                        ${CMAKE_SOURCE_DIR}/cegui/include/CEGUI/RendererModules/OpenGL/StateChangeWrapper.h
                                        ${CMAKE_SOURCE_DIR}/cegui/include/CEGUI/RendererModules/OpenGL/StandardShaderFrag.h
                                        ${CMAKE_SOURCE_DIR}/cegui/include/CEGUI/RendererModules/OpenGL/StandardShaderVert.h)
endif()

cegui_add_library(${CEGUI_TARGET_NAME} CORE_SOURCE_FILES CORE_HEADER_FILES)
if (${CEGUI_USE_EPOXY})
    cegui_add_dependency(${CEGUI_TARGET_NAME} GLM)
    cegui_add_dependency(${CEGUI_TARGET_NAME} EPOXY)
elseif (${CEGUI_USE_GLEW})
    cegui_add_dependency(${CEGUI_TARGET_NAME} OPENGL)
    cegui_add_dependency(${CEGUI_TARGET_NAME} GLM)
    cegui_add_dependency(${CEGUI_TARGET_NAME} GLEW PUBLIC)
endif ()

cegui_target_link_libraries(${CEGUI_TARGET_NAME} ${CEGUI_BASE_LIBNAME})

