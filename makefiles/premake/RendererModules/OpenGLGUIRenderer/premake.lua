--
-- OpenGLGUIRenderer premake script
--

cegui_dynamic("OpenGLGUIRenderer")

package.files =
{
    matchfiles(pkgdir.."*.cpp"),
    matchfiles(pkgdir.."*.h"),
}

include(pkgdir)
include(rootdir)

if windows then
    library("GLU32")
    library("OpenGL32")
    define("NOMINMAX")
end

dependency("CEGUIBase")

define("OPENGL_GUIRENDERER_EXPORTS")
