--
-- CEGUITGAImageCodec premake script
--

cegui_dynamic("CEGUITGAImageCodec", "c++", "lib")

package.files =
{
    matchfiles(pkgdir.."*.cpp"),
    matchfiles(pkgdir.."*.h"),
}

include(pkgdir.."include")

dependency("CEGUIBase")
define("CEGUITGAIMAGECODEC_EXPORTS")

