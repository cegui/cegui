--
-- CEGUIBase premake script
--

cegui_dynamic("CEGUIBase")

package.files =
{
    matchfiles(rootdir.."src/*.cpp"),
    matchfiles(rootdir.."include/*.h"),
    matchfiles(rootdir.."src/elements/*.cpp"),
    matchfiles(rootdir.."include/elements/*.h"),
    matchfiles(rootdir.."src/falagard/*.cpp"),
    matchfiles(rootdir.."include/falagard/*.h"),
}

-- We need to remove edit-and-continue in VS2003 (7.1) or older versions because 
-- of the __LINE__ expansion macro bug (see http://support.microsoft.com/kb/199057/en)
if target=="vs2002" or target=="vs2003" then
	package.buildflags = { "no-edit-and-continue" }
end

library("freetype221","_D")
library("pcre", "_d")

define("CEGUIBASE_EXPORTS")
define("PCRE_STATIC")
