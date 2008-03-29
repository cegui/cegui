--
-- CEGUIFreeImageCodec premake script
--

cegui_dynamic("CEGUIFreeImageImageCodec")

package.files =
{
    matchfiles(pkgdir.."*.cpp"),
    matchfiles(pkgdir.."*.h"),
}

include(pkgdir.."include")

library("FreeImage", "d")

dependency("CEGUIBase")
define("CEGUIFREEIMAGEIMAGECODEC_EXPORTS")
