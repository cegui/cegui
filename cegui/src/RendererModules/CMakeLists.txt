if (CEGUI_BUILD_RENDERER_OPENGL OR CEGUI_BUILD_RENDERER_OPENGL3 OR CEGUI_BUILD_RENDERER_OPENGLES2)
    add_subdirectory(OpenGL)
endif()

if (CEGUI_BUILD_RENDERER_DIRECT3D11)
    add_subdirectory(Direct3D11)
endif()

if (CEGUI_BUILD_RENDERER_OGRE)
    add_subdirectory(Ogre)
endif()

if (CEGUI_BUILD_RENDERER_IRRLICHT)
    add_subdirectory(Irrlicht)
endif()

if (CEGUI_BUILD_RENDERER_DIRECTFB)
    add_subdirectory(DirectFB)
endif()

if (CEGUI_BUILD_RENDERER_NULL)
    add_subdirectory(Null)
endif()

if (CEGUI_BUILD_RENDERER_OPENGLES)
    add_subdirectory(OpenGLES)
endif()


