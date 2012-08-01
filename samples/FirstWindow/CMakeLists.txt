option(CEGUI_SAMPLES_ENABLE_HELLO_WORLD_DEMO "Specifies whether this sample should be built or not" TRUE)
if(CEGUI_SAMPLES_ENABLE_HELLO_WORLD_DEMO)
    cegui_add_sample(CEGUIHelloWorldDemo)
endif()