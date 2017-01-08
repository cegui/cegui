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
#include "SampleBrowserBase.h"
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
        throw RendererException("Failed to open GLFW window.");
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
    case GLFW_KEY_ESC       : return CEGUI::Key::Scan::Esc;
    case GLFW_KEY_F1        : return CEGUI::Key::Scan::F1;
    case GLFW_KEY_F2        : return CEGUI::Key::Scan::F2;
    case GLFW_KEY_F3        : return CEGUI::Key::Scan::F3;
    case GLFW_KEY_F4        : return CEGUI::Key::Scan::F4;
    case GLFW_KEY_F5        : return CEGUI::Key::Scan::F5;
    case GLFW_KEY_F6        : return CEGUI::Key::Scan::F6;
    case GLFW_KEY_F7        : return CEGUI::Key::Scan::F7;
    case GLFW_KEY_F8        : return CEGUI::Key::Scan::F8;
    case GLFW_KEY_F9        : return CEGUI::Key::Scan::F9;
    case GLFW_KEY_F10       : return CEGUI::Key::Scan::F10;
    case GLFW_KEY_F11       : return CEGUI::Key::Scan::F11;
    case GLFW_KEY_F12       : return CEGUI::Key::Scan::F12;
    case GLFW_KEY_F13       : return CEGUI::Key::Scan::F13;
    case GLFW_KEY_F14       : return CEGUI::Key::Scan::F14;
    case GLFW_KEY_F15       : return CEGUI::Key::Scan::F15;
    case GLFW_KEY_UP        : return CEGUI::Key::Scan::ArrowUp;
    case GLFW_KEY_DOWN      : return CEGUI::Key::Scan::ArrowDown;
    case GLFW_KEY_LEFT      : return CEGUI::Key::Scan::ArrowLeft;
    case GLFW_KEY_RIGHT     : return CEGUI::Key::Scan::ArrowRight;
    case GLFW_KEY_LSHIFT    : return CEGUI::Key::Scan::LeftShift;
    case GLFW_KEY_RSHIFT    : return CEGUI::Key::Scan::RightShift;
    case GLFW_KEY_LCTRL     : return CEGUI::Key::Scan::LeftControl;
    case GLFW_KEY_RCTRL     : return CEGUI::Key::Scan::RightControl;
    case GLFW_KEY_LALT      : return CEGUI::Key::Scan::LeftAlt;
    case GLFW_KEY_RALT      : return CEGUI::Key::Scan::RightAlt;
    case GLFW_KEY_TAB       : return CEGUI::Key::Scan::Tab;
    case GLFW_KEY_ENTER     : return CEGUI::Key::Scan::Return;
    case GLFW_KEY_BACKSPACE : return CEGUI::Key::Scan::Backspace;
    case GLFW_KEY_INSERT    : return CEGUI::Key::Scan::Insert;
    case GLFW_KEY_DEL       : return CEGUI::Key::Scan::DeleteKey;
    case GLFW_KEY_PAGEUP    : return CEGUI::Key::Scan::PageUp;
    case GLFW_KEY_PAGEDOWN  : return CEGUI::Key::Scan::PageDown;
    case GLFW_KEY_HOME      : return CEGUI::Key::Scan::Home;
    case GLFW_KEY_END       : return CEGUI::Key::Scan::End;
    case GLFW_KEY_KP_ENTER  : return CEGUI::Key::Scan::NumpadEnter;
    case GLFW_KEY_SPACE     : return CEGUI::Key::Scan::Space;
    case 'A'                : return CEGUI::Key::Scan::A;
    case 'B'                : return CEGUI::Key::Scan::B;
    case 'C'                : return CEGUI::Key::Scan::C;
    case 'D'                : return CEGUI::Key::Scan::D;
    case 'E'                : return CEGUI::Key::Scan::E;
    case 'F'                : return CEGUI::Key::Scan::F;
    case 'G'                : return CEGUI::Key::Scan::G;
    case 'H'                : return CEGUI::Key::Scan::H;
    case 'I'                : return CEGUI::Key::Scan::I;
    case 'J'                : return CEGUI::Key::Scan::J;
    case 'K'                : return CEGUI::Key::Scan::K;
    case 'L'                : return CEGUI::Key::Scan::L;
    case 'M'                : return CEGUI::Key::Scan::M;
    case 'N'                : return CEGUI::Key::Scan::N;
    case 'O'                : return CEGUI::Key::Scan::O;
    case 'P'                : return CEGUI::Key::Scan::P;
    case 'Q'                : return CEGUI::Key::Scan::Q;
    case 'R'                : return CEGUI::Key::Scan::R;
    case 'S'                : return CEGUI::Key::Scan::S;
    case 'T'                : return CEGUI::Key::Scan::T;
    case 'U'                : return CEGUI::Key::Scan::U;
    case 'V'                : return CEGUI::Key::Scan::V;
    case 'W'                : return CEGUI::Key::Scan::W;
    case 'X'                : return CEGUI::Key::Scan::X;
    case 'Y'                : return CEGUI::Key::Scan::Y;
    case 'Z'                : return CEGUI::Key::Scan::Z;
    default                 : return CEGUI::Key::Scan::Unknown;
    }
}

//----------------------------------------------------------------------------//
void GLFWCALL CEGuiGLFWSharedBase::glfwKeyCallback(int key, int action)
{
    CEGUI::Key::Scan ceguiKey = GlfwToCeguiKey(key);

    if(action == GLFW_PRESS)
    {
        d_sampleApp->injectKeyDown(ceguiKey);
    }
    else if (action == GLFW_RELEASE)
    {
        d_sampleApp->injectKeyUp(ceguiKey);
    }
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

