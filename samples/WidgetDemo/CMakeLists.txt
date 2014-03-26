option(CEGUI_SAMPLES_ENABLE_WIDGET_DEMO "Specifies whether this sample should be built or not" TRUE)
if(CEGUI_SAMPLES_ENABLE_WIDGET_DEMO)
    file(GLOB WIDGET_HEADERS "${CMAKE_SOURCE_DIR}/cegui/include/CEGUI/widgets/*.h")
    list(APPEND WIDGET_HEADERS "${CMAKE_SOURCE_DIR}/cegui/include/CEGUI/Window.h")
 
    set(ALL_EVENTS_CODE "std::set<String> allEvents;")
 
    foreach (WIDGET_HEADER ${WIDGET_HEADERS})
        file(READ "${WIDGET_HEADER}" HEADER_SOURCE)
        string(REGEX MATCHALL "static const String Event[^;]+" MATCHED_LINES "${HEADER_SOURCE}")
        foreach (MATCHED_LINE ${MATCHED_LINES})
            string(REGEX REPLACE "static const String Event([^;]+)" "\\1" CAPTURE_GROUP "${MATCHED_LINE}")
            if (NOT CAPTURE_GROUP STREQUAL "Namespace")
                set(ALL_EVENTS_CODE "${ALL_EVENTS_CODE}\nallEvents.insert(\"${CAPTURE_GROUP}\");")
            endif()                                                                                      
        endforeach()
    endforeach()
 
    file(WRITE ${CMAKE_BINARY_DIR}/samples/common/include/AllEvents.inc "${ALL_EVENTS_CODE}")

    include_directories(${CMAKE_BINARY_DIR}/samples/common/include)
	
    
    cegui_add_sample(CEGUIWidgetDemo)
endif()