--
-- IrrlichtRenderer premake script
--

cegui_dynamic("CEGUIIrrlichtRenderer")

package.files =
{
    matchfiles(rootdir.."cegui/src/"..pkgdir.."*.cpp"),
    matchfiles(rootdir.."cegui/include/"..pkgdir.."*.h"),
}

include(rootdir.."cegui/include/"..pkgdir)
include(rootdir)

if IRRLICHT_PATHS then
    add_sdk_paths(IRRLICHT_PATHS)
end

-- The Irrlicht SDK only contains a release version, so no need for '_d' here.
library("Irrlicht")

dependency("CEGUIBase")

define("IRRLICHT_GUIRENDERER_EXPORTS")
