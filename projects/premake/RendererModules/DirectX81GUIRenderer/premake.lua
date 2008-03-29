--
-- DirectX81GUIRenderer premake script
--

cegui_dynamic("DirectX81GUIRenderer")

package.files =
{
    matchfiles(pkgdir.."*.cpp"),
    matchfiles(pkgdir.."*.h"),
}

include(pkgdir)

library("dxerr8")
library("d3dx8", "d")

dependency("CEGUIBase")

tinsert(package.linkoptions, "/NODEFAULTLIB:libci.lib")

define("DIRECTX81_GUIRENDERER_EXPORTS")
