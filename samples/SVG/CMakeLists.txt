option(CEGUI_SAMPLES_ENABLE_SVG "Specifies whether this sample should be built or not" TRUE)
if(CEGUI_SAMPLES_ENABLE_SVG)
    cegui_add_sample(CEGUISample_SVG)
endif()