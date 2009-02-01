--
-- OpenGLGUIRenderer premake script
--

cegui_dynamic("OpenGLGUIRenderer")

package.files =
{
    matchfiles(pkgdir.."*.cpp"),
    matchfiles(pkgdir.."*.h"),
    matchfiles(pkgdir.."GLEW/src/*.c"),
    matchfiles(pkgdir.."GLEW/GL/*.h"),
}

-- Windows does not have GLX, and is not an Apple ;)
package.excludes =
{
    pkgdir.."CEGUIOpenGLGLXPBTextureTarget.cpp",
    pkgdir.."CEGUIOpenGLGLXPBTextureTarget.h",
    pkgdir.."CEGUIOpenGLApplePBTextureTarget.cpp",
    pkgdir.."CEGUIOpenGLApplePBTextureTarget.h",
}

include(pkgdir)
include(pkgdir.."GLEW")
include(rootdir)

if windows then
    library("GLU32")
    library("OpenGL32")
    define("NOMINMAX")
end

dependency("CEGUIBase")

define("OPENGL_GUIRENDERER_EXPORTS")
define("GLEW_STATIC")
