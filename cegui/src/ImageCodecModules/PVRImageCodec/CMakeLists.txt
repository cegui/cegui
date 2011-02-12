set (CEGUI_TARGET_NAME ${CEGUI_PVR_IMAGECODEC_LIBNAME})

cegui_gather_files()
cegui_add_library(TRUE FALSE)
cegui_add_dependency(PVR)

target_link_libraries(${CEGUI_TARGET_NAME} ${CEGUI_BASE_LIBNAME})

