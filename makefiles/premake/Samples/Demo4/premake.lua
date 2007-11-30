--
-- CEGUI sample premake script
--
sample("Demo4")
package.files =
{
    matchfiles(pkgdir.."src/*.cpp"),
    matchfiles(pkgdir.."src/*.h"),
}

include(pkgdir.."src")
