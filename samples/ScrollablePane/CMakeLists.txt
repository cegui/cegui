option(CEGUI_SAMPLES_ENABLE_SCROLLABLEPANE "Specifies whether this sample should be built or not" TRUE)
if(CEGUI_SAMPLES_ENABLE_SCROLLABLEPANE)
    cegui_add_sample(CEGUISample_ScrollablePane)
endif()