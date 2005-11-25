/************************************************************************
    filename:   Win32AppHelper.h
    created:    17/10/2004
    author:     Paul D Turner
*************************************************************************/
/*************************************************************************
    Crazy Eddie's GUI System (http://www.cegui.org.uk)
    Copyright (C)2004 - 2005 Paul D Turner (paul@cegui.org.uk)
 
    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.
 
    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.
 
    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*************************************************************************/
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

#if defined(_WIN32)
#  pragma comment(lib, "dinput8.lib")
#  pragma comment(lib, "dxguid.lib")
#endif


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
        LPDIRECTINPUTDEVICE8 keyboardDevice ;
    };

    static HWND createApplicationWindow(int width, int height);
    static void mouseEnters(void);
    static void mouseLeaves(void);
    static bool initialiseDirectInput(HWND window, Win32AppHelper::DirectInputState& dis);
    static void cleanupDirectInput(Win32AppHelper::DirectInputState& dis);
    static void doDirectInputEvents(const Win32AppHelper::DirectInputState& dis);
    static bool doWin32Events(bool& idle);
    static LRESULT CALLBACK wndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

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
    static bool d_mouseInWindow;
};

#endif  // end of guard _Win32AppHelper_h_
