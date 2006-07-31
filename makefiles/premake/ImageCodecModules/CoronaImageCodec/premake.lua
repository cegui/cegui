--
-- CEGUICoronaImageCodec premake script
--

cegui_dynamic("CEGUICoronaImageCodec")

package.files =
{
    matchfiles(pkgdir.."*.cpp"),
    matchfiles(pkgdir.."*.h"),
}

include(pkgdir.."include")

library("corona", "_d")

dependency("CEGUIBase")

define("CEGUICORONAIMAGECODEC_EXPORTS")

