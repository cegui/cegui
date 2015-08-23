set (CEGUI_TARGET_NAME ${CEGUI_OGRE_RENDERER_LIBNAME})

# this prevents the boost auto-link dis-feature
add_definitions(-DBOOST_ALL_NO_LIB)

cegui_gather_files()
cegui_add_library(${CEGUI_TARGET_NAME} CORE_SOURCE_FILES CORE_HEADER_FILES)
cegui_add_dependency(${CEGUI_TARGET_NAME} OGRE PUBLIC TRUE)

cegui_target_link_libraries(${CEGUI_TARGET_NAME} ${CEGUI_BASE_LIBNAME})

