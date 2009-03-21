--
-- CEGUIBase premake script
--

cegui_dynamic("CEGUIBase")

package.files =
{
    matchfiles(rootdir.."cegui/src/*.cpp"),
    matchfiles(rootdir.."cegui/include/*.h"),
    matchfiles(rootdir.."cegui/src/elements/*.cpp"),
    matchfiles(rootdir.."cegui/include/elements/*.h"),
    matchfiles(rootdir.."cegui/src/falagard/*.cpp"),
    matchfiles(rootdir.."cegui/include/falagard/*.h"),
}

-- we do not want to build this directly; it's included elsewhere when needed.
package.excludes =
{
    rootdir.."cegui/src/minibidi.cpp",
}

library("freetype","_D")
library("pcre", "_d")
library("Winmm", "")

define("CEGUIBASE_EXPORTS")
define("PCRE_STATIC")
