option(CEGUI_SAMPLES_ENABLE_TREE_DEMO "Specifies whether this sample should be built or not" TRUE)
if(CEGUI_SAMPLES_ENABLE_TREE_DEMO)
    cegui_add_sample(CEGUITreeDemo)
endif()