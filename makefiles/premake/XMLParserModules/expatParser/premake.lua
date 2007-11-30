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

library("libexpatMT", "_d")

dependency("CEGUIBase")

define("CEGUIEXPATPARSER_EXPORTS")
define("XML_STATIC")
