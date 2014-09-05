# provide access to common test files
include_directories("${CMAKE_CURRENT_SOURCE_DIR}")

if (CEGUI_BUILD_TESTS)
    add_subdirectory(unit)
endif()

if (CEGUI_BUILD_PERFORMANCE_TESTS)
    add_subdirectory(performance)
endif()

if (CEGUI_BUILD_DATAFILES_TEST)
    add_test(NAME CEGUIDatafilesTest COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/CEGUIDatafilesTest.py)
endif()
