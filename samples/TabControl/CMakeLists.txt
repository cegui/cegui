option(CEGUI_SAMPLES_ENABLE_TABCONTROL "Specifies whether this sample should be built or not" TRUE)
if(CEGUI_SAMPLES_ENABLE_TABCONTROL)
    cegui_add_sample(CEGUISample_TabControl)
endif()