option(CEGUI_SAMPLES_ENABLE_MINESWEEPER "Specifies whether this sample should be built or not" TRUE)
if(CEGUI_SAMPLES_ENABLE_MINESWEEPER)
    cegui_add_sample(CEGUISample_Minesweeper)
endif()