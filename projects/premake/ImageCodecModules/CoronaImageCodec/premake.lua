--
-- CEGUICoronaImageCodec premake script
--

cegui_dynamic("CEGUICoronaImageCodec")

package.files =
{
    matchfiles(rootdir.."cegui/src/"..pkgdir.."*.cpp"),
    matchfiles(rootdir.."cegui/include/"..pkgdir.."*.h"),
}

include(rootdir.."cegui/include/"..pkgdir)

library("corona", "_d")

dependency("CEGUIBase")

define("CEGUICORONAIMAGECODEC_EXPORTS")

