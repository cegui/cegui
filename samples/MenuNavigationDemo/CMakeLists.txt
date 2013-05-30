option(CEGUI_SAMPLES_ENABLE_MENU_NAVIGATION_DEMO "Specifies whether this sample should be built or not" TRUE)
if(CEGUI_SAMPLES_ENABLE_MENU_NAVIGATION_DEMO)
    cegui_add_sample(CEGUIMenuNavigationDemo)
endif()