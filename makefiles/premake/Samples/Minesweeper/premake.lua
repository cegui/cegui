--
-- CEGUI sample premake script
--
sample("Minesweeper")
package.files =
{
    matchfiles(pkgdir.."src/*.cpp"),
    matchfiles(pkgdir.."src/*.h"),
}