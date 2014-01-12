option(CEGUI_SAMPLES_ENABLE_SIMPLE_GAME_MENU "Specifies whether this sample should be built or not" TRUE)
if(CEGUI_SAMPLES_ENABLE_SIMPLE_GAME_MENU)
    #add the navigation strategies
    set(EXTRA_HEADER_FILES
        ${CMAKE_SOURCE_DIR}/samples/common/include/NavigationStrategies.h
    )
    set(EXTRA_SOURCE_FILES
        ${CMAKE_SOURCE_DIR}/samples/common/NavigationStrategies.cpp
    )

    cegui_add_sample_with_extra_files(CEGUISample_SimpleGameMenu ${EXTRA_HEADER_FILES} ${EXTRA_SOURCE_FILES})
endif()
