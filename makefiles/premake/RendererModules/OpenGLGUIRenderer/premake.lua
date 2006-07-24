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
    library("OpenGL32")
    library("GLU32")
    define("NOMINMAX")
end

dependency("CEGUIBase")

if OPENGL_IMAGECODEC == "devil" then
    dependency("CEGUIDevILImageCodec")
    define("USE_DEVIL_LIBRARY")

elseif OPENGL_IMAGECODEC == "freeimage" then
    library("FreeImage", "d")
    dependency("CEGUIFreeImageImageCodec")
    define("USE_FREEIMAGE_LIBRARY")

elseif OPENGL_IMAGECODEC == "corona" then
    library("corona", "_d")
    dependency("CEGUICoronaImageCodec")
    define("USE_CORONA_LIBRARY")

elseif OPENGL_IMAGECODEC == "silly" then
    library("SILLY", "_d")
    dependency("CEGUISILLYImageCodec")
    define("USE_SILLY_LIBRARY")

else
    dependency("CEGUITGAImageCodec")
end

define("OPENGL_GUIRENDERER_EXPORTS")
