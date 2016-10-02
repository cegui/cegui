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
                                        GLFBOTextureTarget.cpp
                                        GLShaderWrapper.cpp)
    list (REMOVE_ITEM CORE_HEADER_FILES ${CMAKE_SOURCE_DIR}/cegui/include/CEGUI/RendererModules/OpenGL/GLRenderer.h
                                        ${CMAKE_SOURCE_DIR}/cegui/include/CEGUI/RendererModules/OpenGL/GLGeometryBuffer.h
                                        ${CMAKE_SOURCE_DIR}/cegui/include/CEGUI/RendererModules/OpenGL/GLFBOTextureTarget.h
                                        ${CMAKE_SOURCE_DIR}/cegui/include/CEGUI/RendererModules/OpenGL/GLShaderWrapper.h)
endif()

if (NOT CEGUI_BUILD_RENDERER_OPENGL3 AND NOT CEGUI_BUILD_RENDERER_OPENGLES2)
    list (REMOVE_ITEM CORE_SOURCE_FILES Shader.cpp
                                        ShaderManager.cpp
                                        StateChangeWrapper.cpp
                                        GLBaseShaderWrapper.cpp)
    list (REMOVE_ITEM CORE_HEADER_FILES ${CMAKE_SOURCE_DIR}/cegui/include/CEGUI/RendererModules/OpenGL/Shader.h
                                        ${CMAKE_SOURCE_DIR}/cegui/include/CEGUI/RendererModules/OpenGL/ShaderManager.h
                                        ${CMAKE_SOURCE_DIR}/cegui/include/CEGUI/RendererModules/OpenGL/StateChangeWrapper.h
                                        ${CMAKE_SOURCE_DIR}/cegui/include/CEGUI/RendererModules/OpenGL/StandardShaderFrag.h
                                        ${CMAKE_SOURCE_DIR}/cegui/include/CEGUI/RendererModules/OpenGL/StandardShaderVert.h
                                        ${CMAKE_SOURCE_DIR}/cegui/include/CEGUI/RendererModules/OpenGL/GLBaseShaderWrapper.h)
endif()


if (NOT CEGUI_BUILD_RENDERER_OPENGL3)
    list (REMOVE_ITEM CORE_SOURCE_FILES GL3Renderer.cpp
                                        GL3Texture.cpp
                                        GL3GeometryBuffer.cpp
                                        GL3FBOTextureTarget.cpp
                                        GL3Shader.cpp
                                        GL3StateChangeWrapper.cpp)
    list (REMOVE_ITEM CORE_HEADER_FILES ${CMAKE_SOURCE_DIR}/cegui/include/CEGUI/RendererModules/OpenGL/GL3Renderer.h
                                        ${CMAKE_SOURCE_DIR}/cegui/include/CEGUI/RendererModules/OpenGL/GL3Texture.h
                                        ${CMAKE_SOURCE_DIR}/cegui/include/CEGUI/RendererModules/OpenGL/GL3GeometryBuffer.h
                                        ${CMAKE_SOURCE_DIR}/cegui/include/CEGUI/RendererModules/OpenGL/GL3Shader.h
                                        ${CMAKE_SOURCE_DIR}/cegui/include/CEGUI/RendererModules/OpenGL/GL3StateShaderWrapper.h
                                        ${CMAKE_SOURCE_DIR}/cegui/include/CEGUI/RendererModules/OpenGL/GL3FBOTextureTarget.h)
endif()

#GL and GL3 both use GLTexture via inheritance
if (NOT CEGUI_BUILD_RENDERER_OPENGL3 AND NOT CEGUI_BUILD_RENDERER_OPENGL)
    list (REMOVE_ITEM CORE_SOURCE_FILES GLTexture.cpp)
    list (REMOVE_ITEM CORE_HEADER_FILES ${CMAKE_SOURCE_DIR}/cegui/include/CEGUI/RendererModules/OpenGL/GLTexture.h)
endif()

if (NOT CEGUI_BUILD_RENDERER_OPENGLES2)
    list (REMOVE_ITEM CORE_SOURCE_FILES GLES2Renderer.cpp
                                        GLES2Texture.cpp
                                        GLES2GeometryBuffer.cpp
                                        GLES2FBOTextureTarget.cpp
                                        GLES2StateChangeWrapper.cpp)
    list (REMOVE_ITEM CORE_HEADER_FILES ${CMAKE_SOURCE_DIR}/cegui/include/CEGUI/RendererModules/OpenGL/GLES2Renderer.h
        ${CMAKE_SOURCE_DIR}/cegui/include/CEGUI/RendererModules/OpenGL/GLES2Texture.h
        ${CMAKE_SOURCE_DIR}/cegui/include/CEGUI/RendererModules/OpenGL/GLES2GeometryBuffer.h
        ${CMAKE_SOURCE_DIR}/cegui/include/CEGUI/RendererModules/OpenGL/GLES2FBOTextureTarget.h
        ${CMAKE_SOURCE_DIR}/cegui/include/CEGUI/RendererModules/OpenGL/GLES2StateShaderWrapper.h)

endif()

cegui_add_library(${CEGUI_TARGET_NAME} CORE_SOURCE_FILES CORE_HEADER_FILES)
cegui_add_dependency(${CEGUI_TARGET_NAME} GLM)
if (CEGUI_BUILD_RENDERER_OPENGLES2)
    if (CEGUI_BUILD_RENDERER_OPENGLES2_WITH_GLES3_SUPPORT)
        add_definitions ( -DCEGUI_GLES3_SUPPORT )
        cegui_add_dependency(${CEGUI_TARGET_NAME} OPENGLES3)
    else()
        cegui_add_dependency(${CEGUI_TARGET_NAME} OPENGLES2)
    endif()
endif()
if (${CEGUI_USE_EPOXY})
    cegui_add_dependency(${CEGUI_TARGET_NAME} GLM)
    cegui_add_dependency(${CEGUI_TARGET_NAME} EPOXY)
elseif (${CEGUI_USE_GLEW})
    cegui_add_dependency(${CEGUI_TARGET_NAME} GLM)
    cegui_add_dependency(${CEGUI_TARGET_NAME} GLEW)
    cegui_add_dependency(${CEGUI_TARGET_NAME} OPENGL)
endif()


cegui_target_link_libraries(${CEGUI_TARGET_NAME} ${CEGUI_BASE_LIBNAME})

