/***********************************************************************
filename:   CEGuiOpenGL3BaseApplication.cpp
created:    24/9/2004
author:     Paul D Turner
*************************************************************************/
/***************************************************************************
*   Copyright (C) 2004 - 2009 Paul D Turner & The CEGUI Development Team
*
*   Permission is hereby granted, free of charge, to any person obtaining
*   a copy of this software and associated documentation files (the
*   "Software"), to deal in the Software without restriction, including
*   without limitation the rights to use, copy, modify, merge, publish,
*   distribute, sublicense, and/or sell copies of the Software, and to
*   permit persons to whom the Software is furnished to do so, subject to
*   the following conditions:
*
*   The above copyright notice and this permission notice shall be
*   included in all copies or substantial portions of the Software.
*
*   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
*   EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
*   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
*   IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
*   OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
*   ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
*   OTHER DEALINGS IN THE SOFTWARE.
***************************************************************************/
#ifdef HAVE_CONFIG_H
#   include "config.h"
#endif

#if defined(__linux__) || defined(__FreeBSD__) || defined(__NetBSD__) || defined(__HAIKU__)
# include <unistd.h>
#endif

// this controls conditional compile of file for Apple
#include "CEGUISamplesConfig.h"
#ifdef CEGUI_SAMPLES_USE_OPENGL3

#include "CEGuiOpenGL3BaseApplication.h"
#include "CEGUI/RendererModules/OpenGL3/Renderer.h"
#include "CEGuiSample.h"
#include "CEGUI/CEGUI.h"

#include <stdexcept>
#include <sstream>


/*************************************************************************
Static Data
*************************************************************************/
CEGuiOpenGL3BaseApplication* CEGuiOpenGL3BaseApplication::d_appInstance = 0;
double  CEGuiOpenGL3BaseApplication::d_frameTime = 0;
int CEGuiOpenGL3BaseApplication::d_modifiers = 0;

//----------------------------------------------------------------------------//
CEGuiOpenGL3BaseApplication::CEGuiOpenGL3BaseApplication()
{
	if (d_appInstance)
		throw CEGUI::InvalidRequestException(
		"CEGuiOpenGL3BaseApplication instance already exists!");

	d_appInstance = this;

	//Create OpenGL 3.2 Core Render Context and Window
	initGLFW();

	// create renderer (other option objects are left at defaults)
	d_renderer = &CEGUI::OpenGL3Renderer::create();

	// Set the clear color
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

//----------------------------------------------------------------------------//
CEGuiOpenGL3BaseApplication::~CEGuiOpenGL3BaseApplication()
{
	CEGUI::OpenGL3Renderer::destroy((CEGUI::OpenGL3Renderer&)*d_renderer);
}

//----------------------------------------------------------------------------//
bool CEGuiOpenGL3BaseApplication::execute_impl(CEGuiSample* sampleApp)
{
	sampleApp->initialiseSample();


	// Input callbacks of glfw for CEGUI
	glfwSetKeyCallback(glfwKeyCallback);
	glfwSetCharCallback(glfwCharCallback);
	glfwSetMouseButtonCallback(glfwMouseButtonCallback);
	glfwSetMouseWheelCallback(glfwMouseWheelCallback);
	glfwSetMousePosCallback(glfwMousePosCallback);

	//Window callbacks
	glfwSetWindowSizeCallback(glfwWindowResizeCallback);



	// set starting time
	d_frameTime = glfwGetTime();

	bool running = true;
	while(running)
	{
		drawFrame();

		running = !glfwGetKey( GLFW_KEY_ESC ) &&
			glfwGetWindowParam( GLFW_OPENED );
	}
	glfwTerminate();


	return true;
}

//----------------------------------------------------------------------------//
void CEGuiOpenGL3BaseApplication::cleanup_impl()
{
	// nothing to do here.
}

//----------------------------------------------------------------------------//
void CEGuiOpenGL3BaseApplication::beginRendering(const float /*elapsed*/)
{
	glClear(GL_COLOR_BUFFER_BIT);
}

//----------------------------------------------------------------------------//
void CEGuiOpenGL3BaseApplication::endRendering()
{
	glfwSwapBuffers();
}

//----------------------------------------------------------------------------//
void CEGuiOpenGL3BaseApplication::drawFrame(void)
{
	// calculate time elapsed since last frame
	double time_now = glfwGetTime();
	const double elapsed = time_now - d_frameTime;
	d_frameTime = time_now;

	d_appInstance->renderSingleFrame(float(elapsed));

	// here we check the 'quitting' state and cleanup as required.
	// this is probably not the best way to do this, but since we're
	// using glut, and glutMainLoop can never return, we need some
	// way of checking when to exit.  And this is it...
	if (d_appInstance->isQuitting())
	{
		// cleanup cegui system
		CEGUI::OpenGL3Renderer* renderer =
			static_cast<CEGUI::OpenGL3Renderer*>(
			CEGUI::System::getSingleton().getRenderer());
		CEGUI::System::destroy();
		CEGUI::OpenGL3Renderer::destroy(*renderer);

		// exit
		exit(0);
	}
}

//----------------------------------------------------------------------------//
void CEGuiOpenGL3BaseApplication::glfwWindowResizeCallback(int w, int h)
{
	/*
	glViewport (0, 0, (GLsizei) w, (GLsizei) h);
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	gluPerspective(60.0, (GLfloat) w/(GLfloat) h, 1.0, 50.0);
	glMatrixMode(GL_MODELVIEW);*/

	CEGUI::System::getSingleton().
		notifyDisplaySizeChanged(CEGUI::Sizef((float)w,(float)h));
}


unsigned int CEGuiOpenGL3BaseApplication::GlfwToCeguiKey(int glfwKey)
{
	switch(glfwKey)
	{
	case GLFW_KEY_UNKNOWN	: return 0;
	case GLFW_KEY_ESC		: return CEGUI::Key::Escape;
	case GLFW_KEY_F1		: return CEGUI::Key::F1;
	case GLFW_KEY_F2		: return CEGUI::Key::F2;
	case GLFW_KEY_F3		: return CEGUI::Key::F3;
	case GLFW_KEY_F4		: return CEGUI::Key::F4;
	case GLFW_KEY_F5		: return CEGUI::Key::F5;
	case GLFW_KEY_F6		: return CEGUI::Key::F6;
	case GLFW_KEY_F7		: return CEGUI::Key::F7;
	case GLFW_KEY_F8		: return CEGUI::Key::F8;
	case GLFW_KEY_F9		: return CEGUI::Key::F9;
	case GLFW_KEY_F10       : return CEGUI::Key::F10;
	case GLFW_KEY_F11       : return CEGUI::Key::F11;
	case GLFW_KEY_F12       : return CEGUI::Key::F12;
	case GLFW_KEY_F13       : return CEGUI::Key::F13;
	case GLFW_KEY_F14       : return CEGUI::Key::F14;
	case GLFW_KEY_F15       : return CEGUI::Key::F15;
	case GLFW_KEY_UP        : return CEGUI::Key::ArrowUp;
	case GLFW_KEY_DOWN      : return CEGUI::Key::ArrowDown;
	case GLFW_KEY_LEFT      : return CEGUI::Key::ArrowLeft;
	case GLFW_KEY_RIGHT     : return CEGUI::Key::ArrowRight;
	case GLFW_KEY_LSHIFT    : return CEGUI::Key::LeftShift;
	case GLFW_KEY_RSHIFT    : return CEGUI::Key::RightShift;
	case GLFW_KEY_LCTRL     : return CEGUI::Key::LeftControl;
	case GLFW_KEY_RCTRL     : return CEGUI::Key::RightControl;
	case GLFW_KEY_LALT      : return CEGUI::Key::LeftAlt;
	case GLFW_KEY_RALT      : return CEGUI::Key::RightAlt;
	case GLFW_KEY_TAB       : return CEGUI::Key::Tab;
	case GLFW_KEY_ENTER     : return CEGUI::Key::Return;
	case GLFW_KEY_BACKSPACE : return CEGUI::Key::Backspace;
	case GLFW_KEY_INSERT    : return CEGUI::Key::Insert;
	case GLFW_KEY_DEL       : return CEGUI::Key::Delete;
	case GLFW_KEY_PAGEUP    : return CEGUI::Key::PageUp;
	case GLFW_KEY_PAGEDOWN  : return CEGUI::Key::PageDown;
	case GLFW_KEY_HOME      : return CEGUI::Key::Home;
	case GLFW_KEY_END       : return CEGUI::Key::End;
	case GLFW_KEY_KP_ENTER	: return CEGUI::Key::NumpadEnter;
	default			: return 0;
	}
}

CEGUI::MouseButton CEGuiOpenGL3BaseApplication::GlfwToCeguiMouseButton(int glfwButton)
{
	switch(glfwButton)
	{
	case GLFW_MOUSE_BUTTON_LEFT	: return CEGUI::LeftButton;
	case GLFW_MOUSE_BUTTON_RIGHT	: return CEGUI::RightButton;
	case GLFW_MOUSE_BUTTON_MIDDLE	: return CEGUI::MiddleButton;
	default				: return CEGUI::NoButton;
	}
}


void GLFWCALL CEGuiOpenGL3BaseApplication::glfwKeyCallback(int key, int action)
{
	unsigned int ceguiKey = GlfwToCeguiKey(key);

	if(action == GLFW_PRESS)
		CEGUI::System::getSingleton().injectKeyDown(ceguiKey);
	else if (action == GLFW_RELEASE)
		CEGUI::System::getSingleton().injectKeyUp(ceguiKey);


}

void GLFWCALL CEGuiOpenGL3BaseApplication::glfwCharCallback(int character, int action)
{
	unsigned int ceguiKey = GlfwToCeguiKey(character);

	if(action == GLFW_PRESS)
		CEGUI::System::getSingleton().injectChar(character);
}


void GLFWCALL CEGuiOpenGL3BaseApplication::glfwMouseButtonCallback(int key, int action)
{
	CEGUI::MouseButton ceguiMouseButton = GlfwToCeguiMouseButton(key);

	if(action == GLFW_PRESS)
	{
		CEGUI::System::getSingleton().injectMouseButtonDown(ceguiMouseButton);
	}
	else if (action == GLFW_RELEASE)
		CEGUI::System::getSingleton().injectMouseButtonUp(ceguiMouseButton);
}

void GLFWCALL CEGuiOpenGL3BaseApplication::glfwMouseWheelCallback(int position)
{
	static int lastPosition = 0;
	CEGUI::System::getSingleton().injectMouseWheelChange((float)position - lastPosition);
	lastPosition = position;
}

void GLFWCALL CEGuiOpenGL3BaseApplication::glfwMousePosCallback(int x, int y)
{
	CEGUI::System::getSingleton().injectMousePosition((float)x, (float)y);
}


void CEGuiOpenGL3BaseApplication::initGLFW()
{
	if(!glfwInit())
	{
		throw CEGUI::RendererException(
			"Failed to initialise GLFW.");
		return;
	}

	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 3);
	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 2);
	glfwOpenWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwOpenWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	if(glfwOpenWindow(800, 600, 0, 0, 0, 0, 24, 8, GLFW_WINDOW) != GL_TRUE)
	{
		throw CEGUI::RendererException(
			"Failed to initialise OpenGL window.");
		glfwTerminate();
		return;
	}

	glfwSetWindowTitle("Crazy Eddie's GUI Mk-2 - Sample Application");

	//Deactivate VSYNC
	glfwSwapInterval(0);

	// Disable the mouse position
	//glfwDisable(GLFW_MOUSE_CURSOR);

	// Clear Errors by GLFW, even if Setup is correct.
	glGetError();
}


#endif

