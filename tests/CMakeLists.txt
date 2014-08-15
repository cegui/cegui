set(EXTRA_HEADER_FILES
    ${CMAKE_SOURCE_DIR}/samples/ModelView/InventoryModel.h
)
set(EXTRA_SOURCE_FILES
    ${CMAKE_SOURCE_DIR}/samples/ModelView/InventoryModel.cpp
)

if (CEGUI_BUILD_PERFORMANCE_TESTS)
    file(GLOB PERFORMANCE_TESTS_HEADER_FILES "performance/*.h")
    file(GLOB PERFORMANCE_TESTS_SOURCE_FILES "performance/*.cpp")

    set(EXTRA_HEADER_FILES ${EXTRA_HEADER_FILES} ${PERFORMANCE_TESTS_HEADER_FILES})
    set(EXTRA_SOURCE_FILES ${EXTRA_SOURCE_FILES} ${PERFORMANCE_TESTS_SOURCE_FILES})

    include_directories(${CMAKE_CURRENT_SOURCE_DIR}/performance/)
endif()

include_directories(${CMAKE_SOURCE_DIR}/samples/ModelView)

cegui_add_test_executable(CEGUITests "${EXTRA_HEADER_FILES}" "${EXTRA_SOURCE_FILES}")

