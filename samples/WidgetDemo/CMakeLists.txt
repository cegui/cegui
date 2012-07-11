option(CEGUI_SAMPLES_ENABLE_WIDGET_DEMO "Specifies whether this sample should be built or not" TRUE)
if(CEGUI_SAMPLES_ENABLE_WIDGET_DEMO)
    cegui_add_sample(CEGUIWidgetDemo)
endif()