cegui_add_test_executable(CEGUIPerformanceTests)

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