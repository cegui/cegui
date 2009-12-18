--
-- CEGUI premake script
--

-- omit some things if we are part of a single solution along with the core libs
if not CEGUI_CORE_LIBRARY_SOLUTION then
    require("helpers")
    require("config")

    --
    -- project setup
    --

    project.name = "CEGUISamples"
    project.bindir = rootdir.."bin"
    project.libdir = rootdir.."lib"

	cegui_configs =
    {
    	"Debug",
    	"Release",
    }

	if WANT_RELEASE_WITH_SYMBOLS_BUILD then
		tinsert(cegui_configs, "ReleaseWithSymbols")
	end

	if WANT_STATIC_BUILD then
		tinsert(cegui_configs, "Debug_Static")
		tinsert(cegui_configs, "Release_Static")
	end

    project.configs = cegui_configs
end

local pkg_table =
{
    ["Samples"] =
    {
        "common",
        "FirstWindow",
        "TextDemo",
        "Demo6",
        "Demo7",
        "DragDropDemo",
        "FalagardDemo1",
        "FontDemo",
        "Minesweeper",
        "ScrollablePaneDemo",
        "TabControlDemo",
        "TreeDemo"
    }
}

local lua_samples =
{
    "Demo8",
}

if LUA_SCRIPT_MODULE then
    for _,v in ipairs(lua_samples) do
        tinsert(pkg_table.Samples, v)
    end
end

createpackages(pkg_table)

-- the end
