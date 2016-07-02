/***********************************************************************
    created:    Tue May 29 2012
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
#include "CEGUISamplesConfig.h"

#ifdef CEGUI_SAMPLES_RENDERER_DIRECT3D11_ACTIVE

#include "CEGuiD3D11BaseApplication.h"
#include "CEGUI/RendererModules/Direct3D11/Renderer.h"
#include "SampleBrowserBase.h"
#include "Win32AppHelper.h"
#include "CEGUI/CEGUI.h"

#include "d3d11-mingw-supplements.h"

#include <stdexcept>
#include <d3d11.h>
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

//----------------------------------------------------------------------------//
struct CEGuiBaseApplication11Impl
{
    HWND d_window;
    IDXGISwapChain* d_swapChain;
    ID3D11Device* d_device;
    ID3D11DeviceContext* d_context;
    D3D_FEATURE_LEVEL d_featureLevel;
    Win32AppHelper::DirectInputState d_directInput;
};

//----------------------------------------------------------------------------//
CEGuiD3D11BaseApplication::CEGuiD3D11BaseApplication() :
    pimpl(new CEGuiBaseApplication11Impl),
    d_lastFrameTime(GetTickCount())
{
    if ((pimpl->d_window = Win32AppHelper::createApplicationWindow(s_defaultWindowWidth, s_defaultWindowHeight)))
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
                d_renderer = &CEGUI::Direct3D11Renderer::create(pimpl->d_device,
                                                                pimpl->d_context);
                return;
            }

            cleanupDirect3D();
        }

        DestroyWindow(pimpl->d_window);
    }
    else
        MessageBox(0, Win32AppHelper::CREATE_WINDOW_ERROR,
                   Win32AppHelper::APPLICATION_NAME, MB_ICONERROR|MB_OK);

    throw std::runtime_error(
        "Windows Direct3D 11 application failed to initialise.");
}

//----------------------------------------------------------------------------//
CEGuiD3D11BaseApplication::~CEGuiD3D11BaseApplication()
{
    Win32AppHelper::cleanupDirectInput(pimpl->d_directInput);

    cleanupDirect3D();

    DestroyWindow(pimpl->d_window);

    delete pimpl;
}

//----------------------------------------------------------------------------//
void CEGuiD3D11BaseApplication::destroyRenderer()
{
    Win32AppHelper::mouseLeaves();

    CEGUI::Direct3D11Renderer::destroy(
        *static_cast<CEGUI::Direct3D11Renderer*>(d_renderer));
}

//----------------------------------------------------------------------------//
void CEGuiD3D11BaseApplication::run()
{
    Win32AppHelper::setSampleBrowser(d_sampleApp);

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
            ID3D11RenderTargetView* rtview;
            pimpl->d_context->OMGetRenderTargets(1, &rtview, 0);

            // clear display
            pimpl->d_context->ClearRenderTargetView(rtview, clear_colour);

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
void CEGuiD3D11BaseApplication::destroyWindow()
{

}


//----------------------------------------------------------------------------//
void CEGuiD3D11BaseApplication::beginRendering(const float /*elapsed*/)
{
}

//----------------------------------------------------------------------------//
void CEGuiD3D11BaseApplication::endRendering()
{
}

//----------------------------------------------------------------------------//
bool CEGuiD3D11BaseApplication::initialiseDirect3D(unsigned int width,
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

    UINT creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;


#if defined(DEBUG) || defined (_DEBUG)
        creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    const D3D_FEATURE_LEVEL featureLevels[] =
    {
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0,
        D3D_FEATURE_LEVEL_9_3,
        D3D_FEATURE_LEVEL_9_2,
        D3D_FEATURE_LEVEL_9_1,
    };

    // initialise main parts of D3D
    res = D3D11CreateDeviceAndSwapChain(0, D3D_DRIVER_TYPE_HARDWARE,
                                        0, creationFlags, featureLevels,
                                        6, D3D11_SDK_VERSION,
                                        &scd, &pimpl->d_swapChain,
                                        &pimpl->d_device, &pimpl->d_featureLevel,
                                        &pimpl->d_context);

    if (SUCCEEDED(res))
    {
         //Debugging stuff
         ID3D11Debug* d3dDebug = 0;
         if( SUCCEEDED( pimpl->d_device->QueryInterface( __uuidof(ID3D11Debug), (void**)&d3dDebug ) ) )
         {
             ID3D11InfoQueue* d3dInfoQueue = 0;
             if( SUCCEEDED( d3dDebug->QueryInterface( __uuidof(ID3D11InfoQueue), (void**)&d3dInfoQueue ) ) )
             {
                 #if defined(DEBUG) || defined (_DEBUG)
                 d3dInfoQueue->SetBreakOnSeverity( D3D11_MESSAGE_SEVERITY_CORRUPTION, true );
                 d3dInfoQueue->SetBreakOnSeverity( D3D11_MESSAGE_SEVERITY_ERROR, true );
                 #endif
 
                 D3D11_MESSAGE_ID hide [] =
                 {
                     D3D11_MESSAGE_ID_SETPRIVATEDATA_CHANGINGPARAMS,
                     // Add more message IDs here as needed
                 };
 
                 D3D11_INFO_QUEUE_FILTER filter;
                 memset( &filter, 0, sizeof(filter) );
                 filter.DenyList.NumIDs = _countof(hide);
                 filter.DenyList.pIDList = hide;
                 d3dInfoQueue->AddStorageFilterEntries( &filter );
                 d3dInfoQueue->Release();
             }

             d3dDebug->ReportLiveDeviceObjects(static_cast<D3D11_RLDO_FLAGS>(D3D11_RLDO_SUMMARY | D3D11_RLDO_DETAIL));

             d3dDebug->Release();
         }



        // obtain handle to thr back buffer of the swap chain
        ID3D11Texture2D* back_buffer;
        res = pimpl->d_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D),
                                            (LPVOID*)&back_buffer);

        if (SUCCEEDED(res))
        {
            ID3D11RenderTargetView* rtview;

            // create render target view using the back buffer
            res = pimpl->d_device->
                CreateRenderTargetView(back_buffer, 0, &rtview);

            // release handle to buffer - we have done all we needed to with it.
            back_buffer->Release();

            if (SUCCEEDED(res))
            {
                // bind the back-buffer render target to get the output.
                pimpl->d_context->
                    OMSetRenderTargets(1, &rtview, 0);

                // set a basic viewport.
                D3D11_VIEWPORT view_port;
                view_port.Width    = static_cast<float>(width);
                view_port.Height   = static_cast<float>(height);
                view_port.MinDepth = 0.0f;
                view_port.MaxDepth = 1.0f;
                view_port.TopLeftX = 0;
                view_port.TopLeftY = 0;
                pimpl->d_context->RSSetViewports(1, &view_port);

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

    MessageBox(0, "Failed to correctly initialise Direct3D 11",
               Win32AppHelper::APPLICATION_NAME, MB_ICONERROR|MB_OK);

    return false;
}

//----------------------------------------------------------------------------//
void CEGuiD3D11BaseApplication::cleanupDirect3D()
{
    if (pimpl->d_device)
    {
        // get render target view
        ID3D11RenderTargetView* rtview;
        pimpl->d_context->OMGetRenderTargets(1, &rtview, 0);

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

