set( CEGUI_TARGET_NAME ${CEGUI_CORONA_IMAGECODEC_LIBNAME} )

cegui_gather_files()
cegui_add_library(TRUE FALSE)
cegui_add_dependency(CORONA)

target_link_libraries(${CEGUI_TARGET_NAME} ${CEGUI_BASE_LIBNAME})

