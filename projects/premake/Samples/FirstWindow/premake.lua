--
-- CEGUI sample premake script
--
sample("FirstWindow")
package.files =
{
    matchfiles(rootdir..pkgdir.."*.cpp"),
    matchfiles(rootdir..pkgdir.."*.h"),
}

include(rootdir..pkgdir)
