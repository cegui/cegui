set (CEGUI_TARGET_NAME ${CEGUI_DIRECT3D11_RENDERER_LIBNAME})

if (NOT MINGW)
    include_directories(${DIRECTXSDK_INCLUDE_DIR})
    link_directories(${DIRECTXSDK_LIBRARY_DIR})

    # this is required for resolving GUID symbols such as "IID_ID3D11ShaderReflection" on older Direct3D versions, such as in June 2010 SDK
    add_definitions( -DINITGUID )
endif ()

cegui_gather_files()
cegui_add_library(${CEGUI_TARGET_NAME} CORE_SOURCE_FILES CORE_HEADER_FILES)

cegui_add_dependency( ${CEGUI_TARGET_NAME} D3DX11EFFECTS)
cegui_target_link_libraries(${CEGUI_TARGET_NAME} ${CEGUI_BASE_LIBNAME} d3d11 )
if( MINGW )
    cegui_target_link_libraries(${CEGUI_TARGET_NAME} d3dcompiler_43 dxguid )
else()
    cegui_target_link_libraries(${CEGUI_TARGET_NAME} d3dcompiler )
endif()