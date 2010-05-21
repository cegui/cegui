--
-- Direct3D11GUIRenderer premake script
--

cegui_dynamic("CEGUIDirect3D11Renderer")

package.files =
{
    matchfiles(rootdir.."cegui/src/"..pkgdir.."*.cpp"),
    matchfiles(rootdir.."cegui/include/"..pkgdir.."*.h"),
}

include(rootdir.."cegui/include/"..pkgdir)
include(rootdir)

library("d3d11")
library("d3dcompiler")
library("dxerr")
library("d3dx10", "d")
library("d3dx11", "d")
library("D3DX11Effects", "D")


dependency("CEGUIBase")

define("DIRECT3D11_GUIRENDERER_EXPORTS")
