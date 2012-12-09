--
-- CEGUI premake script
--

dofile("helpers.lua")
dofile("config.lua")

--
-- project setup
--
project.name = "CEGUI"
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
if WANT_RELEASE_WITH_SYMBOLS_BUILD then
	project.config["ReleaseWithSymbols"].bindir = rootdir.. "bin/" .. "ReleaseWithSymbols"
	project.config["ReleaseWithSymbols"].libdir = rootdir.. "lib/" .. "ReleaseWithSymbols"
end

--
-- Package table
--
local pkg_table =
{
    "BaseSystem"
}


--
-- Window Renderers
--
pkg_table.WindowRendererSets = {}

if FALAGARD_WR then
	tinsert(pkg_table.WindowRendererSets, "Falagard")
end



--
-- XML parser modules
--
pkg_table.XMLParserModules = {}

if EXPAT_PARSER then
    tinsert(pkg_table.XMLParserModules, "expatParser")
end

if XERCES_PARSER then
    tinsert(pkg_table.XMLParserModules, "XercesParser")
end

if LIBXML_PARSER then
    tinsert(pkg_table.XMLParserModules, "libxmlParser")
end

if TINYXML_PARSER then
    tinsert(pkg_table.XMLParserModules, "TinyXMLParser")
end

if RAPIDXML_PARSER then
    tinsert(pkg_table.XMLParserModules, "RapidXMLParser")
end

--
-- Renderer modules
--
pkg_table.RendererModules = {}

if OPENGL_RENDERER then
    tinsert(pkg_table.RendererModules, "OpenGL")
end

if DIRECT3D11_RENDERER then
    tinsert(pkg_table.RendererModules, "Direct3D11")
end

if DIRECT3D10_RENDERER then
    tinsert(pkg_table.RendererModules, "Direct3D10")
end

if DIRECT3D9_RENDERER then
    tinsert(pkg_table.RendererModules, "Direct3D9")
end

if IRRLICHT_RENDERER then
    tinsert(pkg_table.RendererModules, "Irrlicht")
end

if OGRE_RENDERER then
    tinsert(pkg_table.RendererModules, "Ogre")
end

if NULL_RENDERER then
    tinsert(pkg_table.RendererModules, "Null")
end

--
-- Image codec modules
--
pkg_table.ImageCodecModules = {}

if CORONA_IMAGE_CODEC then
    tinsert(pkg_table.ImageCodecModules, "CoronaImageCodec")
end

if DEVIL_IMAGE_CODEC then
    tinsert(pkg_table.ImageCodecModules, "DevILImageCodec")
end

if FREEIMAGE_IMAGE_CODEC then
    tinsert(pkg_table.ImageCodecModules, "FreeImageImageCodec")
end

if TGA_IMAGE_CODEC then
    tinsert(pkg_table.ImageCodecModules, "TGAImageCodec")
end

if SILLY_IMAGE_CODEC then
    tinsert(pkg_table.ImageCodecModules, "SILLYImageCodec")
end

if STB_IMAGE_CODEC then
    tinsert(pkg_table.ImageCodecModules, "STBImageCodec")
end

--
-- Scripting modules
--
pkg_table.ScriptingModules = {}

if LUA_SCRIPT_MODULE then
    tinsert(pkg_table.ScriptingModules, "LuaScriptModule")
    pkg_table.ScriptingModules["LuaScriptModule"]=
    {
        "support/tolua++",
        "support/tolua++bin",
    }
end

if PYTHON_EXTENSION_MODULE then
    pkg_table.ScriptingModules.PythonScriptModule = 
    {
        "PyCEGUI"
    }

    if OPENGL_RENDERER then
        tinsert(pkg_table.ScriptingModules.PythonScriptModule, "PyCEGUIOpenGLRenderer")
    end

    if NULL_RENDERER then
        tinsert(pkg_table.ScriptingModules.PythonScriptModule, "PyCEGUINullRenderer")
    end

    if OGRE_RENDERER then
        tinsert(pkg_table.ScriptingModules.PythonScriptModule, "PyCEGUIOgreRenderer")
    end

end

-- finalise
createpackages(pkg_table)

-- generate config.h
do
    local parsers =
    {
        expat    = "ExpatParser",
        xerces   = "XercesParser",
        libxml   = "LibxmlParser",
        rapidxml = "RapidXMLParser",
        tinyxml  = "TinyXMLParser",
    }

    local codecs =
    {
        corona    = "CoronaImageCodec",
        devil     = "DevILImageCodec",
        freeimage = "FreeImageImageCodec",
        tga       = "TGAImageCodec",
        silly     = "SILLYImageCodec",
        stb       = "STBImageCodec",
    }

    config_h = {}
    config_h.CEGUI_DEFAULT_XMLPARSER = parsers[DEFAULT_XML_PARSER]
    config_h.CEGUI_DEFAULT_IMAGE_CODEC = codecs[DEFAULT_IMAGE_CODEC]

    if OPENGL_RENDERER and SAMPLES_GL then
        config_h.CEGUI_SAMPLES_USE_OPENGL = ""
    end
    if DIRECT3D9_RENDERER and SAMPLES_DX9 then
        config_h.CEGUI_SAMPLES_USE_DIRECTX_9 = ""
    end
    if DIRECT3D10_RENDERER and SAMPLES_DX10 then
        config_h.CEGUI_SAMPLES_USE_DIRECTX_10 = ""
    end
    if SAMPLES_OGRE then
        config_h.CEGUI_SAMPLES_USE_OGRE = ""
    end
    if IRRLICHT_RENDERER then
          -- This only used to happen when samples were to build as well
          config_h.CEGUI_IRR_SDK_VERSION = CEGUI_IRR_SDK_VERSION
          if SAMPLES_IRRLICHT then
                config_h.CEGUI_SAMPLES_USE_IRRLICHT = ""
          end
    end

    if DEFAULT_XML_PARSER == "xerces" then
        config_h.CEGUI_WITH_XERCES = "1"
    end

    if DEFAULT_XML_PARSER == "expat" then
        config_h.CEGUI_WITH_EXPAT = "1"
	end

	if DEFAULT_XML_PARSER == "tinyxml" then
	    config_h.CEGUI_WITH_TINYXML = "1"
	end

	if DEFAULT_IMAGE_CODEC == "silly" then
		config_h.CEGUI_CODEC_SILLY = "1"
	elseif DEFAULT_IMAGE_CODEC == "tga" then
		config_h.CEGUI_CODEC_TGA = "1"
	elseif DEFAULT_IMAGE_CODEC == "corona" then
		config_h.CEGUI_CODEC_CORONA = "1"
	elseif DEFAULT_IMAGE_CODEC == "devil" then
		config_h.CEGUI_CODEC_DEVIL = "1"
	elseif DEFAULT_IMAGE_CODEC == "freeimage" then
		config_h.CEGUI_CODEC_FREEIMAGE = "1"
	elseif DEFAULT_IMAGE_CODEC == "stb" then
		config_h.CEGUI_CODEC_STB = "1"
	end

	-- Lua library version
	config_h.CEGUI_LUA_VER = CEGUI_LUA_VER

	-- It is important that if this changes you re run premake inorder to
	-- get the correct window renderer with a static build.
	if DEFAULT_WINDOW_RENDERER == "falagard" then
	    config_h.CEGUI_FALAGARD_RENDERER = ""
	end

	-- Add a couple of values needed for the TinyXML parser to build.
	config_h.CEGUI_TINYXML_H = '"ceguitinyxml/tinyxml.h"'
	config_h.CEGUI_TINYXML_NAMESPACE = "CEGUITinyXML"

    -- Whether or not to use the freetype lib.
    if CEGUI_USE_FREETYPE then
        config_h.CEGUI_HAS_FREETYPE = ""
    end

    -- Whether or not to use the pcre lib.
    if CEGUI_USE_PCRE_REGEX then
        config_h.CEGUI_HAS_PCRE_REGEX = ""
    end

    -- Whether or not to build the CEGUI::DefaultLogger.
    if CEGUI_USE_DEFAULT_LOGGER then
        config_h.CEGUI_HAS_DEFAULT_LOGGER = ""
    end

    -- BiDi configuration
    if CEGUI_BIDI_SUPPORT then
        config_h.CEGUI_BIDI_SUPPORT = ""

        if CEGUI_USE_MINIBIDI then
            config_h.CEGUI_USE_MINIBIDI = ""
        else
            config_h.CEGUI_USE_FRIBIDI = ""
        end
    end

    -- MinizipResourceProvider
    if MINIZIP_RESOURCE_PROVIDER then
        config_h.CEGUI_HAS_MINIZIP_RESOURCE_PROVIDER = ""
    end

    local filename = "../../cegui/include/config.h"
    local f,err = io.open(filename, "w+")

    if not f then error("Unable to write config.h: "..err) end

    f:write("// config.h - generated by premake\n")
    f:write("// do not edit by hand, instead edit config.lua and regenerate\n")
    f:write("// it is important that you use premake recreate this file if changing xml parsers or codecs for the static builds\n")

    for k,v in pairs(config_h) do
        f:write("#define ",k," ",v,"\n")
    end

    -- The build suffix stuff is getting unified with how it's done on linux systems
    -- As such, the defines that affect this need a conditional around them to only apply to debug builds (for now)
    if DEBUG_DLL_SUFFIX then
        f:write("#if defined(_DEBUG) || defined(DEBUG)\n")
        f:write("#   define CEGUI_HAS_BUILD_SUFFIX\n")
        f:write("#   define CEGUI_BUILD_SUFFIX \""..DEBUG_DLL_SUFFIX.."\"\n")
        f:write("#endif\n")
    end

    f:close()
end

-- should we just include all the samples stuff in this solution?
if SAMPLES_INCLUDED then
    CEGUI_CORE_LIBRARY_SOLUTION = true
    dofile("samples.lua")
end

-- the end
