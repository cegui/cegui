# We check if the required dependencies are available for the sample framework

if(CEGUI_BUILD_RENDERER_OGRE AND NOT CEGUI_OGRE_SAMPLES_AVAILABLE)
    message(SEND_ERROR "The Samples and the OgreRenderer are enabled, but certain Ogre dependencies are missing (boost_system or others). Please configure these missing dependencies in CMake")
endif()
if(CEGUI_BUILD_RENDERER_OPENGL OR CEGUI_BUILD_RENDERER_OPENGL3)
    if(NOT GLFW_FOUND)
        message(SEND_ERROR "The Samples and the OpenGL or OpenGL3 Renderer builds are enabled - However this setup requires GLFW, which was not found. Please add GFLW to your dependencies and reconfigure CMake.")
    endif()
endif()


# We clear the samples.xml file, which will be used by the samples browser, and
# add the samples header to it before starting to add the samples 
file(WRITE ${CMAKE_BINARY_DIR}/datafiles/samples/samples.xml "")
file(READ ${CMAKE_CURRENT_SOURCE_DIR}/common/SamplesXmlHeader.xml DEMO_META_DATA)
file(APPEND ${CMAKE_BINARY_DIR}/datafiles/samples/samples.xml ${DEMO_META_DATA})

add_subdirectory(HelloWorld)
add_subdirectory(WidgetDemo)
add_subdirectory(EffectsDemo)
add_subdirectory(FontDemo)

add_subdirectory(Demo6)
add_subdirectory(DragDropDemo)
add_subdirectory(EditboxValidationDemo)
add_subdirectory(GameMenu)
add_subdirectory(HUDDemo)
add_subdirectory(InventoryDemo)
add_subdirectory(Minesweeper)
add_subdirectory(ScrollablePaneDemo)
add_subdirectory(TabControlDemo)
add_subdirectory(TextDemo)
add_subdirectory(TreeDemo)
add_subdirectory(CommonDialogsDemo)

if (CEGUI_BUILD_LUA_MODULE)
    # add_subdirectory(Demo8)
endif()

# Simple test for GLES renderer.
#
# Commented since it's does not use the samples framework and is merely included
# such that recreation from scratch is not required in the future.  Aka, it's
# one of CrazyEddie's hacks that he wants to keep around :-p
#if (CEGUI_BUILD_RENDERER_OPENGLES)
#    add_subdirectory(GLESTest)
#endif()


# Finally we add the samples footer to the samples.xml
file(READ ${CMAKE_CURRENT_SOURCE_DIR}/common/SamplesXmlFooter.xml DEMO_META_DATA)
file(APPEND ${CMAKE_BINARY_DIR}/datafiles/samples/samples.xml ${DEMO_META_DATA})

# And add it to the install
install(FILES ${CMAKE_BINARY_DIR}/datafiles/samples/samples.xml
        DESTINATION ${CEGUI_DATA_INSTALL_DIR}/samples/)
