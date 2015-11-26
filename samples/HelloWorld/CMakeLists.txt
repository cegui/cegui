option(CEGUI_SAMPLES_ENABLE_HELLO_WORLD "Specifies whether this sample should be built or not" TRUE)
if(CEGUI_SAMPLES_ENABLE_HELLO_WORLD)
    cegui_add_sample(CEGUISample_HelloWorld)
endif()