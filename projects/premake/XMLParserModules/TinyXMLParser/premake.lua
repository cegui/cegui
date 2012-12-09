--
-- CEGUITinyXMLParser premake script
--

cegui_dynamic("CEGUITinyXMLParser")

package.files =
{
	matchfiles(rootdir.."cegui/src/"..pkgdir.."*.cpp"),
	matchfiles(rootdir.."cegui/include/"..pkgdir.."*.h"),
	matchfiles(rootdir.."cegui/src/"..pkgdir.."ceguitinyxml/*.h"),
	matchfiles(rootdir.."cegui/src/"..pkgdir.."ceguitinyxml/*.cpp"),
}

include(rootdir.."cegui/include/"..pkgdir)

dependency("CEGUIBase")

define("CEGUITINYXMLPARSER_EXPORTS")
