option(CEGUI_SAMPLES_ENABLE_CUSTOM_SHAPES_DRAWING "Specifies whether this sample should be built or not" TRUE)
if(CEGUI_SAMPLES_ENABLE_CUSTOM_SHAPES_DRAWING)
    cegui_add_sample(CEGUISample_CustomShapesDrawing)
endif()