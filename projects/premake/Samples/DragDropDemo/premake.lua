--
-- CEGUI sample premake script
--
sample("DragDropDemo")
package.files =
{
    matchfiles(rootdir..pkgdir.."*.cpp"),
    matchfiles(rootdir..pkgdir.."*.h"),
}

include(pkgdir)
