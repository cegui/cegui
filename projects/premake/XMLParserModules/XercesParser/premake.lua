--
-- CEGUIXercesParser premake script
--

cegui_dynamic("CEGUIXercesParser")

package.files =
{
	matchfiles(rootdir.."cegui/src/"..pkgdir.."*.cpp"),
	matchfiles(rootdir.."cegui/include/"..pkgdir.."*.h"),
}

include(rootdir.."cegui/include/"..pkgdir)

library("xerces-c_3","D")

dependency("CEGUIBase")

define("CEGUIXERCESPARSER_EXPORTS")
