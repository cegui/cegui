option(CEGUI_SAMPLES_ENABLE_GAMEMENU "Specifies whether this sample should be built or not" TRUE)
if(CEGUI_SAMPLES_ENABLE_GAMEMENU)
    cegui_add_sample(CEGUISample_GameMenuDemo)
endif()