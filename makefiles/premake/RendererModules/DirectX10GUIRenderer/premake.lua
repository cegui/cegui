--
-- DirectX9GUIRenderer premake script
--

cegui_dynamic("DirectX10GUIRenderer")

package.files =
{
    matchfiles(pkgdir.."*.cpp"),
    matchfiles(pkgdir.."*.h"),
}

include(pkgdir)

library("d3d10")
library("dxerr")
library("d3dx10", "d")

dependency("CEGUIBase")

define("DIRECTX10_GUIRENDERER_EXPORTS")
