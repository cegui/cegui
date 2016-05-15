option(CEGUI_SAMPLES_ENABLE_RESTAURANT_GAME "Specifies whether this sample should be built or not" TRUE)
if(CEGUI_SAMPLES_ENABLE_RESTAURANT_GAME)
    cegui_add_sample(CEGUISample_RestaurantGame)
endif()