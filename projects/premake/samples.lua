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

	-- minimal setup
	cegui_configs =
    {
    	"Debug",
    	"Release",
    }

	-- optional setup
	if WANT_RELEASE_WITH_SYMBOLS_BUILD then
		tinsert(cegui_configs, "ReleaseWithSymbols")
	end

	if WANT_STATIC_BUILD then
		tinsert(cegui_configs, "Debug_Static")
		tinsert(cegui_configs, "Release_Static")
	end

    -- copy to premake configs
	project.configs = cegui_configs

	-- Set config specific output folders to avoid the problem where a ReleaseWithSymbols build would overwrite
	-- the Release files.
	-- Set config specific output folders to avoid the problem where a ReleaseWithSymbols build would overwrite
-- the Release files.
	if WANT_RELEASE_WITH_SYMBOLS_BUILD then
		project.config["ReleaseWithSymbols"].bindir = rootdir.. "bin/" .. "ReleaseWithSymbols"
		project.config["ReleaseWithSymbols"].libdir = rootdir.. "lib/" .. "ReleaseWithSymbols"
	end

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
        "InventoryDemo",
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
