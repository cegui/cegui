--
-- CEGUIFalgardWRBase premake script
--

cegui_dynamic("CEGUIFalagardWRBase")

package.files =
{
	matchfiles(rootdir.."cegui/src/"..pkgdir.."*.cpp"),
	matchfiles(rootdir.."cegui/include/"..pkgdir.."*.h")
}

include(rootdir.."cegui/include/"..pkgdir)

dependency("CEGUIBase")

define("FALAGARDWRBASE_EXPORTS")
define("CEGUIWRMODULE_EXPORTS")
