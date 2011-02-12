set( CEGUI_TARGET_NAME ${CEGUI_STB_IMAGECODEC_LIBNAME} )

cegui_gather_files()
cegui_add_library(TRUE FALSE)

target_link_libraries(${CEGUI_TARGET_NAME} ${CEGUI_BASE_LIBNAME})

