--
-- SampleHelper premake script
--

cegui_dynamic("CEGUISampleHelper")

package.files =
{
	matchfiles(pkgdir.."src/*.cpp"),
	matchfiles(pkgdir.."include/*.h"),
}
package.excludes =
{
    pkgdir.."include/MacCEGuiRendererSelector.h",
    pkgdir.."src/MacCEGuiRendererSelector.cpp",
    pkgdir.."include/GTK2CEGuiRendererSelector.h",
    pkgdir.."src/GTK2CEGuiRendererSelector.cpp",
    pkgdir.."include/CLICEGuiRendererSelector.h",
    pkgdir.."src/CLICEGuiRendererSelector.cpp",
}

include(pkgdir.."include")
include(rootdir)

library("CEGUIBase", "_d")

if SAMPLES_GL then
    library("freeglut", "_d")
    library("OpenGLGUIRenderer", "_d")
end

if DIRECTX9_RENDERER and SAMPLES_DX9 then
    library("DirectX9GUIRenderer", "_d")
end

if DIRECTX81_RENDERER and SAMPLES_DX81 then
    library("DirectX81GUIRenderer")
end

define("CEGUISAMPLE_EXPORTS")
