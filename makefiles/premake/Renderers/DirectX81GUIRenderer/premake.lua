--
-- DirectX81GUIRenderer premake script
--

cegui_dynamic("DirectX81GUIRenderer")

package.files =
{
    matchfiles(rootdir.."src/renderers/DirectX81GUIRenderer/*.cpp"),
    matchfiles(rootdir.."include/renderers/DirectX81GUIRenderer/*.h"),
}

include(rootdir.."include/renderers/DirectX81GUIRenderer")

library("dxerr8")
library("d3dx8")

dependency("CEGUIBase")

define("DIRECTX81_GUIRENDERER_EXPORTS")
