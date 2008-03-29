--
-- SampleHelper premake script
--

cegui_dynamic("CEGUISampleHelper")

if CEGUI_CORE_LIBRARY_SOLUTION then
    package.bindir = rootdir.."Samples/bin"
    package.libdir = rootdir.."lib"
end

package.files =
{
	matchfiles(rootdir..pkgdir.."src/*.cpp"),
	matchfiles(rootdir..pkgdir.."include/*.h"),
}

package.excludes =
{
    rootdir..pkgdir.."include/MacCEGuiRendererSelector.h",
    rootdir..pkgdir.."src/MacCEGuiRendererSelector.cpp",
    rootdir..pkgdir.."include/GTK2CEGuiRendererSelector.h",
    rootdir..pkgdir.."src/GTK2CEGuiRendererSelector.cpp",
    rootdir..pkgdir.."include/CLICEGuiRendererSelector.h",
    rootdir..pkgdir.."src/CLICEGuiRendererSelector.cpp",
}

include(rootdir..pkgdir.."include")
include(rootdir.."cegui/include")

if CEGUI_CORE_LIBRARY_SOLUTION then
    dependency("CEGUIBase")
else
    library("CEGUIBase", DEBUG_DLL_SUFFIX or "")
end

if OPENGL_RENDERER and SAMPLES_GL then
    library("freeglut", "_d")
    if CEGUI_CORE_LIBRARY_SOLUTION then
        dependency("OpenGLGUIRenderer")
    else
        library("OpenGLGUIRenderer", DEBUG_DLL_SUFFIX or "")
    end
end

if DIRECTX9_RENDERER and SAMPLES_DX9 then
    if CEGUI_CORE_LIBRARY_SOLUTION then
        dependency("DirectX9GUIRenderer")
    else
        library("DirectX9GUIRenderer", DEBUG_DLL_SUFFIX or "")
    end
end

if DIRECTX81_RENDERER and SAMPLES_DX81 then
    if CEGUI_CORE_LIBRARY_SOLUTION then
        dependency("DirectX81GUIRenderer")
    else
        library("DirectX81GUIRenderer", DEBUG_DLL_SUFFIX or "")
    end
end

if IRRLICHT_RENDERER and SAMPLES_IRRLICHT then
    if CEGUI_CORE_LIBRARY_SOLUTION then
        dependency("IrrlichtRenderer")
    else
        library("IrrlichtRenderer", DEBUG_DLL_SUFFIX or "")
    end

    if IRRLICHT_PATHS then
        add_sdk_paths(IRRLICHT_PATHS)
    end
end

if DEFAULT_XML_PARSER == "xerces" then
    if CEGUI_CORE_LIBRARY_SOLUTION then
        dependency("CEGUIXercesParser")
    else
        library("CEGUIXercesParser", DEBUG_DLL_SUFFIX or "")
    end

    include(rootdir.."XMLParserModules/XercesParser")
end

if windows then
    library("GLU32")
    library("OpenGL32")
end

define("CEGUISAMPLE_EXPORTS")
