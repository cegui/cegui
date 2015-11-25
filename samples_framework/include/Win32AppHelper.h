/***********************************************************************
    created:    17/10/2004
    author:     Paul D Turner
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2006 Paul D Turner & The CEGUI Development Team
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
#ifndef _Win32AppHelper_h_
#define _Win32AppHelper_h_

#if defined( __WIN32__ ) || defined( _WIN32 )
#   define WIN32_LEAN_AND_MEAN
#   include <windows.h>

#include <dinput.h>

// undefine Microsoft macro evilness
#   undef min
#   undef max
#endif

#ifdef _MSC_VER
#  pragma comment(lib, "dinput8.lib")
#  pragma comment(lib, "dxguid.lib")
#endif

class SamplesFrameworkBase;


/*!
\brief
    All static utility class containing helper / common functions used for the Win32 apps
*/
class Win32AppHelper
{
public:
    struct DirectInputState
    {
        DirectInputState() : directInput(0), keyboardDevice(0)
        {}

        LPDIRECTINPUT8 directInput;
        LPDIRECTINPUTDEVICE8 keyboardDevice;
    };

    static HWND createApplicationWindow(int width, int height);
    static void mouseEnters(void);
    static void mouseLeaves(void);
    static bool initialiseDirectInput(HWND window, Win32AppHelper::DirectInputState& dis);
    static void cleanupDirectInput(Win32AppHelper::DirectInputState& dis);
    static void doDirectInputEvents(const Win32AppHelper::DirectInputState& dis);
    static bool doWin32Events(bool& idle);
    static LRESULT CALLBACK wndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

    static void setSamplesFramework(SamplesFrameworkBase* samplesFramework);

    /*************************************************************************
        Constants
    *************************************************************************/
    // name of the application, used for class and window creation
    static const TCHAR  APPLICATION_NAME[];

    // error strings displayed during initialisation
    static const TCHAR  REGISTER_CLASS_ERROR[];
    static const TCHAR  CREATE_WINDOW_ERROR[];

    // other error strings used
    static const TCHAR  CREATE_D3D_ERROR[];
    static const TCHAR  CREATE_DEVICE_ERROR[];

private:
    static bool s_mouseInWindow;

    static SamplesFrameworkBase* s_samplesFramework;
};

#endif  // end of guard _Win32AppHelper_h_
