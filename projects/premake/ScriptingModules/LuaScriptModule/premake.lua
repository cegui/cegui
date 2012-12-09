--
-- CEGUILua premake script
--

cegui_dynamic("CEGUILuaScriptModule")

package.files =
{
	matchfiles(rootdir.."cegui/src/"..pkgdir.."*.cpp"),
	matchfiles(rootdir.."cegui/src/"..pkgdir.."*.h"),
	matchfiles(rootdir.."cegui/include/"..pkgdir.."*.h"),
}

include(rootdir.."cegui/include/"..pkgdir)
include(rootdir.."cegui/include/"..pkgdir.."support/tolua++")

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
