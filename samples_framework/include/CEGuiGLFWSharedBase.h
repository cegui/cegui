/***********************************************************************
    filename:   CEGuiGLFWSharedBase.h
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

#include "CEGuiBaseApplication.h"
#include "CEGUI/MouseCursor.h"
#include <GL/glfw.h>

class SamplesFrameworkBase;

class CEGuiGLFWSharedBase : public CEGuiBaseApplication
{
public:
    CEGuiGLFWSharedBase();
    ~CEGuiGLFWSharedBase();

protected:
    // implementation of base class abstract functions.
    bool execute_impl();
    void cleanup_impl();
    void beginRendering(const float elapsed);
    void endRendering();

    /*************************************************************************
    Implementation Methods
    *************************************************************************/
    static void initGLFW();
    static void createGLFWWindow();
    static void setGLFWAppConfiguration();

    void drawFrame();

    static void GLFWCALL glfwKeyCallback(int key, int action);
    static void GLFWCALL glfwCharCallback(int character, int action);
    static void GLFWCALL glfwMouseButtonCallback(int key, int action);
    static void GLFWCALL glfwMouseWheelCallback(int position);
    static void GLFWCALL glfwMousePosCallback(int x, int y );

    static void GLFWCALL glfwWindowResizeCallback(int width, int height);

    static CEGUI::Key::Scan GlfwToCeguiKey(int glfwKey);
    static CEGUI::MouseButton GlfwToCeguiMouseButton(int glfwButton);

    /*************************************************************************
    Data fields
    *************************************************************************/
    static CEGuiGLFWSharedBase* d_appInstance;
    static double  d_frameTime;
    static int  d_modifiers;

    static bool d_windowSized;
    static int d_newWindowWidth;
    static int d_newWindowHeight;
};


#endif  // end of guard _CEGuiGLFWSharedBase_h_

