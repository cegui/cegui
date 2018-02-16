option(CEGUI_SAMPLES_ENABLE_INVENTORY "Specifies whether this sample should be built or not" TRUE)
if(CEGUI_SAMPLES_ENABLE_INVENTORY)
    cegui_add_sample(CEGUISample_Inventory)
endif()