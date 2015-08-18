set(EXTRA_HEADER_FILES
    ${CMAKE_SOURCE_DIR}/samples/ModelView/InventoryModel.h
)
set(EXTRA_SOURCE_FILES
    ${CMAKE_SOURCE_DIR}/samples/ModelView/InventoryModel.cpp
)

include_directories(${CMAKE_SOURCE_DIR}/samples/ModelView)

cegui_add_test_executable_with_extra_files(CEGUITests "${EXTRA_HEADER_FILES}" "${EXTRA_SOURCE_FILES}")
