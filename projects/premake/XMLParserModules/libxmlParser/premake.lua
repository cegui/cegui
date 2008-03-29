--
-- CEGUILibXMLParser premake script
--

cegui_dynamic("CEGUILibXMLParser")

package.files =
{
	matchfiles(rootdir.."cegui/src/"..pkgdir.."*.cpp"),
	matchfiles(rootdir.."cegui/include/"..pkgdir.."*.h")
}

include(rootdir.."cegui/include/"..pkgdir)

library("libxml2")

dependency("CEGUIBase")

define("CEGUILIBXMLPARSER_EXPORTS")
