################################################################################
# Custom cmake module for CEGUI to find details about the DX SDK
################################################################################
include(FindPackageHandleStandardArgs)

# MinGW comes with its own DirectX SDK. Therefore, there's no need to look for
# it, or use any include or library directories for it to build.
if (MINGW)

    set (DIRECTXSDK_FOUND TRUE)
    if( CEGUI_MINGW_W64_FOUND AND (CEGUI_MINGW_W64_VER VERSION_LESS 4) )
        set( DIRECTXSDK_MAX_D3D 10 )
    else()
        set (DIRECTXSDK_MAX_D3D 11)
    endif()

else ()

    if (CMAKE_SIZEOF_VOID_P EQUAL 4)
        set (DIRECTXSDK_ARCH x86)
    else()
        set (DIRECTXSDK_ARCH x64)
    endif()

	set(DXSDK_INCLUDE_PATHS
		"$ENV{DXSDK_DIR}/Include"
		"C:/Program Files (x86)/Microsoft DirectX SDK/Include")
	set(DXSDK_LIB_PATHS
		"$ENV{DXSDK_DIR}/Lib/${DIRECTXSDK_ARCH}"
		"C:/Program Files (x86)/Microsoft DirectX SDK/Lib/${DIRECTXSDK_ARCH}")

	if (WIN32)
		GET_FILENAME_COMPONENT(WIN_SDK_ROOT "[HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows Kits\\Installed Roots;KitsRoot10]" ABSOLUTE CACHE)
		list(INSERT DXSDK_INCLUDE_PATHS 0 "${WIN_SDK_ROOT}/Include/${CMAKE_VS_WINDOWS_TARGET_PLATFORM_VERSION}/um")
		list(INSERT DXSDK_LIB_PATHS 0 "${WIN_SDK_ROOT}/Lib/${CMAKE_VS_WINDOWS_TARGET_PLATFORM_VERSION}/um/${DIRECTXSDK_ARCH}")
	endif()

	find_path(DIRECTXSDK_H_PATH
		NAMES d3d11.h
		PATHS ${DXSDK_INCLUDE_PATHS}
		NO_DEFAULT_PATH)
	find_path(DIRECTXSDK_LIB_PATH
		NAMES d3d11.lib
		PATHS ${DXSDK_LIB_PATHS})

    # now test for the specific d3d lib versions and set MAX_D3D accordingly
    if (EXISTS "${DIRECTXSDK_LIB_PATH}/d3d11.lib")
        set (DIRECTXSDK_MAX_D3D 11)
    else()
        set (DIRECTXSDK_MAX_D3D)
    endif()

    find_package_handle_standard_args(DirectXSDK DEFAULT_MSG DIRECTXSDK_LIB_PATH DIRECTXSDK_H_PATH DIRECTXSDK_MAX_D3D)

    mark_as_advanced(DIRECTXSDK_H_PATH DIRECTXSDK_LIB_PATH DIRECTXSDK_ARCH)

    # set up output vars
    if (DIRECTXSDK_FOUND)
        set (DIRECTXSDK_INCLUDE_DIR ${DIRECTXSDK_H_PATH})
        set (DIRECTXSDK_LIBRARY_DIR ${DIRECTXSDK_LIB_PATH})
    else()
        set (DIRECTXSDK_INCLUDE_DIR)
        set (DIRECTXSDK_LIBRARY_DIR)
        set (DIRECTXSDK_MAX_D3D)
    endif()

endif ()
