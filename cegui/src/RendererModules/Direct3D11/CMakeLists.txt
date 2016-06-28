set (CEGUI_TARGET_NAME ${CEGUI_DIRECT3D11_RENDERER_LIBNAME})

if (NOT MINGW)
    include_directories(${DIRECTXSDK_INCLUDE_DIR})
    link_directories(${DIRECTXSDK_LIBRARY_DIR})
endif ()

cegui_gather_files()
cegui_add_library(${CEGUI_TARGET_NAME} CORE_SOURCE_FILES CORE_HEADER_FILES)

cegui_add_dependency( ${CEGUI_TARGET_NAME} D3DX11EFFECTS PRIVATE TRUE)
cegui_target_link_libraries(${CEGUI_TARGET_NAME} ${CEGUI_BASE_LIBNAME} d3d11 )
if (MINGW)
    cegui_target_link_libraries(${CEGUI_TARGET_NAME} d3dx11_43 d3dx10_43 d3dcompiler_43 )
else ()
    cegui_target_link_libraries(${CEGUI_TARGET_NAME} d3dcompiler )
    cegui_target_link_libraries(${CEGUI_TARGET_NAME} optimized d3dx11 d3dx10 debug d3dx11d d3dx10d)
endif ()