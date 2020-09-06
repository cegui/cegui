# This is very simple demo application showing how to use CEGUI with Python3 and OpenGL
# It use:
#  * CEGUI Python bindings with SWIG
#  * glfw (https://pypi.org/project/glfw/) as OpenGL framework

# Created: 2020-08-17
# Author:  Robert Paciorek

# The MIT License
# 
# Copyright (c) 2004 - 2020 Paul D Turner & The CEGUI Development Team
# 
# Permission is hereby granted, free of charge, to any person obtaining a copy of
# this software and associated documentation files (the "Software"), to deal in
# the Software without restriction, including without limitation the rights to
# use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
# the Software, and to permit persons to whom the Software is furnished to do so,
# subject to the following conditions:
# 
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
# 
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
# FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
# COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
# IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
# CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

from OpenGL.GL import *
import glfw

import CEGUI
import CEGUIOpenGL

#
# init OpenGl via glfw
#

glfw.init()
window = glfw.create_window(800, 600, "My OpenGL Window", None, None)
glfw.make_context_current(window)


#
# init and configure CEGUI
#

renderer = CEGUIOpenGL.OpenGLRenderer.bootstrapSystem()

resProv = CEGUI.toDefaultResourceProvider(CEGUI.System.getSingleton().getResourceProvider())
resProv.setResourceGroupDirectory("looknfeels", "../datafiles/looknfeel")
resProv.setResourceGroupDirectory("schemes", "../datafiles/schemes")
resProv.setResourceGroupDirectory("layouts", "../datafiles/layouts")
resProv.setResourceGroupDirectory("imagesets", "../datafiles/imagesets")
resProv.setResourceGroupDirectory("fonts", "../datafiles/fonts")

CEGUI.Font.setDefaultResourceGroup("fonts")
CEGUI.ImageManager.setImagesetDefaultResourceGroup("imagesets")
CEGUI.Scheme.setDefaultResourceGroup("schemes")
CEGUI.WidgetLookManager.setDefaultResourceGroup("looknfeels")
CEGUI.WindowManager.setDefaultResourceGroup("layouts")

CEGUI.SchemeManager.getSingleton().createFromFile("TaharezLook.scheme")
CEGUI.System.getSingleton().setDefaultCursorName("TaharezLook/MouseArrow")

#
# init and configure CEGUIcreate default context and root window
#

context = CEGUI.System.getSingleton().createGUIContext(renderer.getDefaultRenderTarget())
inputAggregator = CEGUI.InputAggregator(context)
mainWindow = CEGUI.WindowManager.getSingleton().createWindow("DefaultWindow", "Sheet")
context.setRootWindow(mainWindow);

#
# add some windows with button callback
#

win = CEGUI.WindowManager.getSingleton().loadLayoutFromFile("Console.layout")
mainWindow.addChild(win);
class Callback(CEGUI.PySubscriber):
	def run(self, n):
		print("hide window")
		win.hide()
		return True; # must return bool
handleHideInfoWin = Callback() # can't be temporary object !!!
CEGUI.toFrameWindow(win).getCloseButton().subscribeEvent( CEGUI.PushButton.EventClicked.c_str(), handleHideInfoWin )

#
# set mouse and keyboard callback
#

def mouseMove(win, x, y):
	#print("move ", x, y)
	inputAggregator.injectMousePosition(x, y)

def mouseClick(win, button, action, mods):
	print("click ", button, action)
	if action == 1:
		inputAggregator.injectMouseButtonDown(button)
	else:
		inputAggregator.injectMouseButtonUp(button)

def keyInput(win, key, scancode, action, mods):
	print("key ", key, scancode, action, mods)
	if action == 1:
		inputAggregator.injectKeyDown(scancode)
		inputAggregator.injectChar(key) # this is too simple approach
	else:
		inputAggregator.injectKeyUp(scancode)

glfw.set_cursor_pos_callback(window, mouseMove)
glfw.set_mouse_button_callback(window, mouseClick)
glfw.set_input_mode(window, glfw.CURSOR, glfw.CURSOR_HIDDEN);
glfw.set_key_callback(window, keyInput)

#
# main loop rendering
#

while not glfw.window_should_close(window):
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT)
	glfw.poll_events()
	CEGUI.System.getSingleton().renderAllGUIContexts()
	glfw.swap_buffers(window)

glfw.terminate()
