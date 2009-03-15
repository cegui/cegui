--
-- OgreGUIRenderer premake script
--

cegui_dynamic("CEGUIOgreRenderer")

package.files =
{
    matchfiles(rootdir.."cegui/src/"..pkgdir.."*.cpp"),
    matchfiles(rootdir.."cegui/include/"..pkgdir.."*.h"),
}

include(rootdir.."cegui/include/"..pkgdir)
include(rootdir)

if OGRE_PATHS then
    add_sdk_paths(OGRE_PATHS)
end

library("OgreMain", "_d")

dependency("CEGUIBase")

define("OGRE_GUIRENDERER_EXPORTS")
