################################################################################
# Custom cmake module for CEGUI to find details about the DX SDK
################################################################################
include(FindPackageHandleStandardArgs)

# MinGW comes with its own DirectX SDK. Therefore, there's no need to look for
# it, or use any include or library directories for it to build.
if (MINGW)

    set (DIRECTXSDK_FOUND TRUE)
    set (DIRECTXSDK_MAX_D3D 11)

else ()

    if (CMAKE_SIZEOF_VOID_P EQUAL 4)
        set (DIRECTXSDK_ARCH x86)
    else()
        set (DIRECTXSDK_ARCH x64)
    endif()

    find_path(DIRECTXSDK_H_PATH NAMES d3d10.h PATHS ENV DXSDK_DIR PATH_SUFFIXES Include NO_DEFAULT_PATH)
    find_path(DIRECTXSDK_LIB_PATH NAMES d3dx10d.lib PATHS ENV DXSDK_DIR PATH_SUFFIXES "Lib/${DIRECTXSDK_ARCH}")

    # now test for the specific d3d lib versions and set MAX_D3D accordingly
    if (EXISTS "${DIRECTXSDK_LIB_PATH}/d3d11.lib")
        set (DIRECTXSDK_MAX_D3D 11)
    elseif (EXISTS "${DIRECTXSDK_LIB_PATH}/d3d10.lib")
        set (DIRECTXSDK_MAX_D3D 10)
    elseif (EXISTS "${DIRECTXSDK_LIB_PATH}/d3d9.lib")
        set (DIRECTXSDK_MAX_D3D 9)
    else()
        set (DIRECTXSDK_MAX_D3D)
    endif()

    find_package_handle_standard_args(DIRECTXSDK DEFAULT_MSG DIRECTXSDK_LIB_PATH DIRECTXSDK_H_PATH DIRECTXSDK_MAX_D3D)

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
