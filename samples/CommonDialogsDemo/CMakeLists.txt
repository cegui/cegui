option(CEGUI_SAMPLES_ENABLE_COMMON_DIALOGUES_DEMO "Specifies whether this sample should be built or not" TRUE)
if(CEGUI_SAMPLES_ENABLE_COMMON_DIALOGUES_DEMO)
    cegui_add_sample(CEGUICommonDialogsDemo)
endif()

cegui_target_link_libraries(${CEGUI_TARGET_NAME} ${CEGUI_COMMON_DIALOGS_LIBNAME})