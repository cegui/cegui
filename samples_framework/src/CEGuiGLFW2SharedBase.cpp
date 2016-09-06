/***********************************************************************
created:    21/7/2015
author:     Yaron Cohen-Tal
*************************************************************************/
/***************************************************************************
*   Copyright (C) 2004 - 2015 Paul D Turner & The CEGUI Development Team
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

#include <stdexcept>
#include <sstream>

//----------------------------------------------------------------------------//
void CEGuiGLFWSharedBase::run()
{
    // Input callbacks of glfw for CEGUI
    glfwSetKeyCallback(glfwKeyCallback);
    glfwSetCharCallback(glfwCharCallback);
    glfwSetMouseButtonCallback(glfwMouseButtonCallback);
    glfwSetMouseWheelCallback(glfwMouseWheelCallback);
    glfwSetMousePosCallback(glfwMousePosCallback);

    //Window callbacks
    glfwSetWindowCloseCallback(glfwWindowCloseCallback);
    glfwSetWindowSizeCallback(glfwWindowResizeCallback);
    d_windowSized = false; //The resize callback is being called immediately after setting it in this version of glfw
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    // set starting time
    d_frameTime = glfwGetTime();

    while (!d_sampleApp->isQuitting() &&
        glfwGetWindowParam(GLFW_OPENED))
    {
        if (d_windowSized)
        {
            d_windowSized = false;
            CEGUI::System::getSingleton().
                notifyDisplaySizeChanged(
                CEGUI::Sizef(static_cast<float>(d_newWindowWidth),
                static_cast<float>(d_newWindowHeight)));
        }

        drawFrame();
    }
}

//----------------------------------------------------------------------------//
void CEGuiGLFWSharedBase::endRendering()
{
    glfwSwapBuffers();
}

//----------------------------------------------------------------------------//
void CEGuiGLFWSharedBase::createGLFWWindow()
{
    if (glfwOpenWindow(s_defaultWindowWidth, s_defaultWindowHeight, 0, 0, 0, 0,
          24, 8, GLFW_WINDOW) != GL_TRUE)
    {
        CEGUI_THROW(RendererException("Failed to open GLFW window."));
        glfwTerminate();
    }
    glfwEnable (GLFW_KEY_REPEAT);
}

//----------------------------------------------------------------------------//
void CEGuiGLFWSharedBase::setGLFWAppConfiguration()
{
    glfwSetWindowTitle(d_windowTitle);

    //Deactivate VSYNC
    glfwSwapInterval(0);

    // Disable the mouse position in Non_Debug mode
#ifndef DEBUG
    glfwDisable(GLFW_MOUSE_CURSOR);
#endif
    // Clear Errors by GLFW, even if Setup is correct.
    glGetError();
}

//----------------------------------------------------------------------------//
int CEGuiGLFWSharedBase::glfwWindowCloseCallback(void)
{
    d_sampleApp->setQuitting(true);
    return GL_TRUE;
}

//----------------------------------------------------------------------------//
void CEGuiGLFWSharedBase::glfwWindowResizeCallback(int w, int h)
{
    // We cache this in order to minimise calls to notifyDisplaySizeChanged,
    // which happens in the main loop whenever d_windowSized is set to true.
    d_windowSized = true;
    d_newWindowWidth = w;
    d_newWindowHeight = h;
}

//----------------------------------------------------------------------------//
CEGUI::Key::Scan CEGuiGLFWSharedBase::GlfwToCeguiKey(int glfwKey)
{
    switch(glfwKey)
    {
    case GLFW_KEY_ESC       : return CEGUI::Key::Escape;
    case GLFW_KEY_F1        : return CEGUI::Key::F1;
    case GLFW_KEY_F2        : return CEGUI::Key::F2;
    case GLFW_KEY_F3        : return CEGUI::Key::F3;
    case GLFW_KEY_F4        : return CEGUI::Key::F4;
    case GLFW_KEY_F5        : return CEGUI::Key::F5;
    case GLFW_KEY_F6        : return CEGUI::Key::F6;
    case GLFW_KEY_F7        : return CEGUI::Key::F7;
    case GLFW_KEY_F8        : return CEGUI::Key::F8;
    case GLFW_KEY_F9        : return CEGUI::Key::F9;
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
    case GLFW_KEY_KP_ENTER  : return CEGUI::Key::NumpadEnter;
    default                 : return CEGUI::Key::Unknown;
    }
}

//----------------------------------------------------------------------------//
void GLFWCALL CEGuiGLFWSharedBase::glfwKeyCallback(int key, int action)
{
    CEGUI::Key::Scan ceguiKey = GlfwToCeguiKey(key);

    if(action == GLFW_PRESS)
        d_sampleApp->injectKeyDown(ceguiKey);
    else if (action == GLFW_RELEASE)
        d_sampleApp->injectKeyUp(ceguiKey);
}

//----------------------------------------------------------------------------//
void GLFWCALL CEGuiGLFWSharedBase::glfwCharCallback(int character, int action)
{
    if(action == GLFW_PRESS)
        d_sampleApp->injectChar(character);
}

//----------------------------------------------------------------------------//
void GLFWCALL CEGuiGLFWSharedBase::glfwMouseButtonCallback(int key, int action)
{
    CEGUI::MouseButton ceguiMouseButton = GlfwToCeguiMouseButton(key);

    if(action == GLFW_PRESS)
        d_sampleApp->injectMouseButtonDown(ceguiMouseButton);
    else if (action == GLFW_RELEASE)
        d_sampleApp->injectMouseButtonUp(ceguiMouseButton);
}

//----------------------------------------------------------------------------//
void GLFWCALL CEGuiGLFWSharedBase::glfwMouseWheelCallback(int position)
{
    static int lastPosition = 0;
    d_sampleApp->injectMouseWheelChange(static_cast<float>(position - lastPosition));
    lastPosition = position;
}

//----------------------------------------------------------------------------//
void GLFWCALL CEGuiGLFWSharedBase::glfwMousePosCallback(int x, int y)
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
        glfwSetMousePos(static_cast<int>(d_oldMousePosX), static_cast<int>(d_oldMousePosY));
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
            int mouse_x_int(0);
            int mouse_y_int(0);
            glfwGetMousePos(&mouse_x_int, &mouse_y_int);
            d_oldMousePosX = mouse_x_int;
            d_oldMousePosY = mouse_y_int;

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

