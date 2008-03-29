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

library("freetype","_D")
library("pcre", "_d")
library("Winmm", "")

define("CEGUIBASE_EXPORTS")
define("PCRE_STATIC")
