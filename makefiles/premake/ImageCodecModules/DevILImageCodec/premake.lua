--
-- CEGUIDevILImageCodec premake script
--

cegui_dynamic("CEGUIDevILImageCodec", "c++", "lib")

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
