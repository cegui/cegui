--
-- NullGUIRenderer premake script
--

cegui_dynamic("CEGUINullRenderer")

package.files =
{
    matchfiles(rootdir.."cegui/src/"..pkgdir.."*.cpp"),
    matchfiles(rootdir.."cegui/include/"..pkgdir.."*.h"),
}

include(rootdir.."cegui/include/"..pkgdir)
include(rootdir)

dependency("CEGUIBase")

define("NULL_GUIRENDERER_EXPORTS")
