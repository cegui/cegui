/***********************************************************************
    filename:   CEGuiD3D10BaseApplication.cpp
    created:    Sun May 25 2008
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
#ifdef HAVE_CONFIG_H
#   include "config.h"
#endif

#ifdef CEGUI_SAMPLES_USE_DIRECTX_10

#include "CEGuiD3D10BaseApplication.h"
#include "RendererModules/directx10GUIRenderer/d3d10renderer.h"
#include "CEGuiSample.h"
#include "Win32AppHelper.h"
#include "CEGUI.h"
#include "CEGUIDefaultResourceProvider.h"

#include <stdexcept>
#include <d3d10.h>
#include <dinput.h>

//----------------------------------------------------------------------------//
struct CEGuiBaseApplicationImpl
{
    HWND d_window;
    IDXGISwapChain* d_swapChain;
    ID3D10Device* d_device;
    ID3D10RenderTargetView* d_renderTargetView;
    CEGUI::DirectX10Renderer* d_renderer;
    Win32AppHelper::DirectInputState d_directInput;
};

//----------------------------------------------------------------------------//
CEGuiD3D10BaseApplication::CEGuiD3D10BaseApplication() :
    pimpl(new CEGuiBaseApplicationImpl),
    d_lastTime(GetTickCount()),
    d_frames(0),
    d_FPS(0)
{
    if (pimpl->d_window = Win32AppHelper::createApplicationWindow(800, 600))
    {
        if (initialiseDirect3D(800, 600, true))
        {
            if (Win32AppHelper::initialiseDirectInput(pimpl->d_window,
                                                      pimpl->d_directInput))
            {
                pimpl->d_renderer =
                    new CEGUI::DirectX10Renderer(pimpl->d_device, 0);

                // initialise the gui system
                new CEGUI::System(pimpl->d_renderer);

                // initialise the required dirs for the DefaultResourceProvider
                CEGUI::DefaultResourceProvider* rp =
                    static_cast<CEGUI::DefaultResourceProvider*>
                        (CEGUI::System::getSingleton().getResourceProvider());

                const char* dataPathPrefix = getDataPathPrefix();
                char resourcePath[PATH_MAX];

                // for each resource type, set a resource group directory
                sprintf(resourcePath, "%s/%s", dataPathPrefix, "schemes/");
                rp->setResourceGroupDirectory("schemes", resourcePath);
                sprintf(resourcePath, "%s/%s", dataPathPrefix, "imagesets/");
                rp->setResourceGroupDirectory("imagesets", resourcePath);
                sprintf(resourcePath, "%s/%s", dataPathPrefix, "fonts/");
                rp->setResourceGroupDirectory("fonts", resourcePath);
                sprintf(resourcePath, "%s/%s", dataPathPrefix, "layouts/");
                rp->setResourceGroupDirectory("layouts", resourcePath);
                sprintf(resourcePath, "%s/%s", dataPathPrefix, "looknfeel/");
                rp->setResourceGroupDirectory("looknfeels", resourcePath);
                sprintf(resourcePath, "%s/%s", dataPathPrefix, "lua_scripts/");
                rp->setResourceGroupDirectory("lua_scripts", resourcePath);
                #if defined(CEGUI_WITH_XERCES) && (CEGUI_DEFAULT_XMLPARSER == XercesParser)
                    sprintf(resourcePath, "%s/%s", dataPathPrefix, "XMLRefSchema/");
                    rp->setResourceGroupDirectory("schemas", resourcePath);
                #endif

                CEGUI::Logger::getSingleton().setLoggingLevel(CEGUI::Informative);

                return;
            }

            // cleanup direct 3d systems
            pimpl->d_renderTargetView->Release();
            pimpl->d_swapChain->Release();
            pimpl->d_device->Release();
        }

        DestroyWindow(pimpl->d_window);
    }
    else
    {
        MessageBox(0, Win32AppHelper::CREATE_WINDOW_ERROR,
                   Win32AppHelper::APPLICATION_NAME, MB_ICONERROR|MB_OK);
    }

    throw std::runtime_error("Windows Direct3D 10 application failed to "
                             "initialise.");
}

//----------------------------------------------------------------------------//
CEGuiD3D10BaseApplication::~CEGuiD3D10BaseApplication()
{
    Win32AppHelper::mouseLeaves();

    // cleanup gui system
    delete CEGUI::System::getSingletonPtr();
    delete pimpl->d_renderer;

    Win32AppHelper::cleanupDirectInput(pimpl->d_directInput);

    // cleanup direct 3d systems
    pimpl->d_renderTargetView->Release();
    pimpl->d_swapChain->Release();
    pimpl->d_device->Release();

    DestroyWindow(pimpl->d_window);

    delete pimpl;
}

//----------------------------------------------------------------------------//
bool CEGuiD3D10BaseApplication::execute(CEGuiSample* sampleApp)
{
    sampleApp->initialiseSample();

    float clear_colour[4] = { 0.0f, 0.0f, 0.0f, 1.0f };

    //
    //  This is basically a modified Win32 message pump
    //
    bool idle;

    while (Win32AppHelper::doWin32Events(idle))
    {
        if (idle)
        {
            CEGUI::System& guiSystem = CEGUI::System::getSingleton();

            // do time based updates
            DWORD thisTime = GetTickCount();
            float elapsed = static_cast<float>(thisTime - d_lastFrameTime);
            d_lastFrameTime = thisTime;
            // inject the time pulse
            guiSystem.injectTimePulse(elapsed / 1000.0f);

            updateFPS();
            char fpsbuff[16];
            sprintf(fpsbuff, "FPS: %d", d_FPS);

            Win32AppHelper::doDirectInputEvents(pimpl->d_directInput);

            // clear display
            pimpl->d_device->
                ClearRenderTargetView(pimpl->d_renderTargetView, clear_colour);

            // main CEGUI rendering call
            guiSystem.renderGUI();

            // render FPS:
            CEGUI::Font* fnt = guiSystem.getDefaultFont();
            if (fnt)
            {
                guiSystem.getRenderer()->setQueueingEnabled(false);
                fnt->drawText(fpsbuff, CEGUI::Vector3(0, 0, 0),
                              guiSystem.getRenderer()->getRect());
            }

            pimpl->d_swapChain->Present(0, 0);
        }

        // check if the application is quitting, and break the loop next time
        // around if so.
        if (isQuitting())
            PostQuitMessage(0);
    }

    return true;
}

//----------------------------------------------------------------------------//
void CEGuiD3D10BaseApplication::cleanup()
{
    // nothing to do here.
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
    res = D3D10CreateDeviceAndSwapChain(0, D3D10_DRIVER_TYPE_HARDWARE,
                                        0, 0, D3D10_SDK_VERSION,
                                        &scd, &pimpl->d_swapChain,
                                        &pimpl->d_device);
    if (SUCCEEDED(res))
    {

        // obtain handle to thr back buffer of the swap chain
        ID3D10Texture2D* back_buffer;
        res = pimpl->d_swapChain->GetBuffer(0, __uuidof(ID3D10Texture2D),
                                            (LPVOID*)&back_buffer);

        if (SUCCEEDED(res))
        {
            // create render target view using the back buffer
            res = pimpl->d_device->
                CreateRenderTargetView(back_buffer, 0,
                                       &pimpl->d_renderTargetView);

            // release handle to buffer - we have done all we needed to with it.
            back_buffer->Release();

            if (SUCCEEDED(res))
            {
                // bind the back-buffer render target to get the output.
                pimpl->d_device->
                    OMSetRenderTargets(1, &pimpl->d_renderTargetView, 0);

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

        }

        pimpl->d_renderTargetView->Release();
        pimpl->d_swapChain->Release();
        pimpl->d_renderTargetView = 0;
        pimpl->d_swapChain = 0;
    }

    MessageBox(0, "Failed to correctly initialise Direct3D 10",
               Win32AppHelper::APPLICATION_NAME, MB_ICONERROR|MB_OK);

    return false;
}

//----------------------------------------------------------------------------//
void CEGuiD3D10BaseApplication::updateFPS(void)
{
    ++d_frames;

    DWORD thisTime = GetTickCount();

    if (thisTime - d_lastTime >= 1000)
    {
        d_FPS = d_frames;
        d_frames = 0;
        d_lastTime = thisTime;
    }
}

//----------------------------------------------------------------------------//
#endif
