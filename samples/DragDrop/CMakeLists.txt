option(CEGUI_SAMPLES_ENABLE_DRAG_AND_DROP "Specifies whether this sample should be built or not" TRUE)
if(CEGUI_SAMPLES_ENABLE_DRAG_AND_DROP)
    cegui_add_sample(CEGUISample_DragAndDrop)
endif()