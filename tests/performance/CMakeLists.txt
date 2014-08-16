set(EXTRA_SOURCE_FILES
    ${CMAKE_SOURCE_DIR}/tests/unit/Entry.cpp
)

cegui_add_test_executable_with_extra_files(CEGUIPerformanceTests "" ${EXTRA_SOURCE_FILES})
