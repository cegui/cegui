option(CEGUI_SAMPLES_ENABLE_FIRST_PERSON_HUD_DEMO "Specifies whether this sample should be built or not" TRUE)
if(CEGUI_SAMPLES_ENABLE_FIRST_PERSON_HUD_DEMO)
    cegui_add_sample(CEGUIFirstPersonHUDDemo)
endif()