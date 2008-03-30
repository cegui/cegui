--
-- OpenGLGUIRenderer premake script
--

cegui_dynamic("OpenGLGUIRenderer")

package.files =
{
    matchfiles(rootdir.."cegui/src/"..pkgdir.."*.cpp"),
    matchfiles(rootdir.."cegui/include/"..pkgdir.."*.h"),
}

-- Windows does not have GLX
package.excludes =
{
    pkgdir.."CEGUIOpenGLGLXPBTextureTarget.cpp",
    pkgdir.."CEGUIOpenGLGLXPBTextureTarget.h",
}

include(rootdir.."cegui/include/"..pkgdir)
include(rootdir)

if windows then
    library("GLU32")
    library("OpenGL32")
    define("NOMINMAX")
end

dependency("CEGUIBase")

define("OPENGL_GUIRENDERER_EXPORTS")
