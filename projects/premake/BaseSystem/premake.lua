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

if not CEGUI_USE_DEFAULT_LOGGER then
    tinsert(package.excludes, rootdir.."cegui/src/CEGUIDefaultLogger.cpp")
end

if not CEGUI_USE_FREETYPE then
    tinsert(package.excludes, rootdir.."cegui/src/CEGUIFreeTypeFont.cpp")
else
    library("freetype","_D")
end

if not CEGUI_USE_PCRE_REGEX then
    tinsert(package.excludes, rootdir.."cegui/src/CEGUIPCRERegexMatcher.cpp")
else
    library("pcre", "_d")
    define("PCRE_STATIC")
end

library("Winmm", "")

define("CEGUIBASE_EXPORTS")
