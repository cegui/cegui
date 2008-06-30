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

	-- NOTE: We are keeping RTTI information, despite adding a little signature. The reasons
      -- lie in the fact that it is only active in debug builds, helps with 
      -- exception handling (see CELayoutEditor) and allows dynamic casting if so desired.
	-- If you wish to prevent RTTI information, uncomment the following line:
	-- package.buildflags = { "no-rtti" }

	-- We need to remove edit-and-continue in VS2003 (7.1) or older versions because 
	-- of the __LINE__ expansion macro bug (see http://support.microsoft.com/kb/199057/en)
	if target=="vs2002" or target=="vs2003" then
		package.buildflags = { "no-edit-and-continue" }
	end

	-- defaults
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
	
	-- debug_static
	debug_static = package.config.Debug_Static
	debug_static.target = name.."_Static"..(DEBUG_DLL_SUFFIX or "")

	if kind == "dll" then
		debug_static.kind = "lib"
	elseif kind == "" then
	    debug_static.kind = "lib"
	elseif not kind then
	    debug_static.kind = "lib"
	else
	    debug_static.kind = kind
	end
	
	debug_static.defines =
    {
        "_DEBUG",
        "CEGUI_STATIC",
        "TOLUA_STATIC"
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
	
	-- release_static (no symbols)
	release_static = package.config.Release_Static
	release_static.target = name.."_Static"

	if kind == "dll" then
	    release_static.kind = "lib"
	elseif kind == "" then
	    release_static.kind = "lib"
	elseif not kind then
	    release_static.kind = "lib"
	else
		release_static.kind = kind
	end
	
	release_static.defines =
	{
		"CEGUI_STATIC",
		"TOLUA_STATIC"
	}
	release_static.buildflags =
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
-- adds a CEGUI Static Lib to the current pkg
--
function library_static(name, staticstr, debugsuffix)
	tinsert(release_static.links,name..(staticstr or ""))
	tinsert(debug_static.links,name..(staticstr or "")..(debugsuffix or ""))
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
        library_static("tolua++", "_Static", DEBUG_DLL_SUFFIX or "")
        library_static("CEGUILua", "_Static", DEBUG_DLL_SUFFIX or "")
		library_static("lua","","_d")
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
    
	setup_static_samples()
end

--
-- creaes a cegui sample that uses Static Libs
--
function setup_static_samples()

-- All Samples using static compiles will need to link against the dependencies
 	library_static("freetype","","_D")
	library_static("pcre","", "_d")
	
	if DIRECTX9_RENDERER then
		library_static("dxguid")
		library_static("d3dx9")
		library_static("dxerr9")
	end

	if DIRECTX10_RENDERER then
		library_static("d3d10")
		library_static("dxerr")
		library_static("d3dx10")
	end
	

	if CEGUI_CORE_LIBRARY_SOLUTION then
		-- Link against any XML Parsers we are compiling
		if EXPAT_PARSER then
		    dependency("CEGUIExpatParser")
		end
		if XERCES_PARSER then
		    dependency("CEGUIXercesParser")
		end
		if TINYXML_PARSER then
		    dependency("CEGUITinyXMLParser")
		end
		if LIBXML_PARSER then
		    dependency("CEGUILibXMLParser")
		end
		
		-- Image Codecs
		if TGA_IMAGE_CODEC then
		    dependency("CEGUITGAImageCodec")
		end
		if SILLY_IMAGE_CODEC then
		    dependency("CEGUISILLYImageCodec")
		end
		if DEVIL_IMAGE_CODEC then
		    dependency("CEGUIDevILImageCodec")
		end
		if FREEIMAGE_IMAGE_CODEC then
		    dependency("CEGUIFreeImageImageCodec")
		end
		if CORONA_IMAGE_CODEC then
		    dependency("CEGUICoronaImageCodec")
		end
		
		-- Renderers
	    if OPENGL_RENDERER then
	        dependency("OpenGLGUIRenderer")
		end
		if DIRECTX81_RENDERER then
		    dependency("DirectX81GUIRenderer")
		end
		if DIRECTX9_RENDERER then
		    dependency("DirectX9GUIRenderer")
		end
		if DIRECTX10_RENDERER then
		    dependency("DirectX10GUIRenderer")
		end

		if IRRLICHT_RENDERER then
		    dependency("IrrlichtRenderer")
		end
		
		--Window Renderers
		if FALAGARD_WR then
		    dependency("CEGUIFalagardWRBase")
		end
	else

		library_static("CEGUIBase","_Static", DEBUG_DLL_SUFFIX or "")

		-- Link against any XML Parsers we are compiling
  		if DEFAULT_XML_PARSER == "expat" then
		    library_static("CEGUIExpatParser", "_Static", DEBUG_DLL_SUFFIX or "")
		    library_static("expat","","_d")
		end
    	      if DEFAULT_XML_PARSER == "xerces" then
		    library_static("CEGUIXercesParser", "_Static", DEBUG_DLL_SUFFIX or "")
		    library_static("xerces-c_2","","D")
		end
  		if DEFAULT_XML_PARSER == "tinyxml" then
		    library_static("CEGUITinyXMLParser", "_Static", DEBUG_DLL_SUFFIX or "")
		end
  		if DEFAULT_XML_PARSER == "libxml" then
		    library_static("CEGUILibXMLParser", "_Static", DEBUG_DLL_SUFFIX or "")
		end

		-- Image Codecs
		if TGA_IMAGE_CODEC then
		    library_static("CEGUITGAImageCodec", "_Static", DEBUG_DLL_SUFFIX or "")
		end
		if SILLY_IMAGE_CODEC then
		    library_static("CEGUISILLYImageCodec", "_Static", DEBUG_DLL_SUFFIX or "")
		    library_static("SILLY","","_d")
		end
		if DEVIL_IMAGE_CODEC then
		    library_static("CEGUIDevILImageCodec", "_Static", DEBUG_DLL_SUFFIX or "")
		end
		if FREEIMAGE_IMAGE_CODEC then
		    library_static("CEGUIFreeImageImageCodec", "_Static", DEBUG_DLL_SUFFIX or "")
		end
		if CORONA_IMAGE_CODEC then
		    library_static("CEGUICoronaImageCodec", "_Static", DEBUG_DLL_SUFFIX or "")
		end

		-- Renderers
	    if OPENGL_RENDERER then
	        library_static("OpenGLGUIRenderer", "_Static", DEBUG_DLL_SUFFIX or "")
		end
		if DIRECTX81_RENDERER then
		    library_static("DirectX81GUIRenderer", "_Static", DEBUG_DLL_SUFFIX or "")
		end
		if DIRECTX9_RENDERER then
		    library_static("DirectX9GUIRenderer", "_Static", DEBUG_DLL_SUFFIX or "")
		end
		if DIRECTX10_RENDERER then
		    library_static("DirectX10GUIRenderer", "_Static", DEBUG_DLL_SUFFIX or "")
		end
		if IRRLICHT_RENDERER then
		    library_static("IrrlichtRenderer", "_Static", DEBUG_DLL_SUFFIX or "")
                if IRRLICHT_PATHS then
                     add_sdk_paths(IRRLICHT_PATHS)
                end
		end
		
		--Window Renderers
		if FALAGARD_WR then
			library_static("CEGUIFalagardWRBase", "_Static", DEBUG_DLL_SUFFIX or "")
		end
	    
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
