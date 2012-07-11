option(CEGUI_SAMPLES_ENABLE_EDITBOX_VALIDATION_DEMO "Specifies whether this sample should be built or not" TRUE)
if(CEGUI_SAMPLES_ENABLE_EDITBOX_VALIDATION_DEMO)
    cegui_add_sample(CEGUIEditboxValidationDemo)
endif()