--
-- CEGUI sample premake script
--
sample("Demo6")
package.files =
{
    matchfiles(pkgdir.."src/*.cpp"),
    matchfiles(pkgdir.."src/*.h"),
}

include(pkgdir.."src")
