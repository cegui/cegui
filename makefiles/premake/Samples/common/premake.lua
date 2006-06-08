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

library("freeglut", "_d")
library("CEGUIBase", "_d")
library("OpenGLGUIRenderer", "_d")
library("DirectX9GUIRenderer", "_d")
--library("DirectX81GUIRenderer")

define("CEGUISAMPLE_EXPORTS")
