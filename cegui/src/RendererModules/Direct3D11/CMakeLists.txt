set (CEGUI_TARGET_NAME ${CEGUI_DIRECT3D11_RENDERER_LIBNAME})

cegui_gather_files()
cegui_add_library(TRUE TRUE)

include_directories(${DIRECTXSDK_INCLUDE_DIR})
link_directories(${DIRECTXSDK_LIBRARY_DIR})

target_link_libraries(${CEGUI_TARGET_NAME} ${CEGUI_BASE_LIBNAME} d3d11 d3dcompiler)
target_link_libraries(${CEGUI_TARGET_NAME} optimized d3dx11 d3dx10 debug d3dx11d d3dx10d)

cegui_add_dependency(D3DX11EFFECTS)

