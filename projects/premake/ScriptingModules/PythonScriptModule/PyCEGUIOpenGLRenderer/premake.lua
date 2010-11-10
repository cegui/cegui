--
-- Python extension module(s) premake script
--

cegui_dynamic("PyCEGUIOpenGLRenderer")

package.files =
{
	matchfiles(rootdir.."cegui/src/ScriptingModules/PythonScriptModule/bindings/output/CEGUIOpenGLRenderer/*.cpp"),
	matchfiles(rootdir.."cegui/src/ScriptingModules/PythonScriptModule/bindings/output/CEGUIOpenGLRenderer/*.hpp"),
}

dependency("CEGUIBase")
dependency("CEGUIOpenGLRenderer")

