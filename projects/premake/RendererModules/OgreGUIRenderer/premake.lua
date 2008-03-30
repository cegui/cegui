--
-- OgreGUIRenderer premake script
--

cegui_dynamic("OgreGuiRenderer")

package.files =
{
    matchfiles(pkgdir.."*.cpp"),
    matchfiles(pkgdir.."*.h"),
}

include(pkgdir)
include(rootdir)

if OGRE_PATHS then
    add_sdk_paths(OGRE_PATHS)
end

if windows then
    define("NOMINMAX")
end

library("OgreMain", "_d")

dependency("CEGUIBase")

define("OGRE_GUIRENDERER_EXPORTS")
