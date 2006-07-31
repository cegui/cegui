--
-- CEGUISILLYImageCodec premake script
--

cegui_dynamic("CEGUISILLYImageCodec")

package.files =
{
    matchfiles(pkgdir.."*.cpp"),
    matchfiles(pkgdir.."*.h"),
}

library("SILLY", "_d")

dependency("CEGUIBase")
define("CEGUISILLYIMAGECODEC_EXPORTS")
