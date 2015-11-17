/***********************************************************************
    created:    Sun May 25 2008
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

#ifdef CEGUI_SAMPLES_RENDERER_DIRECT3D10_ACTIVE

#include "CEGuiD3D10BaseApplication.h"
#include "CEGUI/RendererModules/Direct3D10/Renderer.h"
#include "SamplesFrameworkBase.h"
#include "Win32AppHelper.h"
#include "CEGUI/CEGUI.h"

#include <stdexcept>
#include <d3d10.h>
#include <dinput.h>

#ifdef __MINGW32__

extern "C" HRESULT WINAPI D3DX10CreateDeviceAndSwapChain(
  IDXGIAdapter* pAdapter,
  D3D10_DRIVER_TYPE DriverType,
  HMODULE Software,
  UINT Flags,
  DXGI_SWAP_CHAIN_DESC* pSwapChainDesc,
  IDXGISwapChain** ppSwapChain,    
  ID3D10Device** ppDevice);

#endif

//----------------------------------------------------------------------------//
struct CEGuiBaseApplication10Impl
{
    HWND d_window;
    IDXGISwapChain* d_swapChain;
    ID3D10Device* d_device;
    Win32AppHelper::DirectInputState d_directInput;
};

//----------------------------------------------------------------------------//
CEGuiD3D10BaseApplication::CEGuiD3D10BaseApplication() :
    pimpl(new CEGuiBaseApplication10Impl),
    d_lastFrameTime(GetTickCount())
{
    if ((pimpl->d_window = Win32AppHelper::createApplicationWindow(
           s_defaultWindowWidth, s_defaultWindowHeight)))
    {
        if (initialiseDirect3D(s_defaultWindowWidth, s_defaultWindowHeight, true))
        {
            // set the swap chain ptr into window data so we can get access
            // later.  This is a bit of a hack, but saved us redesigning the
            // entire framework just for this.
            SetWindowLongPtr(pimpl->d_window,
                             GWLP_USERDATA,
                             (LONG_PTR)pimpl->d_swapChain);

            if (Win32AppHelper::initialiseDirectInput(pimpl->d_window,
                                                      pimpl->d_directInput))
            {
                d_renderer = &CEGUI::Direct3D10Renderer::create(pimpl->d_device);
                return;
            }

            cleanupDirect3D();
        }

        DestroyWindow(pimpl->d_window);
    }
    else
    {
        MessageBox(0, Win32AppHelper::CREATE_WINDOW_ERROR,
                   Win32AppHelper::APPLICATION_NAME, MB_ICONERROR|MB_OK);
    }

    CEGUI_THROW(std::runtime_error("Windows Direct3D 10 application failed to "
        "initialise."));
}

//----------------------------------------------------------------------------//
CEGuiD3D10BaseApplication::~CEGuiD3D10BaseApplication()
{
    Win32AppHelper::cleanupDirectInput(pimpl->d_directInput);

    cleanupDirect3D();

    DestroyWindow(pimpl->d_window);

    delete pimpl;
}

//----------------------------------------------------------------------------//
void CEGuiD3D10BaseApplication::destroyRenderer()
{
    Win32AppHelper::mouseLeaves();

    CEGUI::Direct3D10Renderer::destroy(
        *static_cast<CEGUI::Direct3D10Renderer*>(d_renderer));
}

//----------------------------------------------------------------------------//
void CEGuiD3D10BaseApplication::run()
{
    Win32AppHelper::setSamplesFramework(d_sampleApp);

    float clear_colour[4] = {0.0f, 0.0f, 0.0f, 0.0f};

    //
    //  This is basically a modified Win32 message pump
    //
    bool idle;

    while (Win32AppHelper::doWin32Events(idle))
    {
        if (idle)
        {
            // do time based updates
            const DWORD thisTime = GetTickCount();
            const float elapsed =
                static_cast<float>(thisTime - d_lastFrameTime) / 1000.0f;
            d_lastFrameTime = thisTime;

            Win32AppHelper::doDirectInputEvents(pimpl->d_directInput);

            // get render target view
            // this is a bit wasteful, but done like this for now since the
            // resize code can change the view from under us.
            ID3D10RenderTargetView* rtview;
            pimpl->d_device->OMGetRenderTargets(1, &rtview, 0);

            // clear display
            pimpl->d_device->ClearRenderTargetView(rtview, clear_colour);

            renderSingleFrame(elapsed);

            pimpl->d_swapChain->Present(0, 0);
            rtview->Release();
        }

        // check if the application is quitting, and break the loop next time
        // around if so.
        if (d_sampleApp->isQuitting())
            PostQuitMessage(0);
    }
}

//----------------------------------------------------------------------------//
void CEGuiD3D10BaseApplication::destroyWindow()
{

}

//----------------------------------------------------------------------------//
void CEGuiD3D10BaseApplication::beginRendering(const float /*elapsed*/)
{
}

//----------------------------------------------------------------------------//
void CEGuiD3D10BaseApplication::endRendering()
{
}

//----------------------------------------------------------------------------//
bool CEGuiD3D10BaseApplication::initialiseDirect3D(unsigned int width,
    unsigned int height, bool windowed)
{
    HRESULT res;

    // init sqap chain descriptor structure
    DXGI_SWAP_CHAIN_DESC scd;
    ZeroMemory(&scd, sizeof(scd));
    scd.BufferCount = 1;
    scd.BufferDesc.Width = width;
    scd.BufferDesc.Height = height;
    scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    scd.BufferDesc.RefreshRate.Numerator = 60;
    scd.BufferDesc.RefreshRate.Denominator = 1;
    scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    scd.OutputWindow = pimpl->d_window;
    scd.SampleDesc.Count = 1;
    scd.SampleDesc.Quality = 0;
    scd.Windowed = windowed;

    // initialise main parts of D3D
#ifdef __MINGW32__
    res = D3DX10CreateDeviceAndSwapChain(0, D3D10_DRIVER_TYPE_HARDWARE,
                                         0, 0,
                                         &scd, &pimpl->d_swapChain,
                                         &pimpl->d_device);
#else
    res = D3D10CreateDeviceAndSwapChain(0, D3D10_DRIVER_TYPE_HARDWARE,
                                        0, 0, D3D10_SDK_VERSION,
                                        &scd, &pimpl->d_swapChain,
                                        &pimpl->d_device);
#endif
    if (SUCCEEDED(res))
    {

        // obtain handle to thr back buffer of the swap chain
        ID3D10Texture2D* back_buffer;
        res = pimpl->d_swapChain->GetBuffer(0, __uuidof(ID3D10Texture2D),
                                            (LPVOID*)&back_buffer);

        if (SUCCEEDED(res))
        {
            ID3D10RenderTargetView* rtview;

            // create render target view using the back buffer
            res = pimpl->d_device->
                CreateRenderTargetView(back_buffer, 0, &rtview);

            // release handle to buffer - we have done all we needed to with it.
            back_buffer->Release();

            if (SUCCEEDED(res))
            {
                // bind the back-buffer render target to get the output.
                pimpl->d_device->
                    OMSetRenderTargets(1, &rtview, 0);

                // set a basic viewport.
                D3D10_VIEWPORT view_port;
                view_port.Width    = width;
                view_port.Height   = height;
                view_port.MinDepth = 0.0f;
                view_port.MaxDepth = 1.0f;
                view_port.TopLeftX = 0;
                view_port.TopLeftY = 0;
                pimpl->d_device->RSSetViewports(1, &view_port);

                // complete window initialisation
                ShowWindow(pimpl->d_window, SW_NORMAL);
                UpdateWindow(pimpl->d_window);

                return true;
            }

            rtview->Release();
        }

        pimpl->d_swapChain->Release();
        pimpl->d_device->Release();
        pimpl->d_swapChain = 0;
        pimpl->d_device = 0;
    }

    MessageBox(0, "Failed to correctly initialise Direct3D 10",
               Win32AppHelper::APPLICATION_NAME, MB_ICONERROR|MB_OK);

    return false;
}

//----------------------------------------------------------------------------//
void CEGuiD3D10BaseApplication::cleanupDirect3D()
{
    if (pimpl->d_device)
    {
        // get render target view
        ID3D10RenderTargetView* rtview;
        pimpl->d_device->OMGetRenderTargets(1, &rtview, 0);

        if (rtview)
        {
            // we release once for the reference we just asked for
            rtview->Release();
            // we release again for the original reference made at creation.
            rtview->Release();
        }

        pimpl->d_swapChain->Release();
        pimpl->d_device->Release();

        pimpl->d_swapChain = 0;
        pimpl->d_device = 0;
    }
}

#endif
