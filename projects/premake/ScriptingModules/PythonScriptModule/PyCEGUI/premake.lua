--
-- Python extension module(s) premake script
--

cegui_dynamic("PyCEGUI")

package.files =
{
	matchfiles(rootdir.."cegui/src/ScriptingModules/PythonScriptModule/bindings/output/CEGUI/*.cpp"),
	matchfiles(rootdir.."cegui/src/ScriptingModules/PythonScriptModule/bindings/output/CEGUI/*.hpp"),
}

dependency("CEGUIBase")
