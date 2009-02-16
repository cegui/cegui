--
-- DirectX9GUIRenderer premake script
--

cegui_dynamic("Direct3D9GUIRenderer")

package.files =
{
    matchfiles(pkgdir.."*.cpp"),
    matchfiles(pkgdir.."*.h"),
}

include(pkgdir)

library("dxerr9")
library("d3dx9", "d")

dependency("CEGUIBase")

define("DIRECT3D9_GUIRENDERER_EXPORTS")
