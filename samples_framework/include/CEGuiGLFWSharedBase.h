/***********************************************************************
    created:    12/2/2012
    author:     Paul D Turner
*************************************************************************/
/***************************************************************************
*   Copyright (C) 2004 - 2012 Paul D Turner & The CEGUI Development Team
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
#ifndef _CEGuiGLFWSharedBase_h_
#define _CEGuiGLFWSharedBase_h_

#include "CEGUISamplesConfig.h"
#include "CEGUI/RendererModules/OpenGL/GL.h"

#if CEGUI_USE_GLFW_VER == 3
#   include <GLFW/glfw3.h>
#   if (GLFW_VERSION_MINOR < 1) && (GLFW_VERSION_PATCH < 1)
#       define GLFW_DONT_CARE -1
#   endif 
#elif CEGUI_USE_GLFW_VER == 2
#   include <GL/glfw.h>
#else
#   error Unsupported "CEGUI_GLFW_VER".
#endif
#include "CEGuiBaseApplication.h"
#include "CEGUI/MouseCursor.h"

class CEGuiGLFWSharedBase : public CEGuiBaseApplication
{
public:
    CEGuiGLFWSharedBase();

protected:
    // implementation of base class abstract functions.
    void run();
    void destroyWindow();
    void beginRendering(const float elapsed);
    void endRendering();

    /*************************************************************************
    Implementation Methods
    *************************************************************************/
    static void initGLFW();
    static void createGLFWWindow();
    static void setGLFWAppConfiguration();

    void drawFrame();

#if GLFW_VERSION_MAJOR >= 3
    
    static void glfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void glfwCharCallback(GLFWwindow* window, unsigned codepoint);
    static void glfwMouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    static void glfwScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
    static void glfwCursorPosCallback(GLFWwindow* window, double xpos, double ypos);

    static void glfwWindowCloseCallback(GLFWwindow* window);
    static void glfwWindowResizeCallback(GLFWwindow* window, int width, int height);

#else // GLFW_VERSION_MAJOR <= 2

    static void GLFWCALL glfwKeyCallback(int key, int action);
    static void GLFWCALL glfwCharCallback(int character, int action);
    static void GLFWCALL glfwMouseButtonCallback(int key, int action);
    static void GLFWCALL glfwMouseWheelCallback(int position);
    static void GLFWCALL glfwMousePosCallback(int x, int y );

    static int GLFWCALL glfwWindowCloseCallback(void);
    static void GLFWCALL glfwWindowResizeCallback(int width, int height);
    
#endif

    static CEGUI::Key::Scan GlfwToCeguiKey(int glfwKey);
    static CEGUI::MouseButton GlfwToCeguiMouseButton(int glfwButton);

    /*************************************************************************
    Data fields
    *************************************************************************/
    static double  d_frameTime;
    static int  d_modifiers;

    static bool d_windowSized;
    static int d_newWindowWidth;
    static int d_newWindowHeight;

    static bool d_mouseLeftWindow;
    static bool d_mouseDisableCalled;
    static double d_oldMousePosX;
    static double d_oldMousePosY;
    
    static const char d_windowTitle[];
    
#if GLFW_VERSION_MAJOR >= 3
    static GLFWwindow* d_window;
#endif
};

#endif  // end of guard _CEGuiGLFWSharedBase_h_
