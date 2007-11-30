--
-- tolua++ premake script
--

if TOLUA_STATIC then
    cegui_dynamic("tolua++", "c", "lib")
    define("TOLUA_STATIC")
else
    cegui_dynamic("tolua++", "c")
    define("TOLUA_EXPORTS")
end

package.files =
{
	matchfiles(pkgdir.."*.c"),
	matchfiles(pkgdir.."*.h"),
}

library("lua50", "_d")
