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

library("lua", "_d")
-- library_static("lua","","_d")

define("CEGUILUA_EXPORTS")

if not LUA_SCRIPT_MODULE_SAFE then
    define("TOLUA_RELEASE", "Release")
end

if TOLUA_STATIC then
    define("TOLUA_STATIC")
end
