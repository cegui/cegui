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

library("DevIL", "_d")
library("ILU", "_d")

dependency("CEGUIBase")
define("CEGUIDEVILIMAGECODEC_EXPORTS")
define("IL_STATIC_LIB", "Debug_Static")
define("IL_STATIC_LIB", "Release_Static")