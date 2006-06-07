--
-- CEGUILua premake script
--

cegui_dynamic("CEGUILua")

package.files =
{
	matchfiles(pkgdir.."src/*.cpp"),
	matchfiles(pkgdir.."src/*.h"),
	matchfiles(pkgdir.."include/*.h"),
}

include(pkgdir.."include")
include(pkgdir.."../lua_and_tolua++/include")

dependency("CEGUIBase")
dependency("lua_and_tolua++")

define("CEGUILUA_EXPORTS")
