--
-- CEGUI sample premake script
--
sample("TextDemo")
package.files =
{
    matchfiles(rootdir..pkgdir.."*.cpp"),
    matchfiles(rootdir..pkgdir.."*.h"),
}

include(rootdir..pkgdir)
