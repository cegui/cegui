--
-- CEGUIRapidXMLParser premake script
--

cegui_dynamic("CEGUIRapidXMLParser")

package.files =
{
	matchfiles(rootdir.."cegui/src/"..pkgdir.."*.cpp"),
	matchfiles(rootdir.."cegui/include/"..pkgdir.."*.h")
}

include(rootdir.."cegui/include/"..pkgdir)

if RAPIDXML_PATHS then
    add_sdk_paths(RAPIDXML_PATHS)
end

dependency("CEGUIBase")

define("CEGUIRAPIDXMLPARSER_EXPORTS")
