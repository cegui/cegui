/************************************************************************
    filename:   Win32AppHelper.cpp
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
// this controls conditional compile of file for MSVC
#include "CEGUIConfig.h"
#if defined(CEGUI_SAMPLES_USE_DIRECTX_8) || defined(CEGUI_SAMPLES_USE_DIRECTX_9)

#include "Win32AppHelper.h"
#include "CEGUI.h"
#include <TCHAR.H>


/*************************************************************************
    Static Data Definitions
*************************************************************************/
// name of the application, used for class and window creation
const TCHAR Win32AppHelper::APPLICATION_NAME[]      = _TEXT("Crazy Eddie's GUI Mk-2 - Sample Application");

// error strings displayed during initialisation
const TCHAR Win32AppHelper::REGISTER_CLASS_ERROR[]  = _TEXT("Failed to register window class.");
const TCHAR Win32AppHelper::CREATE_WINDOW_ERROR[]   = _TEXT("Failed to create window.");

// other error strings used
const TCHAR Win32AppHelper::CREATE_D3D_ERROR[]      = _TEXT("Failed to create main Direct3D object.");
const TCHAR Win32AppHelper::CREATE_DEVICE_ERROR[]   = _TEXT("Failed to create Direct3D Device object.");

// variable for tracking Win32 cursor
bool Win32AppHelper::d_mouseInWindow = false;


/*************************************************************************
    Create main application window.
*************************************************************************/
HWND Win32AppHelper::createApplicationWindow(int width, int height)
{
    WNDCLASS    wndClass;       // structure used to register window class

    // Initialise WNDCLASS structure.
    wndClass.style          = 0;
    wndClass.lpfnWndProc    = wndProc;
    wndClass.cbClsExtra     = 0;
    wndClass.cbWndExtra     = 0;
    wndClass.hInstance      = GetModuleHandle(0);
    wndClass.hIcon          = LoadIcon(0, IDI_WINLOGO);
    wndClass.hCursor        = LoadCursor(0, IDC_ARROW);
    wndClass.hbrBackground  = static_cast<HBRUSH>(GetStockObject(WHITE_BRUSH));
    wndClass.lpszMenuName   = 0;
    wndClass.lpszClassName  = APPLICATION_NAME;

    HWND window = 0;

    // register class.  Report error & exit upon failure
    if (RegisterClass(&wndClass))
    {
        // create new window
        window = CreateWindow(APPLICATION_NAME, APPLICATION_NAME, WS_OVERLAPPEDWINDOW,
                              0, 0, width, height, 0, 0, GetModuleHandle(0), 0);
    }
    else
    {
        MessageBox(0, REGISTER_CLASS_ERROR, APPLICATION_NAME, MB_ICONERROR|MB_OK);
    }

    return window;
}


/*************************************************************************
    Win32 'Window Procedure' function
*************************************************************************/
LRESULT CALLBACK Win32AppHelper::wndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch(message)
    {
    case WM_CHAR:
        CEGUI::System::getSingleton().injectChar((CEGUI::utf32)wParam);
        break;

    case WM_MOUSELEAVE:
        mouseLeaves();
        break;

    case WM_NCMOUSEMOVE:
        mouseLeaves();
        break;

    case WM_MOUSEMOVE:
        mouseEnters();

        CEGUI::System::getSingleton().injectMousePosition((float)(LOWORD(lParam)), (float)(HIWORD(lParam)));
        break;

    case WM_LBUTTONDOWN:
        CEGUI::System::getSingleton().injectMouseButtonDown(CEGUI::LeftButton);
        break;

    case WM_LBUTTONUP:
        CEGUI::System::getSingleton().injectMouseButtonUp(CEGUI::LeftButton);
        break;

    case WM_RBUTTONDOWN:
        CEGUI::System::getSingleton().injectMouseButtonDown(CEGUI::RightButton);
        break;

    case WM_RBUTTONUP:
        CEGUI::System::getSingleton().injectMouseButtonUp(CEGUI::RightButton);
        break;

    case WM_MBUTTONDOWN:
        CEGUI::System::getSingleton().injectMouseButtonDown(CEGUI::MiddleButton);
        break;

    case WM_MBUTTONUP:
        CEGUI::System::getSingleton().injectMouseButtonUp(CEGUI::MiddleButton);
        break;

    case 0x020A: // WM_MOUSEWHEEL:
        CEGUI::System::getSingleton().injectMouseWheelChange(static_cast<float>((short)HIWORD(wParam)) / static_cast<float>(120));
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    case WM_SIZE:
        // TODO: Notify about new size
        break;

    case WM_PAINT:
        {
            HDC         hDC;
            PAINTSTRUCT ps;

            hDC = BeginPaint(hWnd, &ps);
            EndPaint(hWnd, &ps);
            break;
        }

    default:
        return(DefWindowProc(hWnd, message, wParam, lParam));
        break;
    }

    return 0;
}


void Win32AppHelper::mouseEnters(void)
{
    if (!d_mouseInWindow)
    {
        d_mouseInWindow = true;
        ShowCursor(false);
    }
}

void Win32AppHelper::mouseLeaves(void)
{
    if (d_mouseInWindow)
    {
        d_mouseInWindow = false;
        ShowCursor(true);
    }
}


bool Win32AppHelper::initialiseDirectInput(HWND window, Win32AppHelper::DirectInputState& dis)
{
    if (SUCCEEDED(DirectInput8Create(GetModuleHandle(0), DIRECTINPUT_VERSION, IID_IDirectInput8, (LPVOID*)&dis.directInput, 0)))
    {
        if (SUCCEEDED(dis.directInput->CreateDevice(GUID_SysKeyboard, &dis.keyboardDevice, 0)))
        {
            if (SUCCEEDED(dis.keyboardDevice->SetDataFormat(&c_dfDIKeyboard)))
            {
                if (SUCCEEDED(dis.keyboardDevice->SetCooperativeLevel(window, DISCL_FOREGROUND|DISCL_NONEXCLUSIVE)))
                {
                    DIPROPDWORD inputProp;
                    // the header
                    inputProp.diph.dwSize       = sizeof(DIPROPDWORD);
                    inputProp.diph.dwHeaderSize = sizeof(DIPROPHEADER);
                    inputProp.diph.dwObj        = 0;
                    inputProp.diph.dwHow        = DIPH_DEVICE;
                    inputProp.dwData            = 16;

                    if (SUCCEEDED(dis.keyboardDevice->SetProperty(DIPROP_BUFFERSIZE, &inputProp.diph)))
                    {
                        dis.keyboardDevice->Acquire();
                        return true;
                    }
                    else
                    {
                        MessageBox(0, _TEXT("Failed to set buffer size for keyboard device."), APPLICATION_NAME, MB_ICONERROR|MB_OK);
                    }

                }
                else
                {
                    MessageBox(0, _TEXT("Failed to set co-operative level for keyboard device."), APPLICATION_NAME, MB_ICONERROR|MB_OK);
                }

            }
            else
            {
                MessageBox(0, _TEXT("Failed to set data format for keyboard device."), APPLICATION_NAME, MB_ICONERROR|MB_OK);
            }

            dis.keyboardDevice->Release();
            dis.keyboardDevice = 0;
        }
        else
        {
            MessageBox(0, _TEXT("Failed to create DirectInput keyboard device."), APPLICATION_NAME, MB_ICONERROR|MB_OK);
        }

        dis.directInput->Release();
        dis.directInput = 0;
    }
    else
    {
        MessageBox(0, _TEXT("Failed to create main DirectInput object."), APPLICATION_NAME, MB_ICONERROR|MB_OK);
    }

    return false;
}


void Win32AppHelper::cleanupDirectInput(Win32AppHelper::DirectInputState& dis)
{
    if (dis.keyboardDevice)
    {
        dis.keyboardDevice->Unacquire();
        dis.keyboardDevice->Release();
        dis.keyboardDevice = 0;
    }

    if (dis.directInput)
    {
        dis.directInput->Release();
        dis.directInput = 0;
    }
}


void Win32AppHelper::doDirectInputEvents(const Win32AppHelper::DirectInputState& dis)
{
    // handle direct input based inputs
    DIDEVICEOBJECTDATA devDat;
    DWORD itemCount = 1;

    HRESULT res = dis.keyboardDevice->GetDeviceData(sizeof(DIDEVICEOBJECTDATA), &devDat, &itemCount, 0);

    if (SUCCEEDED(res))
    {
        if (itemCount > 0)
        {
            if (LOBYTE(devDat.dwData) & 0x80)
            {
                // force quit on ESCAPE key
                if (devDat.dwOfs == CEGUI::Key::Escape)
                {
                    PostQuitMessage(0);
                }
                else
                {
                    CEGUI::System::getSingleton().injectKeyDown(devDat.dwOfs);
                }

            }
            else
            {
                CEGUI::System::getSingleton().injectKeyUp(devDat.dwOfs);
            }

        }
    }
    else
    {
        // try to re-acquire device if that was the cause of the error.
        if ((res == DIERR_NOTACQUIRED) || (res == DIERR_INPUTLOST))
        {
            dis.keyboardDevice->Acquire();
        }

    }

}

bool Win32AppHelper::doWin32Events(bool& idle)
{
    MSG msg;

    if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
    {

        if (msg.message == WM_QUIT)
        {
            return false;
        }

        TranslateMessage(&msg);
        DispatchMessage(&msg);

        idle = false;
    }
    else
    {
        idle = true;
    }

    return true;
}

#endif
