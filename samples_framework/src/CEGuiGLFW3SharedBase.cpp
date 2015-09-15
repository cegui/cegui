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

//----------------------------------------------------------------------------//
GLFWwindow* CEGuiGLFWSharedBase::d_window = 0;

//----------------------------------------------------------------------------//
void CEGuiGLFWSharedBase::run()
{
    // Input callbacks of glfw for CEGUI
    glfwSetKeyCallback(d_window, glfwKeyCallback);
    glfwSetCharCallback(d_window, glfwCharCallback);
    glfwSetMouseButtonCallback(d_window, glfwMouseButtonCallback);
    glfwSetScrollCallback(d_window, glfwScrollCallback);
    glfwSetCursorPosCallback(d_window, glfwCursorPosCallback);

    //Window callbacks
    glfwSetWindowCloseCallback(d_window, glfwWindowCloseCallback);
    glfwSetWindowSizeCallback(d_window, glfwWindowResizeCallback);
    d_windowSized = false; //The resize callback is being called immediately after setting it in this version of glfw
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    // set starting time
    d_frameTime = glfwGetTime();

    while (!d_sampleApp->isQuitting())
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
        glfwPollEvents();
    }

}

//----------------------------------------------------------------------------//
void CEGuiGLFWSharedBase::endRendering()
{
    glfwSwapBuffers(d_window);
}

//----------------------------------------------------------------------------//
void CEGuiGLFWSharedBase::createGLFWWindow()
{
    glfwWindowHint(GLFW_RED_BITS,   GLFW_DONT_CARE);
    glfwWindowHint(GLFW_GREEN_BITS, GLFW_DONT_CARE);
    glfwWindowHint(GLFW_BLUE_BITS,  GLFW_DONT_CARE);
    glfwWindowHint(GLFW_ALPHA_BITS, 0);
    if (!(d_window = glfwCreateWindow(s_defaultWindowWidth,
            s_defaultWindowHeight, d_windowTitle, 0, 0)))
    {
        CEGUI_THROW(RendererException("Failed to open GLFW window."));
        glfwTerminate();
    }
    glfwMakeContextCurrent (d_window);
}

//----------------------------------------------------------------------------//
void CEGuiGLFWSharedBase::setGLFWAppConfiguration()
{
    //Deactivate VSYNC
    glfwSwapInterval(0);

    // Disable the mouse position in Non_Debug mode
#ifndef DEBUG
    glfwSetInputMode(d_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
#endif
    // Clear Errors by GLFW, even if Setup is correct.
    glGetError();
}

//----------------------------------------------------------------------------//
void CEGuiGLFWSharedBase::glfwWindowCloseCallback(GLFWwindow* /*window*/)
{
    d_sampleApp->setQuitting(true);
}

//----------------------------------------------------------------------------//
void CEGuiGLFWSharedBase::glfwWindowResizeCallback(GLFWwindow* /*window*/, int width, int height)
{
    // We cache this in order to minimise calls to notifyDisplaySizeChanged,
    // which happens in the main loop whenever d_windowSized is set to true.
    d_windowSized = true;
    d_newWindowWidth = width;
    d_newWindowHeight = height;
}

//----------------------------------------------------------------------------//
CEGUI::Key::Scan CEGuiGLFWSharedBase::GlfwToCeguiKey(int glfwKey)
{
    switch(glfwKey)
    {
    case GLFW_KEY_ESCAPE       : return CEGUI::Key::Escape;
    case GLFW_KEY_F1           : return CEGUI::Key::F1;
    case GLFW_KEY_F2           : return CEGUI::Key::F2;
    case GLFW_KEY_F3           : return CEGUI::Key::F3;
    case GLFW_KEY_F4           : return CEGUI::Key::F4;
    case GLFW_KEY_F5           : return CEGUI::Key::F5;
    case GLFW_KEY_F6           : return CEGUI::Key::F6;
    case GLFW_KEY_F7           : return CEGUI::Key::F7;
    case GLFW_KEY_F8           : return CEGUI::Key::F8;
    case GLFW_KEY_F9           : return CEGUI::Key::F9;
    case GLFW_KEY_F10          : return CEGUI::Key::F10;
    case GLFW_KEY_F11          : return CEGUI::Key::F11;
    case GLFW_KEY_F12          : return CEGUI::Key::F12;
    case GLFW_KEY_F13          : return CEGUI::Key::F13;
    case GLFW_KEY_F14          : return CEGUI::Key::F14;
    case GLFW_KEY_F15          : return CEGUI::Key::F15;
    case GLFW_KEY_UP           : return CEGUI::Key::ArrowUp;
    case GLFW_KEY_DOWN         : return CEGUI::Key::ArrowDown;
    case GLFW_KEY_LEFT         : return CEGUI::Key::ArrowLeft;
    case GLFW_KEY_RIGHT        : return CEGUI::Key::ArrowRight;
    case GLFW_KEY_LEFT_SHIFT   : return CEGUI::Key::LeftShift;
    case GLFW_KEY_RIGHT_SHIFT  : return CEGUI::Key::RightShift;
    case GLFW_KEY_LEFT_CONTROL : return CEGUI::Key::LeftControl;
    case GLFW_KEY_RIGHT_CONTROL: return CEGUI::Key::RightControl;
    case GLFW_KEY_LEFT_ALT     : return CEGUI::Key::LeftAlt;
    case GLFW_KEY_RIGHT_ALT    : return CEGUI::Key::RightAlt;
    case GLFW_KEY_TAB          : return CEGUI::Key::Tab;
    case GLFW_KEY_ENTER        : return CEGUI::Key::Return;
    case GLFW_KEY_BACKSPACE    : return CEGUI::Key::Backspace;
    case GLFW_KEY_INSERT       : return CEGUI::Key::Insert;
    case GLFW_KEY_DELETE       : return CEGUI::Key::Delete;
    case GLFW_KEY_PAGE_UP      : return CEGUI::Key::PageUp;
    case GLFW_KEY_PAGE_DOWN    : return CEGUI::Key::PageDown;
    case GLFW_KEY_HOME         : return CEGUI::Key::Home;
    case GLFW_KEY_END          : return CEGUI::Key::End;
    case GLFW_KEY_KP_ENTER     : return CEGUI::Key::NumpadEnter;
    default                    : return CEGUI::Key::Unknown;
    }
}

//----------------------------------------------------------------------------//
void CEGuiGLFWSharedBase::glfwKeyCallback(GLFWwindow* /*window*/, int key,
  int /*scancode*/, int action, int /*mods*/)
{
    CEGUI::Key::Scan ceguiKey = GlfwToCeguiKey(key);

    if(action == GLFW_PRESS)
        d_sampleApp->injectKeyDown(ceguiKey);
    else if (action == GLFW_RELEASE)
        d_sampleApp->injectKeyUp(ceguiKey);
}

//----------------------------------------------------------------------------//
void CEGuiGLFWSharedBase::glfwCharCallback
  (GLFWwindow* /*window*/, unsigned codepoint)
{
    d_sampleApp->injectChar(codepoint);
}

//----------------------------------------------------------------------------//
void CEGuiGLFWSharedBase::glfwMouseButtonCallback
  (GLFWwindow* /*window*/, int button, int action, int /*mods*/)
{
    CEGUI::MouseButton ceguiMouseButton = GlfwToCeguiMouseButton(button);

    if (action == GLFW_PRESS)
        d_sampleApp->injectMouseButtonDown(ceguiMouseButton);
    else if (action == GLFW_RELEASE)
        d_sampleApp->injectMouseButtonUp(ceguiMouseButton);
}

//----------------------------------------------------------------------------//
void CEGuiGLFWSharedBase::glfwScrollCallback
  (GLFWwindow* /*window*/, double /*xoffset*/, double yoffset)
{
    d_sampleApp->injectMouseWheelChange(static_cast<float>(yoffset/4));
}

//----------------------------------------------------------------------------//
void CEGuiGLFWSharedBase::glfwCursorPosCallback
  (GLFWwindow* /*window*/, double x, double y)
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
        glfwSetCursorPos(d_window, d_oldMousePosX, d_oldMousePosY);
        d_mouseDisableCalled = false;
    }

#ifndef DEBUG
    if (x < 0 || y < 0
        || x > d_newWindowWidth
        || y > d_newWindowHeight)
    {
        // show cursor
        glfwSetInputMode(d_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

        // move the cursor to the position where it left the window
        glfwSetCursorPos(d_window, x, y);
        
        // "note down" that the cursor left the window
        d_mouseLeftWindow = true;
    }
    else
    {
        if (d_mouseLeftWindow)
        {
            // get cursor position to restore afterwards
            glfwGetCursorPos(d_window, &d_oldMousePosX, &d_oldMousePosY);

            // we need to inject the previous cursor position because
            // glfw moves the cursor to the centre of the render 
            // window if it gets disabled. therefore notify the 
            // next MousePosCallback invocation of the "mouse disabled" event.
            d_mouseDisableCalled = true;

            // disable cursor
            glfwSetInputMode(d_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

            // "note down" that the cursor is back in the render window
            d_mouseLeftWindow = false;
        }
    }
#endif
}
