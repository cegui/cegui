--
-- CEGUIExpatParser premake script
--

cegui_dynamic("CEGUIExpatParser")

package.files =
{
	matchfiles(rootdir.."cegui/src/"..pkgdir.."*.cpp"),
	matchfiles(rootdir.."cegui/include/"..pkgdir.."*.h")
}

include(rootdir.."cegui/include/"..pkgdir)

library("expat", "_d")
library_static("expat","","_d")

dependency("CEGUIBase")

define("CEGUIEXPATPARSER_EXPORTS")
define("XML_STATIC")
