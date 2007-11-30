--
-- CEGUI sample premake script
--
sample("FirstWindow")
package.files =
{
    matchfiles(pkgdir.."src/*.cpp"),
    matchfiles(pkgdir.."include/*.h"),
}

include(pkgdir.."include")
