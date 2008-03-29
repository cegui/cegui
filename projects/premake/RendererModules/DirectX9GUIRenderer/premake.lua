--
-- DirectX9GUIRenderer premake script
--

cegui_dynamic("DirectX9GUIRenderer")

package.files =
{
    matchfiles(rootdir.."cegui/src/"..pkgdir.."*.cpp"),
    matchfiles(rootdir.."cegui/include/"..pkgdir.."*.h"),
}

include(rootdir.."cegui/include/"..pkgdir)

library("dxerr9")
library("d3dx9", "d")

dependency("CEGUIBase")

define("DIRECTX9_GUIRENDERER_EXPORTS")
