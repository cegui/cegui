option(CEGUI_SAMPLES_ENABLE_TREE "Specifies whether this sample should be built or not" TRUE)
if(CEGUI_SAMPLES_ENABLE_TREE)
    cegui_add_sample(CEGUISample_Tree)
endif()