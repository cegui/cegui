--
-- CEGUISILLYImageCodec premake script
--

cegui_dynamic("CEGUISILLYImageCodec")

package.files =
{
    matchfiles(rootdir.."cegui/src/"..pkgdir.."*.cpp"),
    matchfiles(rootdir.."cegui/include/"..pkgdir.."*.h"),
}

include(rootdir.."cegui/include/"..pkgdir)

library("SILLY", "_d")

dependency("CEGUIBase")
define("CEGUISILLYIMAGECODEC_EXPORTS")

if WANT_STATIC_BUILD and not STATIC_BUILD_WITH_DYNAMIC_DEPS then
	define("SILLY_STATIC", "Release_Static")
	define("SILLY_STATIC", "Debug_Static")
end
