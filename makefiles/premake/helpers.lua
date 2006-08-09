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
		"_CRT_SECURE_NO_DEPRECATE",
		"HAVE_CONFIG_H",
	}

	-- debug
	debug = package.config.Debug
	debug.target = name..(DEBUG_DLL_SUFFIX or "")
	debug.defines =
    {
        "_DEBUG"
    }
	debug.buildflags = {}

	-- release with symbols
	release_sym = package.config.ReleaseWithSymbols
	release_sym.defines = {}
	release_sym.buildflags =
	{
	   "optimize-speed",
	}


	-- release (no symbols)
	release = package.config.Release
	release.defines = {}
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
-- adds library path to the current package
--
function librarypath(path)
    tinsert(package.libpaths, path)
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
function define(def, conf)
    if conf then
        tinsert(package.config[conf].defines, def)
    else
        tinsert(package.defines, def)
    end
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
    include(rootdir.."ScriptingModules/CEGUILua/tolua++")
    include(rootdir.."ScriptingModules/CEGUILua/LuaScriptModule/include")
    if CEGUI_CORE_LIBRARY_SOLUTION then
        dependency("CEGUILua")
        dependency("tolua++")
    else
        library("tolua++", DEBUG_DLL_SUFFIX or "")
        library("CEGUILua", DEBUG_DLL_SUFFIX or "")
    end
end

--
-- creates a cegui sample easily
--
function sample(name, ext)
    cegui_dynamic("Sample_"..name, "c++", "exe")
    include(pkgparentdir.."common/include")

    dependency("CEGUISampleHelper")

    if CEGUI_CORE_LIBRARY_SOLUTION then
        package.bindir = rootdir.."Samples/bin"
        package.libdir = rootdir.."lib"
        dependency("CEGUIBase")
    else
        library("CEGUIBase", DEBUG_DLL_SUFFIX or "")
    end

    -- lua
    if ext == "lua" then
        if not LUA_SCRIPT_MODULE then error("Cannot build Lua sample without the Lua script module") end
        lua_sample()
    end
end

--
-- returns the include path and library path of some sdk
--
function add_sdk_paths(t)
    local path = t[1]
    local inc_suffix = t[2] or ""
    local lib_suffix = t[3] or ""

    local sfind = string.find

    -- we dont want to worry about backslashes
    path = string.gsub(path, "\\", "/")
    inc_suffix = string.gsub(inc_suffix, "\\", "/")
    lib_suffix = string.gsub(lib_suffix, "\\", "/")

    -- check for absolute path
    if sfind(path, "^/") or sfind(path, "^%a:/") then
        include(path.."/"..inc_suffix)
        librarypath(path.."/"..lib_suffix)
    else
        include(rootdir..path.."/"..inc_suffix)
        librarypath(rootdir..path.."/"..lib_suffix)
    end
end
