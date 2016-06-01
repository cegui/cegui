option(CEGUI_SAMPLES_ENABLE_TEXT "Specifies whether this sample should be built or not" TRUE)
if(CEGUI_SAMPLES_ENABLE_TEXT)
    cegui_add_sample(CEGUISample_Text)
endif()