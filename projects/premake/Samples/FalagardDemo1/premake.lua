--
-- CEGUI sample premake script
--
sample("FalagardDemo1")
package.files =
{
    matchfiles(rootdir..pkgdir.."*.cpp"),
    matchfiles(rootdir..pkgdir.."*.h"),
}

include(rootdir..pkgdir)
