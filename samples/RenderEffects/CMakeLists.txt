option(CEGUI_SAMPLES_ENABLE_EFFECTS "Specifies whether this sample should be built or not" TRUE)
if(CEGUI_SAMPLES_ENABLE_EFFECTS)
    cegui_add_sample(CEGUISample_Effects)
endif()