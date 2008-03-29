--
-- CEGUI sample premake script
--
sample("Medieval", "lua")
package.files =
{
    matchfiles(pkgdir.."src/*.cpp"),
    matchfiles(pkgdir.."src/*.h"),
}

include(pkgdir.."src")
