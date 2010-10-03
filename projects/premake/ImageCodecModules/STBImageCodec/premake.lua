--
-- CEGUISTBImageCodec premake script
--

cegui_dynamic("CEGUISTBImageCodec")

package.files =
{
    matchfiles(rootdir.."cegui/src/"..pkgdir.."*.cpp"),
    matchfiles(rootdir.."cegui/include/"..pkgdir.."*.h"),
}

include(rootdir.."cegui/include/"..pkgdir)

dependency("CEGUIBase")
define("CEGUISTBIMAGECODEC_EXPORTS")

