--
-- DirectX9GUIRenderer premake script
--

cegui_dynamic("DirectX9GUIRenderer")

package.files =
{
    matchfiles(pkgdir.."*.cpp"),
    matchfiles(pkgdir.."*.h"),
}

include(pkgdir)

library("dxerr9")
library("d3dx9d")

dependency("CEGUIBase")

define("DIRECTX9_GUIRENDERER_EXPORTS")
