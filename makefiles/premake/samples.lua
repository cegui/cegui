--
-- CEGUI premake script
--

-- omit some things if we are part of a single solution along with the core libs
if not CEGUI_CORE_LIBRARY_SOLUTION then
    require("helpers.lua")
    require("config.lua")

    --
    -- project setup
    --

    project.name = "CEGUISamples"
    project.bindir = rootdir.."Samples/bin"
    project.libdir = rootdir.."lib"
    project.configs =
    {
    	"Debug",
    	"ReleaseWithSymbols",
    	"Release",
    }
end

local pkg_table =
{
    ["Samples"] =
    {
        "common",
        "FirstWindow",
        "Demo4",
        "Demo6",
        "Demo7",
        "FalagardDemo1",
        "FontDemo",
        "Minesweeper",
        "ScrollablePaneDemo",
        "TabControlDemo",
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
