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
#include "CEGuiGLFWSharedBase.h"
#include "SamplesFrameworkBase.h"
#include "CEGUI/CEGUI.h"

//----------------------------------------------------------------------------//
CEGuiGLFWSharedBase* CEGuiGLFWSharedBase::d_appInstance = 0;
const char CEGuiGLFWSharedBase::d_windowTitle[] = "Crazy Eddie's GUI Mk-2 - Sample Application";
double  CEGuiGLFWSharedBase::d_frameTime = 0;
int CEGuiGLFWSharedBase::d_modifiers = 0;
bool CEGuiGLFWSharedBase::d_windowSized = false;
int CEGuiGLFWSharedBase::d_newWindowWidth = CEGuiGLFWSharedBase::s_defaultWindowWidth;
int CEGuiGLFWSharedBase::d_newWindowHeight = CEGuiGLFWSharedBase::s_defaultWindowWidth;
bool CEGuiGLFWSharedBase::d_mouseLeftWindow = false;
bool CEGuiGLFWSharedBase::d_mouseDisableCalled = false;
double CEGuiGLFWSharedBase::d_oldMousePosX = 0.;
double CEGuiGLFWSharedBase::d_oldMousePosY = 0.;

//----------------------------------------------------------------------------//
CEGuiGLFWSharedBase::CEGuiGLFWSharedBase()
{
    if (d_appInstance)
        throw CEGUI::InvalidRequestException(
        "CEGuiGLFWSharedBase instance already exists!");

    d_appInstance = this;

#if defined CEGUI_OPENGL_VER_MAJOR_FORCE
    CEGUI::OpenGLInfo::getSingleton().verForce(CEGUI_OPENGL_VER_MAJOR_FORCE,
                                               CEGUI_OPENGL_VER_MINOR_FORCE);
#endif
}

//----------------------------------------------------------------------------//
void CEGuiGLFWSharedBase::destroyWindow()
{
    glfwTerminate();
}

//----------------------------------------------------------------------------//
void CEGuiGLFWSharedBase::beginRendering(const float /*elapsed*/)
{
    glClear(GL_COLOR_BUFFER_BIT);
}

//----------------------------------------------------------------------------//
void CEGuiGLFWSharedBase::drawFrame()
{
    // calculate time elapsed since last frame
    double time_now = glfwGetTime();
    const double elapsed = time_now - d_frameTime;
    d_frameTime = time_now;

    d_appInstance->renderSingleFrame(static_cast<float>(elapsed));
}

//----------------------------------------------------------------------------//
CEGUI::MouseButton CEGuiGLFWSharedBase::GlfwToCeguiMouseButton(int glfwButton)
{
    switch(glfwButton)
    {
    case GLFW_MOUSE_BUTTON_LEFT     : return CEGUI::LeftButton;
    case GLFW_MOUSE_BUTTON_RIGHT    : return CEGUI::RightButton;
    case GLFW_MOUSE_BUTTON_MIDDLE   : return CEGUI::MiddleButton;
    default                         : return CEGUI::NoButton;
    }
}

//----------------------------------------------------------------------------//
#if GLFW_VERSION_MAJOR >= 3
void CEGuiGLFWSharedBase::glfwCursorPosCallback(GLFWwindow* window, double x, double y)
#else // GLFW_VERSION_MAJOR <= 2
void GLFWCALL CEGuiGLFWSharedBase::glfwMousePosCallback(int x, int y)
#endif
{
    if (!d_mouseDisableCalled)
    {
        // if cursor didn't leave the window nothing changes
        d_sampleApp->injectMousePosition(static_cast<float>(x), static_cast<float>(y));
    }
    else
    {
        // if the cursor left the window, we need to use the saved position
        // because glfw beams the cursor to the middle of the window if 
        // the cursor is disabled
        d_sampleApp->injectMousePosition(static_cast<float>(d_oldMousePosX), static_cast<float>(d_oldMousePosY));
#if GLFW_VERSION_MAJOR >= 3
        glfwSetCursorPos(d_window, d_oldMousePosX, d_oldMousePosY);
#else // GLFW_VERSION_MAJOR <= 2
        glfwSetMousePos(static_cast<int>(d_oldMousePosX), static_cast<int>(d_oldMousePosY));
#endif
        d_mouseDisableCalled = false;
    }

#ifndef DEBUG
    if (x < 0 || y < 0
        || x > d_newWindowWidth
        || y > d_newWindowHeight)
    {
        // show cursor
        glfwEnable(GLFW_MOUSE_CURSOR);

        // move the cursor to the position where it left the window
        glfwSetMousePos(x, y);
        
        // "note down" that the cursor left the window
        d_mouseLeftWindow = true;
    }
    else
    {
        if (d_mouseLeftWindow)
        {
            // get cursor position to restore afterwards
            glfwGetMousePos(&d_oldMousePosX, &d_oldMousePosY);

            // we need to inject the previous cursor position because
            // glfw moves the cursor to the centre of the render 
            // window if it gets disabled. therefore notify the 
            // next MousePosCallback invocation of the "mouse disabled" event.
            d_mouseDisableCalled = true;

            // disable cursor
            glfwDisable(GLFW_MOUSE_CURSOR);

            // "note down" that the cursor is back in the render window
            d_mouseLeftWindow = false;
        }
    }
#endif
}

//----------------------------------------------------------------------------//
void CEGuiGLFWSharedBase::initGLFW()
{
    if (glfwInit() != GL_TRUE)
        CEGUI_THROW(RendererException("Failed to initialise GLFW."));
}
