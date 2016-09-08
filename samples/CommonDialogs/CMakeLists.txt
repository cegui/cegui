option(CEGUI_SAMPLES_ENABLE_COMMON_DIALOGS "Specifies whether this sample should be built or not" TRUE)
if(CEGUI_SAMPLES_ENABLE_COMMON_DIALOGS)
    cegui_add_sample(CEGUISample_CommonDialogs)
endif()

cegui_target_link_libraries(${CEGUI_TARGET_NAME} ${CEGUI_COMMON_DIALOGS_LIBNAME})