option(CEGUI_SAMPLES_ENABLE_TEXT_DEMO "Specifies whether this sample should be built or not" TRUE)
if(CEGUI_SAMPLES_ENABLE_TEXT_DEMO)
    cegui_add_sample(CEGUITextDemo)
endif()