--
-- CEGUIXercesParser premake script
--

cegui_dynamic("CEGUIXercesParser")

package.files =
{
	matchfiles(pkgdir.."*.cpp"),
	matchfiles(pkgdir.."*.h"),
}

include(pkgdir)

library("xerces-c_2","D")

dependency("CEGUIBase")

define("CEGUIXERCESPARSER_EXPORTS")
