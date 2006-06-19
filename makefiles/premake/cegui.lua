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

pkg_table =
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
    },
	
	["ImageCodecModules"] = {},
	
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

local image_codec =
{
    corona="CoronaImageCodec",
    devil="DevILImageCodec",
    freeimage="FreeImageImageCodec", 
    tga="TGAImageCodec",
    silly="SILLYImageCodec",
}
tinsert(pkg_table.ImageCodecModules, image_codec[OPENGL_IMAGECODEC])

if DIRECTX9_RENDERER then
    tinsert(pkg_table.RendererModules, "DirectX9GUIRenderer")
end

if DIRECTX81_RENDERER then
    tinsert(pkg_table.RendererModules, "DirectX81GUIRenderer")
end

createpackages(pkg_table)

-- the end
