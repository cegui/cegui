--
-- Python extension module(s) premake script
--

cegui_dynamic("PyCEGUIOpenGLRenderer")

include(rootdir.."cegui/src/ScriptingModules/PythonScriptModule/bindings")

package.files =
{
	matchfiles(rootdir.."cegui/src/ScriptingModules/PythonScriptModule/bindings/output/CEGUIOpenGLRenderer/*.cpp"),
	matchfiles(rootdir.."cegui/src/ScriptingModules/PythonScriptModule/bindings/output/CEGUIOpenGLRenderer/*.hpp"),
}

if PYTHON_PATHS then
    add_sdk_paths(PYTHON_PATHS)
end

if BOOST_PYTHON_PATHS then
    add_sdk_paths(BOOST_PYTHON_PATHS)
end

dependency("CEGUIBase")
dependency("CEGUIOpenGLRenderer")

