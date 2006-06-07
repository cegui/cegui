--
-- DirectX9GUIRenderer premake script
--

cegui_dynamic("DirectX9GUIRenderer")

package.files =
{
    matchfiles(rootdir.."src/renderers/DirectX9GUIRenderer/*.cpp"),
    matchfiles(rootdir.."include/renderers/DirectX9GUIRenderer/*.h"),
}

include(rootdir.."include/renderers/DirectX9GUIRenderer")

library("dxerr9")
library("d3dx9d")

dependency("CEGUIBase")

define("DIRECTX9_GUIRENDERER_EXPORTS")
