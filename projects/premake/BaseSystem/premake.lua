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

if not CEGUI_BIDI_SUPPORT then
    tinsert(package.excludes, rootdir.."cegui/src/CEGUIFribidiVisualMapping.cpp")
    tinsert(package.excludes, rootdir.."cegui/src/CEGUIMinibidiVisualMapping.cpp")
else
    if CEGUI_USE_MINIBIDI then
        tinsert(package.excludes, rootdir.."cegui/src/CEGUIFribidiVisualMapping.cpp")
    else
        tinsert(package.excludes, rootdir.."cegui/src/CEGUIMinibidiVisualMapping.cpp")
    end
end

if not CEGUI_USE_EXPRESSION_DIM then
    tinsert(package.excludes, matchfiles(rootdir.."cegui/src/falagard/CEGUIFalExpressionDim*"))
    tinsert(package.excludes, matchfiles(rootdir.."cegui/include/falagard/CEGUIFalExpressionDim*"))
else
    -- match the additional bison generated c++ parser skeleton files
    tinsert(package.files, matchfiles(rootdir.."cegui/src/falagard/*.hh"))
    tinsert(package.files, matchfiles(rootdir.."cegui/src/falagard/*.hpp"))
    -- match the bison / flex input sources
    -- (this way they show up in the files list for easy editing)
    tinsert(package.files, matchfiles(rootdir.."cegui/src/falagard/*.y"))
    tinsert(package.files, matchfiles(rootdir.."cegui/src/falagard/*.l"))
end

library("Winmm", "")

define("CEGUIBASE_EXPORTS")

-- always define this (even though we way not compile what uses it)
define("YY_NO_UNISTD_H")
