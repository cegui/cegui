################################################################################
# Custom cmake module for CEGUI to find the D3D 11 Effects lib.
################################################################################
include(FindPackageHandleStandardArgs)

find_path(D3DX11EFFECTS_H_PATH NAMES d3dx11effect.h)
find_library(D3DX11EFFECTS_LIB NAMES D3DX11Effects)
find_library(D3DX11EFFECTS_LIB_DBG NAMES D3DX11EffectsD)

find_package_handle_standard_args(D3DX11EFFECTS DEFAULT_MSG D3DX11EFFECTS_LIB D3DX11EFFECTS_H_PATH)

mark_as_advanced(D3DX11EFFECTS_H_PATH D3DX11EFFECTS_LIB D3DX11EFFECTS_LIB_DBG)

# set up output vars
if (D3DX11EFFECTS_FOUND)
    set (D3DX11EFFECTS_INCLUDE_DIR ${D3DX11EFFECTS_H_PATH})
    set (D3DX11EFFECTS_LIBRARIES ${D3DX11EFFECTS_LIB})
    if (D3DX11EFFECTS_LIB_DBG)
        set (D3DX11EFFECTS_LIBRARIES_DBG ${D3DX11EFFECTS_LIB_DBG})
    endif()
else()
    set (D3DX11EFFECTS_INCLUDE_DIR)
    set (D3DX11EFFECTS_LIBRARIES)
    set (D3DX11EFFECTS_LIBRARIES_DBG)
endif()

