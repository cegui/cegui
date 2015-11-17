/***********************************************************************
    created:    17/10/2004
    author:     Paul D Turner
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2008 Paul D Turner & The CEGUI Development Team
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
#include "CEGUISamplesConfig.h"

#if defined(CEGUI_SAMPLES_RENDERER_DIRECT3D9_ACTIVE) || defined(CEGUI_SAMPLES_RENDERER_DIRECT3D10_ACTIVE) || defined(CEGUI_SAMPLES_RENDERER_DIRECT3D11_ACTIVE)

#include "Win32AppHelper.h"
#include "CEGUI/CEGUI.h"
#include <tchar.h>

#include "SamplesFrameworkBase.h"

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
bool Win32AppHelper::s_mouseInWindow = false;

// For input injection
SamplesFrameworkBase* Win32AppHelper::s_samplesFramework(0);

/*************************************************************************
    Prototypes for internal helper functions a.k.a "The hacks section"
    The implementations for these are in separate files; this is because
    we othereise would need multiple versions of the D3D headers included
    which is obviously a big no-no.
*************************************************************************/
void DeviceReset_Direct3D9(HWND window, CEGUI::Renderer* renderer);
void DeviceReset_Direct3D10(HWND window, CEGUI::Renderer* renderer);
void DeviceReset_Direct3D11(HWND window, CEGUI::Renderer* renderer);

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
        // get size of window to use so that client area is desired size
        RECT wnd_size = {0,0,width,height};
        AdjustWindowRect(&wnd_size, WS_OVERLAPPEDWINDOW, FALSE);

        // create new window
        window = CreateWindow(APPLICATION_NAME, APPLICATION_NAME,
                              WS_OVERLAPPEDWINDOW,
                              0, 0,
                              wnd_size.right - wnd_size.left,
                              wnd_size.bottom - wnd_size.top,
                              0, 0, GetModuleHandle(0), 0);
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
        s_samplesFramework->injectChar((CEGUI::utf32)wParam);
        break;

    case WM_MOUSELEAVE:
        mouseLeaves();
        break;

    case WM_NCMOUSEMOVE:
        mouseLeaves();
        break;

    case WM_MOUSEMOVE:
        mouseEnters();

        s_samplesFramework->injectMousePosition((float)(LOWORD(lParam)), (float)(HIWORD(lParam)));
        break;

    case WM_LBUTTONDOWN:
        s_samplesFramework->injectMouseButtonDown(CEGUI::LeftButton);
        break;

    case WM_LBUTTONUP:
        s_samplesFramework->injectMouseButtonUp(CEGUI::LeftButton);
        break;

    case WM_RBUTTONDOWN:
        s_samplesFramework->injectMouseButtonDown(CEGUI::RightButton);
        break;

    case WM_RBUTTONUP:
        s_samplesFramework->injectMouseButtonUp(CEGUI::RightButton);
        break;

    case WM_MBUTTONDOWN:
        s_samplesFramework->injectMouseButtonDown(CEGUI::MiddleButton);
        break;

    case WM_MBUTTONUP:
        s_samplesFramework->injectMouseButtonUp(CEGUI::MiddleButton);
        break;

    case 0x020A: // WM_MOUSEWHEEL:
        s_samplesFramework->injectMouseWheelChange(static_cast<float>((short)HIWORD(wParam)) / static_cast<float>(120));
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    case WM_SIZE:
        {
            // get CEGUI::System as a pointer so we can detect if it's not
            // instantiated yet.
            CEGUI::System* cegui_system = CEGUI::System::getSingletonPtr();

            // only continue if CEGUI is up an running and window was not
            // minimised (else it's just a waste of time)
            if ((cegui_system != 0) && (wParam != SIZE_MINIMIZED))
            {
                // get renderer identification string
                CEGUI::Renderer* renderer = cegui_system->getRenderer();
                CEGUI::String id(renderer->getIdentifierString());

                // invoke correct function based on the renderer we have ID'd
#ifdef CEGUI_SAMPLES_RENDERER_DIRECT3D9_ACTIVE
                if (id.find("Official Direct3D 9") != id.npos)
                    DeviceReset_Direct3D9(hWnd, renderer);
#endif
#ifdef CEGUI_SAMPLES_RENDERER_DIRECT3D10_ACTIVE
                if (id.find("Official Direct3D 10") != id.npos)
                    DeviceReset_Direct3D10(hWnd, renderer);
#endif
#ifdef CEGUI_SAMPLES_RENDERER_DIRECT3D11_ACTIVE
                if (id.find("Official Direct3D 11") != id.npos)
                    DeviceReset_Direct3D11(hWnd, renderer);
#endif
            }
        }
        break;

    case WM_PAINT:
        {
            PAINTSTRUCT ps;

            BeginPaint(hWnd, &ps);
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
    if (!s_mouseInWindow)
    {
        s_mouseInWindow = true;
        ShowCursor(false);
    }
}

void Win32AppHelper::mouseLeaves(void)
{
    if (s_mouseInWindow)
    {
        s_mouseInWindow = false;
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
                s_samplesFramework->injectKeyDown((CEGUI::Key::Scan)devDat.dwOfs);
            }
            else
            {
                s_samplesFramework->injectKeyUp((CEGUI::Key::Scan)devDat.dwOfs);
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


void Win32AppHelper::setSamplesFramework(SamplesFrameworkBase* samplesFramework)
{
    s_samplesFramework = samplesFramework;
}

#endif
