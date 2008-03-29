--
-- CEGUITinyXMLParser premake script
--

cegui_dynamic("CEGUITinyXMLParser")

package.files =
{
	matchfiles(pkgdir.."*.cpp"),
	matchfiles(pkgdir.."*.h"),
	matchfiles(pkgdir.."ceguitinyxml/*.h"),
	matchfiles(pkgdir.."ceguitinyxml/*.cpp"),
}

include(pkgdir)

dependency("CEGUIBase")

define("CEGUITINYXMLPARSER_EXPORTS")
