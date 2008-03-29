--
-- CEGUIDevILImageCodec premake script
--

cegui_dynamic("CEGUIDevILImageCodec")

package.files =
{
    matchfiles(pkgdir.."*.cpp"),
    matchfiles(pkgdir.."*.h"),
}

include(pkgdir.."include")

library("DevIL")
library("ILU")
library("ILUT")

dependency("CEGUIBase")
define("CEGUIDEVILIMAGECODEC_EXPORTS")
