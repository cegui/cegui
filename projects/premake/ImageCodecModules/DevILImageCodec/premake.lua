--
-- CEGUIDevILImageCodec premake script
--

cegui_dynamic("CEGUIDevILImageCodec")

package.files =
{
    matchfiles(rootdir.."cegui/src/"..pkgdir.."*.cpp"),
    matchfiles(rootdir.."cegui/include/"..pkgdir.."*.h"),
}

include(rootdir.."cegui/include/"..pkgdir)

library("DevIL")
library("ILU")
library("ILUT")

dependency("CEGUIBase")
define("CEGUIDEVILIMAGECODEC_EXPORTS")
