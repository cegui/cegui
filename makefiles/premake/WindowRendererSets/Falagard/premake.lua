--
-- CEGUIFalgardWRBase premake script
--

cegui_dynamic("CEGUIFalagardWRBase")

package.files =
{
	matchfiles(pkgdir.."src/*.cpp"),
	matchfiles(pkgdir.."include/*.h")
}

include(pkgdir.."include")

dependency("CEGUIBase")

define("FALAGARDWRBASE_EXPORTS")
define("CEGUIWRMODULE_EXPORTS")
