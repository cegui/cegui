--
-- Python extension module(s) premake script
--

cegui_dynamic("PyCEGUI")

include(rootdir.."cegui/src/ScriptingModules/PythonScriptModule/bindings")
include(rootdir.."cegui/src/ScriptingModules/PythonScriptModule/bindings/output/CEGUI")

package.files =
{
	matchfiles(rootdir.."cegui/src/ScriptingModules/PythonScriptModule/bindings/output/CEGUI/*.cpp"),
	matchfiles(rootdir.."cegui/src/ScriptingModules/PythonScriptModule/bindings/output/CEGUI/*.hpp"),
}

if PYTHON_PATHS then
    add_sdk_paths(PYTHON_PATHS)
end

if BOOST_PYTHON_PATHS then
    add_sdk_paths(BOOST_PYTHON_PATHS)
end

dependency("CEGUIBase")
