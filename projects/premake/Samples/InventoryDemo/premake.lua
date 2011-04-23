--
-- CEGUI sample premake script
--
sample("InventoryDemo")
package.files =
{
    matchfiles(rootdir..pkgdir.."*.cpp"),
    matchfiles(rootdir..pkgdir.."*.h"),
}

include(rootdir..pkgdir)
