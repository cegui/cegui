--
-- CEGUI premake script
-- 

require("helpers.lua")

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

createpackages {
    ["Samples"] =
    {
        "common",
        "FirstWindow",
        "Demo4",
        "Demo6",
        "Demo7",
        "Demo8",
        "FalagardDemo1",
        "Medieval",
    }
}

-- the end
