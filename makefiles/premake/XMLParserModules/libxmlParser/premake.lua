--
-- CEGUILibXMLParser premake script
--

cegui_dynamic("CEGUILibXMLParser")

package.files =
{
	matchfiles(pkgdir.."*.cpp"),
	matchfiles(pkgdir.."*.h")
}

include(pkgdir)

library("libxml2")

dependency("CEGUIBase")

define("CEGUILIBXMLPARSER_EXPORTS")
