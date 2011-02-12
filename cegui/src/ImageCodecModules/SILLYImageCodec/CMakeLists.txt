set (CEGUI_TARGET_NAME ${CEGUI_SILLY_IMAGECODEC_LIBNAME})

cegui_gather_files()
cegui_add_library(TRUE FALSE)
cegui_add_dependency(SILLY)

target_link_libraries(${CEGUI_TARGET_NAME} ${CEGUI_BASE_LIBNAME})

if (UNIX AND NOT APPLE)
    set_target_properties(${CEGUI_TARGET_NAME} PROPERTIES COMPILE_DEFINITIONS USE_SILLYOPTIONS_H)
endif()

