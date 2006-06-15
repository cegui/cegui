--
-- CEGUI premake script
-- 

dofile("config.lua")
dofile("helpers.lua")

--
-- project setup
--
project.name = "CEGUI"
project.bindir = rootdir.."bin"
project.libdir = rootdir.."lib"
project.configs =
{
	"Debug",
	"ReleaseWithSymbols",
	"Release"
}

createpackages
{
	"BaseSystem",
	
	["WindowRendererSets"] =
	{
		"Falagard",
	},
	
    ["XMLParserModules"] =
	{
		"expatParser",
		--"libxmlParser",
		"TinyXMLParser",
		"XercesParser",
	},
	
	["RendererModules"] =
	{
	    "OpenGLGUIRenderer",
	    "DirectX9GUIRenderer",
	    --"DirectX81GUIRenderer",
	},
	
	["ImageCodecModules"] =
	{
        "CoronaImageCodec",
        "DevILImageCodec",
        "TGAImageCodec",
        "SILLYImageCodec",
	},
	
	["ScriptingModules"] =
	{
        ["CEGUILua"] =
        {
            "lua_and_tolua++",
            "LuaScriptModule",
            "tolua++bin",
        },
	},
}

-- the end
