set (CEGUI_TARGET_NAME ${CEGUI_DIRECT3D10_RENDERER_LIBNAME})

include_directories(${DIRECTXSDK_INCLUDE_DIR})
link_directories(${DIRECTXSDK_LIBRARY_DIR})

cegui_gather_files()
cegui_add_library(TRUE TRUE)

target_link_libraries(${CEGUI_TARGET_NAME} ${CEGUI_BASE_LIBNAME} d3d10)
target_link_libraries(${CEGUI_TARGET_NAME} optimized d3dx10 debug d3dx10d)

