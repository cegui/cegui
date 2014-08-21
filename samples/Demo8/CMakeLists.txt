list (APPEND SAMPLES_SOURCES Sample_Demo8.cpp)
list (APPEND SAMPLES_HEADERS Sample_Demo8.h)

cegui_target_link_libraries(${CEGUI_TARGET_NAME} ${CEGUI_LUA_SCRIPTMODULE_LIBNAME})