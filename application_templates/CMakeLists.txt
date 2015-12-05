cegui_dependent_option(CEGUI_BUILD_APPLICATION_TEMPLATE_SDL2 "Specifies whether to build the application template for SDL2" "SDL2_FOUND;CEGUI_BUILD_RENDERER_OPENGL")
cegui_dependent_option(CEGUI_BUILD_APPLICATION_TEMPLATE_GLFW3 "Specifies whether to build the application template for GLFW3" "GLFW3_FOUND;CEGUI_BUILD_RENDERER_OPENGL")

if(CEGUI_BUILD_RENDERER_OPENGL)
    if(${CEGUI_BUILD_APPLICATION_TEMPLATE_SDL2})
        add_executable(CEGUITemplate_SDL2 SDL2.cpp)
        
        cegui_target_link_libraries(CEGUITemplate_SDL2 ${CEGUI_BASE_LIBNAME})
        cegui_target_link_libraries(CEGUITemplate_SDL2 ${CEGUI_OPENGL_RENDERER_LIBNAME})
        cegui_add_dependency(CEGUITemplate_SDL2 SDL2)
    endif()

    if(${CEGUI_BUILD_APPLICATION_TEMPLATE_GLFW3})
        add_executable(CEGUITemplate_GLFW3 glfw3.cpp)
        if (WIN32)
            set_property (TARGET CEGUITemplate_GLFW3 PROPERTY WIN32_EXECUTABLE TRUE)
        endif ()
        
        cegui_target_link_libraries(CEGUITemplate_GLFW3 ${CEGUI_BASE_LIBNAME})
        cegui_target_link_libraries(CEGUITemplate_GLFW3 ${CEGUI_OPENGL_RENDERER_LIBNAME})
        cegui_add_dependency(CEGUITemplate_GLFW3 GLFW3)

    endif()
endif()
