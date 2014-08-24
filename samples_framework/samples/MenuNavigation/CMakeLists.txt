list (APPEND SAMPLES_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/MenuNavigation.cpp)
list (APPEND SAMPLES_SOURCES ${CMAKE_SOURCE_DIR}/samples_framework/samples/common/NavigationStrategies.cpp)

list (APPEND SAMPLES_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/MenuNavigation.h)
list (APPEND SAMPLES_HEADERS ${CMAKE_SOURCE_DIR}/samples_framework/samples/common/include/NavigationStrategies.h)

list (APPEND SAMPLES_LIST "MenuNavigation")

set (SAMPLES_LIST "${SAMPLES_LIST}" PARENT_SCOPE)
set (SAMPLES_SOURCES "${SAMPLES_SOURCES}" PARENT_SCOPE)
set (SAMPLES_HEADERS "${SAMPLES_HEADERS}" PARENT_SCOPE)