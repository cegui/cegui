--
-- CEGUI sample premake script
--
sample("TreeDemo")
package.files =
{
    matchfiles(pkgdir.."src/*.cpp"),
    matchfiles(pkgdir.."include/*.h")
}