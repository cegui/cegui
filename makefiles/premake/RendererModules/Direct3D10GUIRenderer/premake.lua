--
-- Direct3D10GUIRenderer premake script
--

cegui_dynamic("Direct3D10GUIRenderer")

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

define("DIRECT3D10_GUIRENDERER_EXPORTS")
