--
-- CEGUI sample premake script
--
sample("Demo8", "lua")
package.files =
{
    matchfiles(pkgdir.."src/*.cpp"),
    matchfiles(pkgdir.."src/*.h"),
}

include(pkgdir.."src")
