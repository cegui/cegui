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
include(pkgdir.."../lua_and_tolua++/include")

dependency("lua_and_tolua++")
