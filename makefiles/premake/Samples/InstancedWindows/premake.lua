--
-- CEGUI sample premake script
--
sample("InstancedWindows","lua")
package.files =
{
    matchfiles(pkgdir.."src/*.cpp"),
    matchfiles(pkgdir.."include/*.h")
}

library_static("lua","", "_d")
