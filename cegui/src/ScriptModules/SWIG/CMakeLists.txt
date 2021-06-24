cmake_minimum_required(VERSION 3.1)

set(Python_ADDITIONAL_VERSIONS 3.5)

find_package(PythonInterp)
find_package(PythonLibs)
find_package(SWIG 3.0.8)

include_directories(${PYTHON_INCLUDE_PATH})

include(${SWIG_USE_FILE})
set(CMAKE_SWIG_FLAGS  -builtin)


include_directories("../include")
include_directories("${PROJECT_BINARY_DIR}/cegui/include")

set_source_files_properties(CEGUI.i PROPERTIES CPLUSPLUS ON)
if (${CMAKE_VERSION} VERSION_LESS "3.8.0")
	swig_add_module(CEGUI python CEGUI.i)
else()
	swig_add_library(CEGUI LANGUAGE python SOURCES CEGUI.i)
endif()
swig_link_libraries(CEGUI "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/lib${CEGUI_BASE_LIBNAME}${CMAKE_SHARED_LIBRARY_SUFFIX}" ${PYTHON_LIBRARIES})

set_source_files_properties(OpenGL.i PROPERTIES CPLUSPLUS ON)
if (${CMAKE_VERSION} VERSION_LESS "3.8.0")
	swig_add_module(CEGUIOpenGL python OpenGL.i)
else()
	swig_add_library(CEGUIOpenGL LANGUAGE python SOURCES OpenGL.i)
endif()
swig_link_libraries(CEGUIOpenGL "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/lib${CEGUI_BASE_LIBNAME}${CMAKE_SHARED_LIBRARY_SUFFIX}" "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/lib${CEGUI_OPENGL_RENDERER_LIBNAME}${CMAKE_SHARED_LIBRARY_SUFFIX}" ${PYTHON_LIBRARIES})

set(PYTHON_SITE_PACKAGES lib/python${PYTHON_VERSION_MAJOR}.${PYTHON_VERSION_MINOR}/dist-packages/)
install(TARGETS ${SWIG_MODULE_CEGUI_REAL_NAME} LIBRARY DESTINATION ${PYTHON_SITE_PACKAGES})
install(FILES ${PROJECT_BINARY_DIR}/cegui/src/ScriptModules/SWIG/CEGUI.py DESTINATION ${PYTHON_SITE_PACKAGES})

install(TARGETS ${SWIG_MODULE_CEGUIOpenGL_REAL_NAME} LIBRARY DESTINATION ${PYTHON_SITE_PACKAGES})
install(FILES ${PROJECT_BINARY_DIR}/cegui/src/ScriptModules/SWIG/CEGUIOpenGL.py DESTINATION ${PYTHON_SITE_PACKAGES})
