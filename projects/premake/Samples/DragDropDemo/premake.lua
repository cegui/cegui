--
-- CEGUI sample premake script
--
sample("DragDropDemo")
package.files =
{
    matchfiles(pkgdir.."*.cpp"),
    matchfiles(pkgdir.."*.h"),
}

include(pkgdir)
