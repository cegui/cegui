--
-- IrrlichtRenderer premake script
--

cegui_dynamic("IrrlichtRenderer")

package.files =
{
    matchfiles(pkgdir.."*.cpp"),
    matchfiles(pkgdir.."*.h"),
}

include(pkgdir)
include(rootdir)

if IRRLICHT_PATHS then
    add_sdk_paths(IRRLICHT_PATHS)
end

-- The Irrlicht SDK only contains a release version, so no need for '_d' here.
library("Irrlicht")

dependency("CEGUIBase")

define("IRRRENDERER_EXPORTS")
