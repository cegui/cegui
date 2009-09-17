--
-- DirectX9GUIRenderer premake script
--

cegui_dynamic("CEGUIDirect3D9Renderer")

package.files =
{
    matchfiles(rootdir.."cegui/src/"..pkgdir.."*.cpp"),
    matchfiles(rootdir.."cegui/include/"..pkgdir.."*.h"),
}

include(rootdir.."cegui/include/"..pkgdir)
include(rootdir)

library("dxerr")
library("d3dx9", "d")

dependency("CEGUIBase")

define("DIRECT3D9_GUIRENDERER_EXPORTS")
