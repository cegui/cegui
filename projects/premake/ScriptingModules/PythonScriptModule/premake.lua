--
-- Python extension module(s) premake script
--

cegui_dynamic("PyCEGUI")

package.files =
{
	matchfiles(rootdir.."cegui/src/"..pkgdir.."binding/output/CEGUI/*.cpp"),
	matchfiles(rootdir.."cegui/src/"..pkgdir.."binding/output/CEGUI/*.hpp"),
}

dependency("CEGUIBase")



cegui_dynamic("PyCEGUIOpenGLRenderer")

package.files =
{
	matchfiles(rootdir.."cegui/src/"..pkgdir.."binding/output/CEGUIOpenGLRenderer/*.cpp"),
	matchfiles(rootdir.."cegui/src/"..pkgdir.."binding/output/CEGUIOpenGLRenderer/*.hpp"),
}

dependency("CEGUIBase")
dependency("CEGUIOpenGLRenderer")

