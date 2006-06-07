--
-- CEGUI premake helper functions
--

rootdir = "../../"

--
-- do a package easily
--
do
local _tpkgdir = {}
local _tpath = {}
table.insert(_tpath, rootdir)

local _dopackage = dopackage

-- ugly path management :/
function dopackage(name)
    table.insert(_tpath, string.gsub(name, "([^/]+)", "..").."/")
	rootdir = table.concat(_tpath)
	
	table.insert(_tpkgdir, name)
	pkgdir = rootdir..table.concat(_tpkgdir).."/"
	pkgparentdir = string.gsub(pkgdir, "([^/]+/)$", "")
    
    _dopackage(name)
	
    table.remove(_tpath)
	table.remove(_tpkgdir)
end
end

--
-- this functions initialises a default CEGUI project
--
function cegui_dynamic(name, lang, kind)
	package.name = name
	package.kind = kind or "dll"
	package.language = lang or "c++"

	-- defaults
	package.buildflags =
	{
		"no-rtti"
	}
	package.includepaths =
	{
		rootdir.."include",
		rootdir.."dependencies/include",
	}
	package.libpaths =
	{
		rootdir.."dependencies/lib",
	}
	package.defines =
	{
		"_CRT_SECURE_NO_DEPRECATE"
	}

	-- debug
	debug = package.config.Debug
	debug.target = name.."_d"
	debug.defines =
	{
		"_DEBUG"
	}

	-- release with symbols
	release_sym = package.config.ReleaseWithSymbols

	-- release (no symbols)
	release = package.config.Release
	release.buildflags =
	{
	    "no-symbols",
	    "optimize-speed",
	    "no-frame-pointer",
	}
end

--
-- adds a library to the current package
--
function library(name, debugsuffix)
	tinsert(debug.links, name..(debugsuffix or ""))
	tinsert(release.links, name)
	tinsert(release_sym.links, name)
end

--
-- adds include path to the current package
--
function include(path)
	tinsert(package.includepaths, path)
end

--
-- adds defines to the current package
--
function define(def)
	tinsert(package.defines, def)
end

--
-- adds a dependency to the current package
--
function dependency(dep)
	tinsert(package.links, dep)
end

--
-- create the packages presented in table
--
function createpackages(list, prefix)
    local p = prefix or ""
	for k,v in pairs(list) do
		if type(v) ~= "string" then
			createpackages(v,p..k.."/")
		else
			dopackage(p..v)
		end
	end
end

--
-- add lua support to a sample
--
local function lua_sample()
    include(rootdir.."ScriptingModules/CEGUILua/lua_and_tolua++/include")
    include(rootdir.."ScriptingModules/CEGUILua/LuaScriptModule/include")
    library("lua_and_tolua++", "_d")
    library("CEGUILua", "_d")
end

--
-- creates a cegui sample easily
--
function sample(name, ext)
    cegui_dynamic("Sample_"..name, "c++", "exe")
    include(pkgparentdir.."common/include")
    library("CEGUIBase", "_d")
    dependency("CEGUISampleHelper")
    
    -- lua
    if ext == "lua" then
        lua_sample()
    end
end
