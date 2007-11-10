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

-- We need to remove edit-and-continue in VS2003 (7.1) or older versions because 
-- of the __LINE__ expansion macro bug (see http://support.microsoft.com/kb/199057/en)
if target=="vs2002" or target=="vs2003" then
	package.buildflags = { "no-edit-and-continue" }
end

include(pkgdir.."include")
include(pkgdir.."../tolua++")

dependency("CEGUIBase")
dependency("tolua++")

library("lua50", "_d")

define("CEGUILUA_EXPORTS")

if not LUA_SCRIPT_MODULE_SAFE then
    define("TOLUA_RELEASE", "Release")
end

if TOLUA_STATIC then
    define("TOLUA_STATIC")
end
