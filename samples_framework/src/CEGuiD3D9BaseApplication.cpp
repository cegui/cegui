/***********************************************************************
    created:    24/9/2004
    author:     Paul D Turner
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2009 Paul D Turner & The CEGUI Development Team
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

#ifdef CEGUI_SAMPLES_RENDERER_DIRECT3D9_ACTIVE

#include "CEGuiD3D9BaseApplication.h"

#include <d3d9.h>
#include <dinput.h>
// undefine Microsoft macro evilness
#undef min
#undef max

#include "CEGUI/RendererModules/Direct3D9/Renderer.h"
#include "SamplesFrameworkBase.h"
#include "Win32AppHelper.h"
#include "CEGUI/CEGUI.h"
#include <stdexcept>

//----------------------------------------------------------------------------//
struct CEGuiBaseApplication9Impl
{
    HWND d_window;
    LPDIRECT3D9 d_D3D;
    LPDIRECT3DDEVICE9 d_3DDevice;
    D3DPRESENT_PARAMETERS d_ppars;
    Win32AppHelper::DirectInputState d_directInput;
};

//----------------------------------------------------------------------------//
CEGuiD3D9BaseApplication::CEGuiD3D9BaseApplication() :
pimpl(new CEGuiBaseApplication9Impl),
    d_lastFrameTime(GetTickCount())
{
    if ((pimpl->d_window = Win32AppHelper::createApplicationWindow(
           s_defaultWindowWidth, s_defaultWindowHeight)))
    {
        if (initialiseDirect3D(s_defaultWindowWidth, s_defaultWindowHeight, D3DADAPTER_DEFAULT, true))
        {
            if (Win32AppHelper::initialiseDirectInput(pimpl->d_window, pimpl->d_directInput))
            {
                d_renderer = &CEGUI::Direct3D9Renderer::create(pimpl->d_3DDevice);
                return;
            }

            // cleanup direct 3d systems
            pimpl->d_3DDevice->Release();
            pimpl->d_D3D->Release();
        }

        DestroyWindow(pimpl->d_window);
    }
    else
    {
        MessageBox(0, Win32AppHelper::CREATE_WINDOW_ERROR, Win32AppHelper::APPLICATION_NAME, MB_ICONERROR|MB_OK);
    }

    CEGUI_THROW(std::runtime_error("Win32 DirectX 9 application failed to initialise."));
}

//----------------------------------------------------------------------------//
CEGuiD3D9BaseApplication::~CEGuiD3D9BaseApplication()
{
    Win32AppHelper::cleanupDirectInput(pimpl->d_directInput);

    // cleanup direct 3d systems
    pimpl->d_3DDevice->Release();
    pimpl->d_D3D->Release();

    DestroyWindow(pimpl->d_window);

    delete pimpl;
}

//----------------------------------------------------------------------------//
void CEGuiD3D9BaseApplication::destroyRenderer()
{
    Win32AppHelper::mouseLeaves();

    CEGUI::Direct3D9Renderer::destroy(
        *static_cast<CEGUI::Direct3D9Renderer*>(d_renderer));
}

//----------------------------------------------------------------------------//
void CEGuiD3D9BaseApplication::run()
{
    Win32AppHelper::setSamplesFramework(d_sampleApp);

    //
    //  This is basically a modified Win32 message pump
    //
    bool idle;
    HRESULT coop;

    while (Win32AppHelper::doWin32Events(idle))
    {
        if (idle)
        {
            // do time based updates
            DWORD thisTime = GetTickCount();
            const float elapsed =
                static_cast<float>(thisTime - d_lastFrameTime) / 1000.0f;
            d_lastFrameTime = thisTime;

            // handle D3D lost device stuff
            coop = pimpl->d_3DDevice->TestCooperativeLevel();

            if (coop == D3DERR_DEVICELOST)
            {
                Sleep(500);
                continue;
            }
            else if (coop == D3DERR_DEVICENOTRESET)
            {
                if (!resetDirect3D())
                {
                    continue;
                }
            }

            Win32AppHelper::doDirectInputEvents(pimpl->d_directInput);

            // draw display
            if (FAILED(pimpl->d_3DDevice->BeginScene()))
                continue;

            renderSingleFrame(elapsed);
        }

        // check if the application is quitting, and break the loop next time
        // around if so.
        if (d_sampleApp->isQuitting())
            PostQuitMessage(0);
    }
}

//----------------------------------------------------------------------------//
void CEGuiD3D9BaseApplication::destroyWindow()
{

}

//----------------------------------------------------------------------------//
void CEGuiD3D9BaseApplication::beginRendering(const float /*elapsed*/)
{
    pimpl->d_3DDevice->Clear(0, 0, D3DCLEAR_TARGET,
                             D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
}

//----------------------------------------------------------------------------//
void CEGuiD3D9BaseApplication::endRendering()
{
    pimpl->d_3DDevice->EndScene();
    pimpl->d_3DDevice->Present(0, 0, 0, 0);
}

//----------------------------------------------------------------------------//
bool CEGuiD3D9BaseApplication::initialiseDirect3D(unsigned int width, unsigned int height, unsigned int adapter, bool windowed)
{
    pimpl->d_D3D = Direct3DCreate9(D3D_SDK_VERSION);

    // display error and exit if D3D creation failed
    if (pimpl->d_D3D)
    {
        D3DDISPLAYMODE d3ddm;
        pimpl->d_D3D->GetAdapterDisplayMode(adapter, &d3ddm);

        D3DFORMAT format = d3ddm.Format;

        // complete window initialisation
        ShowWindow(pimpl->d_window, SW_NORMAL);
        UpdateWindow(pimpl->d_window);

        ZeroMemory(&pimpl->d_ppars, sizeof(pimpl->d_ppars));
        pimpl->d_ppars.BackBufferFormat = format;
        pimpl->d_ppars.hDeviceWindow    = pimpl->d_window;
        pimpl->d_ppars.SwapEffect       = D3DSWAPEFFECT_DISCARD;
        pimpl->d_ppars.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
        pimpl->d_ppars.Windowed         = windowed;

        if (!windowed)
        {
            pimpl->d_ppars.BackBufferWidth          = width;
            pimpl->d_ppars.BackBufferHeight         = height;
            pimpl->d_ppars.BackBufferCount          = 1;
            pimpl->d_ppars.MultiSampleType          = D3DMULTISAMPLE_NONE;
            pimpl->d_ppars.FullScreen_RefreshRateInHz       = D3DPRESENT_RATE_DEFAULT;
        }

        if (SUCCEEDED(pimpl->d_D3D->CreateDevice(adapter, D3DDEVTYPE_HAL, pimpl->d_window, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &pimpl->d_ppars, &pimpl->d_3DDevice)))
        {
            return true;
        }
        else
        {
            MessageBox(0, Win32AppHelper::CREATE_DEVICE_ERROR, Win32AppHelper::APPLICATION_NAME, MB_ICONERROR|MB_OK);
        }

        pimpl->d_D3D->Release();
        pimpl->d_D3D = 0;
    }
    else
    {
        MessageBox(0, Win32AppHelper::CREATE_D3D_ERROR, Win32AppHelper::APPLICATION_NAME, MB_ICONERROR|MB_OK);
    }

    return false;
}

//----------------------------------------------------------------------------//
bool CEGuiD3D9BaseApplication::resetDirect3D(void)
{
    // perform ops needed prior to reset
    static_cast<CEGUI::Direct3D9Renderer*>(d_renderer)->preD3DReset();

    if (SUCCEEDED(pimpl->d_3DDevice->Reset(&pimpl->d_ppars)))
    {
        // re-build stuff now reset has been done.
        static_cast<CEGUI::Direct3D9Renderer*>(d_renderer)->postD3DReset();
        return true;
    }

    return false;
}

//----------------------------------------------------------------------------//

#endif
