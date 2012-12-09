--
-- CEGUITGAImageCodec premake script
--

cegui_dynamic("CEGUITGAImageCodec")

package.files =
{
    matchfiles(rootdir.."cegui/src/"..pkgdir.."*.cpp"),
    matchfiles(rootdir.."cegui/include/"..pkgdir.."*.h"),
}

include(rootdir.."cegui/include/"..pkgdir)

dependency("CEGUIBase")
define("CEGUITGAIMAGECODEC_EXPORTS")

