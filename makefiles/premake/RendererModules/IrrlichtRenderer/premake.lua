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

library("Irrlicht")

dependency("CEGUIBase")

define("IRRRENDERER_EXPORTS")
