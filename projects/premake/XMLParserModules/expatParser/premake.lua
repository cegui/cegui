--
-- CEGUIExpatParser premake script
--

cegui_dynamic("CEGUIExpatParser")

package.files =
{
	matchfiles(pkgdir.."*.cpp"),
	matchfiles(pkgdir.."*.h")
}

include(pkgdir)

library("expat", "_d")
library_static("expat","","_d")

dependency("CEGUIBase")

define("CEGUIEXPATPARSER_EXPORTS")
define("XML_STATIC")
