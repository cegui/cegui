--
-- CEGUI sample premake script
--
sample("Demo8", "lua")
package.files =
{
    matchfiles(rootdir..pkgdir.."*.cpp"),
    matchfiles(rootdir..pkgdir.."*.h"),
}

include(rootdir..pkgdir)
library_static("lua","", "_d")