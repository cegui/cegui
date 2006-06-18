--
-- CEGUIFreeImageCodec premake script
--

cegui_dynamic("CEGUIFreeImageImageCodec", "c++", "lib")

package.files =
{
    matchfiles(pkgdir.."*.cpp"),
    matchfiles(pkgdir.."*.h"),
}

include(pkgdir.."include")

library("FreeImage", "d")

dependency("CEGUIBase")
