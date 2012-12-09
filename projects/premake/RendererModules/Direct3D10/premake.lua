--
-- Direct3D10GUIRenderer premake script
--

cegui_dynamic("CEGUIDirect3D10Renderer")

package.files =
{
    matchfiles(rootdir.."cegui/src/"..pkgdir.."*.cpp"),
    matchfiles(rootdir.."cegui/include/"..pkgdir.."*.h"),
}

include(rootdir.."cegui/include/"..pkgdir)
include(rootdir)

library("d3d10")
library("dxerr")
library("d3dx10", "d")

dependency("CEGUIBase")

define("DIRECT3D10_GUIRENDERER_EXPORTS")
