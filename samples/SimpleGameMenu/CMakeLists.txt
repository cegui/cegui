list (APPEND SAMPLES_SOURCES SimpleGameMenu.cpp)
list (APPEND SAMPLES_SOURCES ${CMAKE_SOURCE_DIR}/samples/common/NavigationStrategies.cpp)

list (APPEND SAMPLES_HEADERS SimpleGameMenu.h)
list (APPEND SAMPLES_HEADERS ${CMAKE_SOURCE_DIR}/samples/common/include/NavigationStrategies.h)

set (SAMPLES_SOURCES "${SAMPLES_SOURCES}" PARENT_SCOPE)
set (SAMPLES_HEADERS "${SAMPLES_HEADERS}" PARENT_SCOPE)