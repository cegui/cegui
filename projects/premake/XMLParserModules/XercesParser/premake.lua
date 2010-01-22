--
-- CEGUIXercesParser premake script
--

cegui_dynamic("CEGUIXercesParser")

package.files =
{
	matchfiles(rootdir.."cegui/src/"..pkgdir.."*.cpp"),
	matchfiles(rootdir.."cegui/include/"..pkgdir.."*.h"),
}

include(rootdir.."cegui/include/"..pkgdir)

library("xerces-c_3","D")

dependency("CEGUIBase")

define("CEGUIXERCESPARSER_EXPORTS")

if WANT_STATIC_BUILD and not STATIC_BUILD_WITH_DYNAMIC_DEPS then
	define("XERCES_STATIC_LIBRARY", "Debug_Static")
	define("XERCES_STATIC_LIBRARY", "Release_Static")
end
