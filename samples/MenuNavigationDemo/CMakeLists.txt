option(CEGUI_SAMPLES_ENABLE_MENU_NAVIGATION_DEMO "Specifies whether this sample should be built or not" TRUE)
if(CEGUI_SAMPLES_ENABLE_MENU_NAVIGATION_DEMO)
    #add the navigation strategies
    set(EXTRA_HEADER_FILES
        ${CMAKE_SOURCE_DIR}/samples/common/include/NavigationStrategies.h
    )
    set(EXTRA_SOURCE_FILES
        ${CMAKE_SOURCE_DIR}/samples/common/NavigationStrategies.cpp
    )

    cegui_add_sample_with_extra_files(CEGUIMenuNavigationDemo ${EXTRA_HEADER_FILES} ${EXTRA_SOURCE_FILES})
endif()
