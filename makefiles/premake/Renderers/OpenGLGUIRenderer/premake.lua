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
include(string.sub(rootdir,1,-2))

library("OpenGL32")
library("GLU32")
library("GLee", "_d")

dependency("CEGUIBase")

if USE_DEVIL_LIBRARY then
    dependency("CEGUIDevILImageCodec")
    define("USE_DEVIL_LIBRARY")
elseif USE_CORONA_LIBRARY then
    library("corona", "_d")
    dependency("CEGUICoronaImageCodec")
    define("USE_CORONA_LIBRARY")
else
    dependency("CEGUITGAImageCodec")
end

define("OPENGL_GUIRENDERER_EXPORTS")
