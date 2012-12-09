--
-- SampleHelper premake script
--

cegui_dynamic("CEGUISampleHelper")

if CEGUI_CORE_LIBRARY_SOLUTION then
    package.bindir = rootdir.."bin"
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
        dependency("CEGUIOpenGLRenderer")
    else
        library("CEGUIOpenGLRenderer", DEBUG_DLL_SUFFIX or "")
    end

	if WANT_STATIC_BUILD and not STATIC_BUILD_WITH_DYNAMIC_DEPS then
		define("FREEGLUT_STATIC", "Release_Static")
		define("FREEGLUT_STATIC", "Debug_Static")
	end
end

if DIRECT3D9_RENDERER and SAMPLES_DX9 then
    if CEGUI_CORE_LIBRARY_SOLUTION then
        dependency("CEGUIDirect3D9Renderer")
    else
        library("CEGUIDirect3D9Renderer", DEBUG_DLL_SUFFIX or "")
    end
end

if DIRECT3D10_RENDERER and SAMPLES_DX10 then
    if CEGUI_CORE_LIBRARY_SOLUTION then
        dependency("CEGUIDirect3D10Renderer")
    else
        library("CEGUIDirect3D10Renderer", DEBUG_DLL_SUFFIX or "")
    end
end

if IRRLICHT_RENDERER and SAMPLES_IRRLICHT then
    if CEGUI_CORE_LIBRARY_SOLUTION then
        dependency("CEGUIIrrlichtRenderer")
    else
        library("CEGUIIrrlichtRenderer", DEBUG_DLL_SUFFIX or "")
    end

    if IRRLICHT_PATHS then
        add_sdk_paths(IRRLICHT_PATHS)
    end
end

if OGRE_RENDERER and SAMPLES_OGRE then
    if CEGUI_CORE_LIBRARY_SOLUTION then
        dependency("CEGUIOgreRenderer")
    else
        library("CEGUIOgreRenderer", DEBUG_DLL_SUFFIX or "")
    end

    if OGRE_PATHS then
        add_sdk_paths(OGRE_PATHS)
    end
    if OIS_PATHS then
        add_sdk_paths(OIS_PATHS)
    end
    if CEGUI_OLD_OIS_API then
        define("CEGUI_OLD_OIS_API")
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
