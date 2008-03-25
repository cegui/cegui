--
-- tolua++cegui premake script
--

cegui_dynamic("tolua++cegui", "c", "exe")

package.files =
{
	matchfiles(pkgdir.."*.c"),
	matchfiles(pkgdir.."*.h"),
}

--include(pkgdir)
include(pkgdir.."../tolua++")

dependency("tolua++")
library("lua","_d")
library_static("lua","", "_d")

if TOLUA_STATIC then
    define("TOLUA_STATIC")
end
