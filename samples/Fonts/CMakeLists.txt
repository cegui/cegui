option(CEGUI_SAMPLES_ENABLE_FONTS "Specifies whether this sample should be built or not" TRUE)
if(CEGUI_SAMPLES_ENABLE_FONTS)
    cegui_add_sample(CEGUISample_Fonts)
endif()