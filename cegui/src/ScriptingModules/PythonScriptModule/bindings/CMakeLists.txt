file (GLOB PYCEGUI_SOURCE_FILES output/CEGUI/*.cpp)
file (GLOB PYCEGUINULLRENDERER_SOURCE_FILES output/CEGUINullRenderer/*.cpp)
file (GLOB PYCEGUIOGRERENDERER_SOURCE_FILES output/CEGUIOgreRenderer/*.cpp)
file (GLOB PYCEGUIOPENGLRENDERER_SOURCE_FILES output/CEGUIOpenGLRenderer/*.cpp)

execute_process(
    COMMAND ${PYTHON_EXECUTABLE} -c "import distutils.sysconfig; print distutils.sysconfig.get_python_lib()"
    OUTPUT_VARIABLE CEGUI_PYTHON_INSTALL_DIR
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/output/CEGUI
                    ${CMAKE_CURRENT_SOURCE_DIR}
                    ${PYTHON_INCLUDE_DIR}
                    ${Boost_INCLUDE_DIR})

add_library(PyCEGUI MODULE ${PYCEGUI_SOURCE_FILES})
target_link_libraries(PyCEGUI ${CEGUI_BASE_LIBNAME} ${Boost_LIBRARIES} ${PYTHON_LIBRARIES})
set_target_properties(PyCEGUI PROPERTIES PREFIX "")

if (UNIX AND NOT APPLE AND NOT WIN32)
    install(TARGETS PyCEGUI LIBRARY DESTINATION "${CEGUI_PYTHON_INSTALL_DIR}")
endif()

if (CEGUI_BUILD_RENDERER_NULL)
    include_directories(BEFORE ${CMAKE_CURRENT_SOURCE_DIR}/output/CEGUINullRenderer)
    add_library(PyCEGUINullRenderer MODULE ${PYCEGUINULLRENDERER_SOURCE_FILES})
    target_link_libraries(PyCEGUINullRenderer ${CEGUI_NULL_RENDERER_LIBNAME} ${CEGUI_BASE_LIBNAME} ${Boost_LIBRARIES} ${PYTHON_LIBRARIES})
    set_target_properties(PyCEGUINullRenderer PROPERTIES PREFIX "")

    if (UNIX AND NOT APPLE AND NOT WIN32)
        install(TARGETS PyCEGUINullRenderer LIBRARY DESTINATION "${CEGUI_PYTHON_INSTALL_DIR}")
    endif()
endif()

if (CEGUI_BUILD_RENDERER_OGRE)
    include_directories(BEFORE ${CMAKE_CURRENT_SOURCE_DIR}/output/CEGUIOgreRenderer)
    add_library(PyCEGUIOgreRenderer MODULE ${PYCEGUIOGRERENDERER_SOURCE_FILES})
    target_link_libraries(PyCEGUIOgreRenderer ${CEGUI_OGRE_RENDERER_LIBNAME} ${CEGUI_BASE_LIBNAME} ${Boost_LIBRARIES} ${PYTHON_LIBRARIES})
    set_target_properties(PyCEGUIOgreRenderer PROPERTIES PREFIX "")

    if (UNIX AND NOT APPLE AND NOT WIN32)
        install(TARGETS PyCEGUIOgreRenderer LIBRARY DESTINATION "${CEGUI_PYTHON_INSTALL_DIR}")
    endif()
endif()

if (CEGUI_BUILD_RENDERER_OPENGL)
    include_directories(BEFORE ${CMAKE_CURRENT_SOURCE_DIR}/output/CEGUIOpenGLRenderer)
    add_library(PyCEGUIOpenGLRenderer MODULE ${PYCEGUIOPENGLRENDERER_SOURCE_FILES})
    target_link_libraries(PyCEGUIOpenGLRenderer ${CEGUI_OPENGL_RENDERER_LIBNAME} ${CEGUI_BASE_LIBNAME} ${Boost_LIBRARIES} ${PYTHON_LIBRARIES})
    set_target_properties(PyCEGUIOpenGLRenderer PROPERTIES PREFIX "")

    if (UNIX AND NOT APPLE AND NOT WIN32)
        install(TARGETS PyCEGUIOpenGLRenderer LIBRARY DESTINATION "${CEGUI_PYTHON_INSTALL_DIR}")
    endif()
endif()

