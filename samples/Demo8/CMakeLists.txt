option(CEGUI_SAMPLES_ENABLE_DEMO8 "Specifies whether this sample should be built or not" TRUE)
if(CEGUI_SAMPLES_ENABLE_DEMO8)
    cegui_add_sample(CEGUISample_Demo8)
endif()

cegui_target_link_libraries(${CEGUI_TARGET_NAME} ${CEGUI_LUA_SCRIPTMODULE_LIBNAME})