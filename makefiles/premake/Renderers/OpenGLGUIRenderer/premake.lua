--
-- OpenGLGUIRenderer premake script
--

cegui_dynamic("OpenGLGUIRenderer")

package.files =
{
    matchfiles(rootdir.."src/renderers/OpenGLGUIRenderer/*.cpp"),
    matchfiles(rootdir.."include/renderers/OpenGLGUIRenderer/*.h"),
}

include(rootdir.."include/renderers/OpenGLGUIRenderer")

library("OpenGL32")
library("GLU32")
library("GLee", "_d")

dependency("CEGUIBase")

define("OPENGL_GUIRENDERER_EXPORTS")
