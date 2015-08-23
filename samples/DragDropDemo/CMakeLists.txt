option(CEGUI_SAMPLES_ENABLE_DRAGDROP_DEMO "Specifies whether this sample should be built or not" TRUE)
if(CEGUI_SAMPLES_ENABLE_DRAGDROP_DEMO)
    cegui_add_sample(CEGUIDragDropDemo)
endif()