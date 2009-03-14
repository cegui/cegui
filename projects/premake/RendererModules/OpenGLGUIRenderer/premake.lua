--
-- OpenGLGUIRenderer premake script
--

cegui_dynamic("OpenGLGUIRenderer")

package.files =
{
    matchfiles(rootdir.."cegui/src/"..pkgdir.."*.cpp"),
    matchfiles(rootdir.."cegui/include/"..pkgdir.."*.h"),
    matchfiles(rootdir.."cegui/src/"..pkgdir.."GLEW/src/*.c"),
    matchfiles(rootdir.."cegui/src/"..pkgdir.."GLEW/GL/*.h"),
}

-- Windows does not have GLX, and is not an Apple ;)
package.excludes =
{
    pkgdir.."CEGUIOpenGLGLXPBTextureTarget.cpp",
    pkgdir.."CEGUIOpenGLGLXPBTextureTarget.h",
    pkgdir.."CEGUIOpenGLApplePBTextureTarget.cpp",
    pkgdir.."CEGUIOpenGLApplePBTextureTarget.h",
}

include(rootdir.."cegui/include/"..pkgdir)
include(rootdir.."cegui/src/"..pkgdir.."GLEW")
include(rootdir)

if windows then
    library("GLU32")
    library("OpenGL32")
    define("NOMINMAX")
end

dependency("CEGUIBase")

define("OPENGL_GUIRENDERER_EXPORTS")
define("GLEW_STATIC")
