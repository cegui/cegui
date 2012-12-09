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
    	pkgdir = table.concat(_tpkgdir).."/"
    	pkgparentdir = rootdir..string.gsub(pkgdir, "([^/]+/)$", "")

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
		rootdir.."cegui/include",
		rootdir.."dependencies/include",
	}
	package.defines =
	{
		"_CRT_SECURE_NO_DEPRECATE",
		"HAVE_CONFIG_H",
	}

	-- Extract extra include and lib search paths for this project
	if CEGUI_EXTRA_PATHS then
		for i,v in ipairs(CEGUI_EXTRA_PATHS) do
			if not v[4] or v[4] == name or v[4] == "" then
				add_sdk_paths(v)
			end
		end
	end

	-- debug
	debug = package.config.Debug
	debug.target = name..(DEBUG_DLL_SUFFIX or "")
	debug.defines =
    {
        "_DEBUG"
    }

    if not FULLY_CHECKED_DEBUG_ITERATORS then
        tinsert(debug.defines, "_HAS_ITERATOR_DEBUGGING=0" )
        tinsert(debug.defines, "_SECURE_SCL=0" )
    end

	debug.buildflags = {}

	debug.libpaths =
	{
		rootdir.."dependencies/lib/dynamic",
	}
	
	-- debug_static
	if WANT_STATIC_BUILD then
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

        if not FULLY_CHECKED_DEBUG_ITERATORS then
            tinsert(debug_static.defines, "_HAS_ITERATOR_DEBUGGING=0" )
            tinsert(debug_static.defines, "_SECURE_SCL=0" )
        end

		if STATIC_BUILD_WITH_DYNAMIC_DEPS then
			debug_static.libpaths =
			{
				rootdir.."dependencies/lib/dynamic",
			}
		else
			debug_static.buildflags = 
			{   -- Static build means static runtimes! (/MTd)
				"static-runtime"
			}
			debug_static.libpaths =
			{
				rootdir.."dependencies/lib/static",
			}
		end
	end

	-- release with symbols
	if WANT_RELEASE_WITH_SYMBOLS_BUILD then
		release_sym = package.config.ReleaseWithSymbols
		release_sym.defines = {}
		release_sym.buildflags =
		{
	--    Optimisation disabled for mantis #293
	--    (http://www.cegui.org.uk/mantis/view.php?id=293)
	--    "optimize-speed",
			-- Custom option by the cegui team to support release builds without optimizations
		  "no-optimize-release",
		  "no-edit-and-continue",
		}

		release_sym.libpaths =
		{
			rootdir.."dependencies/lib/dynamic",
		}
	end

	-- release (no symbols)
	release = package.config.Release
	release.defines = {}
	release.buildflags =
	{
	    "no-symbols",
	    "optimize-speed",
	    "no-frame-pointer",
	}

	release.libpaths =
	{
		rootdir.."dependencies/lib/dynamic",
	}
	
	-- release_static (no symbols)
	if WANT_STATIC_BUILD then
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

		if STATIC_BUILD_WITH_DYNAMIC_DEPS then
			release_static.libpaths =
			{
				rootdir.."dependencies/lib/dynamic",
			}
		else
			-- Static build means static runtimes! (/MTd)
			tinsert(release_static.buildflags, "static-runtime")

			release_static.libpaths =
			{
				rootdir.."dependencies/lib/static",
			}
		end
	end
end

--
-- adds a library to the current package
--
function library(name, debugsuffix)
	tinsert(debug.links, name..(debugsuffix or ""))
	tinsert(release.links, name)
	
	if WANT_RELEASE_WITH_SYMBOLS_BUILD then
		tinsert(release_sym.links, name)
	end
end

--
-- adds a CEGUI Static Lib to the current pkg
--
function library_static(name, staticstr, debugsuffix)
	if WANT_STATIC_BUILD then
		tinsert(release_static.links,name..(staticstr or ""))
		tinsert(debug_static.links,name..(staticstr or "")..(debugsuffix or ""))
	end
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
    include(rootdir.."cegui/include/ScriptingModules/LuaScriptModule/support/tolua++")
    include(rootdir.."cegui/include/ScriptingModules/LuaScriptModule")
    if CEGUI_CORE_LIBRARY_SOLUTION then
        dependency("CEGUILuaScriptModule")
        dependency("tolua++")
    else
        library("tolua++", DEBUG_DLL_SUFFIX or "")
        library("CEGUILuaScriptModule", DEBUG_DLL_SUFFIX or "")
        library_static("tolua++", "_Static", DEBUG_DLL_SUFFIX or "")
        library_static("CEGUILuaScriptModule", "_Static", DEBUG_DLL_SUFFIX or "")
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
        package.bindir = rootdir.."bin"
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
-- functions to correctly bring in dependencies and libs for static configs
--
function setup_static_renderer_libs(core_solution)
    -- Renderers
    if OPENGL_RENDERER then
		if core_solution then
			dependency("CEGUIOpenGLRenderer")
		else
	        library_static("CEGUIOpenGLRenderer", "_Static", DEBUG_DLL_SUFFIX or "")
		end
    end
    if DIRECT3D9_RENDERER then
		if core_solution then
			dependency("CEGUIDirect3D9Renderer")
		else
	        library_static("CEGUIDirect3D9Renderer", "_Static", DEBUG_DLL_SUFFIX or "")
		end
    end
    if DIRECT3D10_RENDERER then
		if core_solution then
			dependency("CEGUIDirect3D10Renderer")
		else
	        library_static("CEGUIDirect3D10Renderer", "_Static", DEBUG_DLL_SUFFIX or "")
		end
    end
    if IRRLICHT_RENDERER then
		if core_solution then
			dependency("CEGUIIrrlichtRenderer")
		else
	        library_static("CEGUIIrrlichtRenderer", "_Static", DEBUG_DLL_SUFFIX or "")
		end
        if IRRLICHT_PATHS then
            add_sdk_paths(IRRLICHT_PATHS)
        end
    end
    if OGRE_RENDERER then
		if core_solution then
			dependency("CEGUIOgreRenderer")
		else
	        library_static("CEGUIOgreRenderer", "_Static", DEBUG_DLL_SUFFIX or "")
		end
        if OGRE_PATHS then
            add_sdk_paths(OGRE_PATHS)
        end
    end
    if NULL_RENDERER then
	    if core_solution then
		    dependency("CEGUINullRenderer")
	    else
		    library_static("CEGUINullRenderer", "_Static", DEBUG_DLL_SUFFIX or "")
	    end
    end
end

function setup_static_renderer_dependency_libs()
    -- Warn user when both D3D9 and D3D10 are defined during static builds
    if DIRECT3D9_RENDERER and DIRECT3D10_RENDERER then
        print "D3D9 and D3D10 cannot both be defined for static builds, because it will result in a linker conflict."
    end

    if OPENGL_RENDERER then
        library_static("freeglut", "", "_d")
    end
	
    if DIRECT3D9_RENDERER then
        library_static("dxguid")
        library_static("d3dx9")
        library_static("dxerr")
    end

    if DIRECT3D10_RENDERER then
        library_static("d3d10")
        library_static("dxerr")
        library_static("d3dx10")
    end
end

function setup_static_core_libs()
    if CEGUI_USE_FREETYPE then
        library_static("freetype","","_D")
    end
    if CEGUI_USE_PCRE_REGEX then
	   library_static("pcre","", "_d")
    end
end

function setup_static_xml_parser_libs(core_solution)
    -- Link against the default xml parser
    if DEFAULT_XML_PARSER == "expat" then
		if core_solution then
			dependency("CEGUIExpatParser")
		else
	        library_static("CEGUIExpatParser", "_Static", DEBUG_DLL_SUFFIX or "")
		end
        library_static("expat","","_d")
    end
    if DEFAULT_XML_PARSER == "xerces" then
		if core_solution then
			dependency("CEGUIXercesParser")
		else
	        library_static("CEGUIXercesParser", "_Static", DEBUG_DLL_SUFFIX or "")
		end
		if STATIC_BUILD_WITH_DYNAMIC_DEPS then
			library_static("xerces-c_3","","D")
		else
			library_static("xerces-c_static_3","","D")
		end
    end
    if DEFAULT_XML_PARSER == "tinyxml" then
		if core_solution then
			dependency("CEGUITinyXMLParser")
		else
	        library_static("CEGUITinyXMLParser", "_Static", DEBUG_DLL_SUFFIX or "")
		end
    end
    if DEFAULT_XML_PARSER == "libxml" then
		if core_solution then
			dependency("CEGUILibXMLParser")
		else
	        library_static("CEGUILibXMLParser", "_Static", DEBUG_DLL_SUFFIX or "")
		end
    end

end

function setup_static_image_codec_libs(core_solution)
    -- Link against the default image codec
    if DEFAULT_IMAGE_CODEC == "tga" then
		if core_solution then
			dependency("CEGUITGAImageCodec")
		else
	        library_static("CEGUITGAImageCodec", "_Static", DEBUG_DLL_SUFFIX or "")
		end
    end
    if DEFAULT_IMAGE_CODEC == "stb" then
		if core_solution then
			dependency("CEGUISTBImageCodec")
		else
	        library_static("CEGUISTBImageCodec", "_Static", DEBUG_DLL_SUFFIX or "")
		end
    end
    if DEFAULT_IMAGE_CODEC == "silly" then
		if core_solution then
			dependency("CEGUISILLYImageCodec")
		else
	        library_static("CEGUISILLYImageCodec", "_Static", DEBUG_DLL_SUFFIX or "")
		end
        library_static("SILLY","","_d")

		if not STATIC_BUILD_WITH_DYNAMIC_DEPS then
			library_static("libpng","","_d")
			library_static("jpeg","","_d")
			library_static("zlib","","_d")
		end
    end
    if DEFAULT_IMAGE_CODEC == "devil" then
		if core_solution then
			dependency("CEGUIDevILImageCodec")
		else
	        library_static("CEGUIDevILImageCodec", "_Static", DEBUG_DLL_SUFFIX or "")
		end
        library_static("DevIL", "", "_d")
        library_static("ILU", "", "_d")
		if not STATIC_BUILD_WITH_DYNAMIC_DEPS then
			library_static("libpng","","_d")
			library_static("libmng","","_d")
			library_static("libtiff","","_d")
			library_static("jpeg","","_d")
			library_static("zlib","","_d")
		end
    end
    if DEFAULT_IMAGE_CODEC == "freeimage" then
		if core_solution then
			dependency("CEGUIFreeImageImageCodec")
		else
	        library_static("CEGUIFreeImageImageCodec", "_Static", DEBUG_DLL_SUFFIX or "")
		end
        library_static("FreeImage", "", "d")
    end
    if DEFAULT_IMAGE_CODEC == "corona" then
		if core_solution then
			dependency("CEGUICoronaImageCodec")
		else
			library_static("CEGUICoronaImageCodec", "_Static", DEBUG_DLL_SUFFIX or "")
		end
        library_static("corona", "", "_d")
    end
end

function setup_static_window_renderer_libs(core_solution)
    if FALAGARD_WR then
		if core_solution then
			dependency("CEGUIFalagardWRBase")
		else
	        library_static("CEGUIFalagardWRBase", "_Static", DEBUG_DLL_SUFFIX or "")
		end
    end	    
end

function setup_static_samples()
	setup_static_core_libs()
	setup_static_renderer_dependency_libs()	

	if not CEGUI_CORE_LIBRARY_SOLUTION then
		library_static("CEGUIBase","_Static", DEBUG_DLL_SUFFIX or "")
	end

	setup_static_xml_parser_libs(CEGUI_CORE_LIBRARY_SOLUTION)
	setup_static_image_codec_libs(CEGUI_CORE_LIBRARY_SOLUTION)
	setup_static_renderer_libs(CEGUI_CORE_LIBRARY_SOLUTION)
	setup_static_window_renderer_libs(CEGUI_CORE_LIBRARY_SOLUTION)
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
