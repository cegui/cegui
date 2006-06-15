--
-- CEGUISILLYImageCodec premake script
--

cegui_dynamic("CEGUISILLYImageCodec", "c++", "lib")

package.files =
{
    matchfiles(pkgdir.."*.cpp"),
    matchfiles(pkgdir.."*.h"),
}

library("SILLY", "_d")

dependency("CEGUIBase")
