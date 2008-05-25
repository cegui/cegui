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

if DIRECTX10_RENDERER and SAMPLES_DX10 then
    if CEGUI_CORE_LIBRARY_SOLUTION then
        dependency("DirectX10GUIRenderer")
    else
        library("DirectX10GUIRenderer", DEBUG_DLL_SUFFIX or "")
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
