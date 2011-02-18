set (CEGUI_TARGET_NAME ${CEGUI_DIRECT3D9_RENDERER_LIBNAME})

cegui_gather_files()
cegui_add_library(TRUE TRUE)

include_directories(${DIRECTXSDK_INCLUDE_DIR})
link_directories(${DIRECTXSDK_LIBRARY_DIR})

target_link_libraries(${CEGUI_TARGET_NAME} ${CEGUI_BASE_LIBNAME} d3d9)
target_link_libraries(${CEGUI_TARGET_NAME} optimized d3dx9 debug d3dx9d)

