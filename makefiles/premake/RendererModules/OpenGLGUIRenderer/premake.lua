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

library("OpenGL32")
library("GLU32")
--library("GLee", "_d")

dependency("CEGUIBase")

if USE_DEVIL_LIBRARY then
    dependency("CEGUIDevILImageCodec")
    define("USE_DEVIL_LIBRARY")
elseif USE_CORONA_LIBRARY then
    library("corona", "_d")
    dependency("CEGUICoronaImageCodec")
    define("USE_CORONA_LIBRARY")
elseif USE_SILLY_LIBRARY then
    library("SILLY", "_d")
    dependency("CEGUISILLYImageCodec")
    define("USE_SILLY_LIBRARY")
elseif USE_BUILTIN_TGA then
    dependency("CEGUITGAImageCodec")
    define("USE_BUILTIN_TGA")
else
    error("No image codec specified for the OpenGL renderer module")
end

define("OPENGL_GUIRENDERER_EXPORTS")
