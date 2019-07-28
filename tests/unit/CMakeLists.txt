set(EXTRA_HEADER_FILES
    ${CMAKE_SOURCE_DIR}/samples/ModelView/InventoryModel.h
)
set(EXTRA_SOURCE_FILES
    ${CMAKE_SOURCE_DIR}/samples/ModelView/InventoryModel.cpp
)

include_directories(${CMAKE_SOURCE_DIR}/samples/ModelView)

cegui_add_test_executable_with_extra_files(CEGUITests "${EXTRA_HEADER_FILES}" "${EXTRA_SOURCE_FILES}")

###########################################################################
#                    MSVC PROJ USER FILE TEMPLATES
###########################################################################
if(MSVC)
    # Placing vcxproj file to set debugging directory and other settings for the project by default
    configure_file(
        ${CMAKE_MODULE_PATH}/templates/VisualStudioUserFile.vcxproj.user.in
        ${CMAKE_CURRENT_BINARY_DIR}/${CEGUI_TARGET_NAME}.vcxproj.user
        @ONLY
        )
endif()