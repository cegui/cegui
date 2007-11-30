--
-- CEGUI sample premake script
--
sample("FalagardDemo1")
package.files =
{
    matchfiles(pkgdir.."src/*.cpp"),
    matchfiles(pkgdir.."include/*.h"),
}

include(pkgdir.."include")
