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
	matchfiles(rootdir.."cegui/src/"..pkgdir.."*.c"),
	matchfiles(rootdir.."cegui/src/"..pkgdir.."*.h"),
	matchfiles(rootdir.."cegui/include/"..pkgdir.."*.h"),
}

include(rootdir.."cegui/include/"..pkgdir)

library("lua", "_d")
