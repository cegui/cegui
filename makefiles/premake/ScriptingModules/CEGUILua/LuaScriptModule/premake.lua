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
include(pkgdir.."../tolua++")

dependency("CEGUIBase")
dependency("tolua++")

library("lua50", "_d")

define("CEGUILUA_EXPORTS")

if TOLUA_STATIC then
    define("TOLUA_STATIC")
end
