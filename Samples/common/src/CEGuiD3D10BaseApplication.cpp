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
#include "RendererModules/Direct3D10/CEGUIDirect3D10Renderer.h"
#include "CEGuiSample.h"
#include "Win32AppHelper.h"
#include "CEGUI.h"
#include "CEGUIRenderingRoot.h"

#include <stdexcept>
#include <d3d10.h>
#include <dinput.h>

//----------------------------------------------------------------------------//
struct CEGuiBaseApplicationImpl
{
    HWND d_window;
    IDXGISwapChain* d_swapChain;
    ID3D10Device* d_device;
    CEGUI::Direct3D10Renderer* d_renderer;
    Win32AppHelper::DirectInputState d_directInput;
};

//----------------------------------------------------------------------------//
CEGuiD3D10BaseApplication::CEGuiD3D10BaseApplication() :
    pimpl(new CEGuiBaseApplicationImpl),
    d_lastFrameTime(GetTickCount()),
    d_fps_lastTime(d_lastFrameTime),
    d_fps_frames(0),
    d_fps_value(0)
{
    if (pimpl->d_window = Win32AppHelper::createApplicationWindow(800, 600))
    {
        if (initialiseDirect3D(800, 600, true))
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
                pimpl->d_renderer =
                    &CEGUI::Direct3D10Renderer::bootstrapSystem(pimpl->d_device);

                initialiseResourceGroupDirectories();
                initialiseDefaultResourceGroups();

                CEGUI::Logger::getSingleton().setLoggingLevel(CEGUI::Informative);

                // setup required to do direct rendering of FPS value
                const CEGUI::Rect scrn(CEGUI::Vector2(0, 0), pimpl->d_renderer->getDisplaySize());
                d_fps_geometry = &pimpl->d_renderer->createGeometryBuffer();
                d_fps_geometry->setClippingRegion(scrn);

                // setup for logo
                CEGUI::ImagesetManager::getSingleton().
                    createFromImageFile("cegui_logo", "logo.png", "imagesets");
                d_logo_geometry = &pimpl->d_renderer->createGeometryBuffer();
                d_logo_geometry->setClippingRegion(scrn);
                d_logo_geometry->setPivot(CEGUI::Vector3(50, 34.75f, 0));
                d_logo_geometry->setTranslation(CEGUI::Vector3(10, 520, 0));
                CEGUI::ImagesetManager::getSingleton().get("cegui_logo").
                    getImage("full_image").draw(*d_logo_geometry, CEGUI::Rect(0, 0, 100, 69.5f), 0);

                // clearing this queue actually makes sure it's created(!)
                pimpl->d_renderer->getDefaultRenderingRoot().clearGeometry(CEGUI::RQ_OVERLAY);

                // subscribe handler to render overlay items
                pimpl->d_renderer->getDefaultRenderingRoot().
                    subscribeEvent(CEGUI::RenderingSurface::EventRenderQueueStarted,
                        CEGUI::Event::Subscriber(&CEGuiD3D10BaseApplication::overlayHandler,
                                                 this));
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
    Win32AppHelper::mouseLeaves();

    // cleanup gui system
    CEGUI::Direct3D10Renderer::destroySystem();

    Win32AppHelper::cleanupDirectInput(pimpl->d_directInput);

    cleanupDirect3D();
   
    DestroyWindow(pimpl->d_window);

    delete pimpl;
}

//----------------------------------------------------------------------------//
bool CEGuiD3D10BaseApplication::overlayHandler(const CEGUI::EventArgs& args)
{
    using namespace CEGUI;

    if (static_cast<const RenderQueueEventArgs&>(args).queueID != RQ_OVERLAY)
        return false;

    // render FPS:
    Font* fnt = System::getSingleton().getDefaultFont();
    if (fnt)
    {
        d_fps_geometry->reset();
        fnt->drawText(*d_fps_geometry, d_fps_textbuff, Vector2(0, 0), 0,
                      colour(0xFFFFFFFF));
        d_fps_geometry->draw();
    }

    d_logo_geometry->draw();

    return true;
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

            doFPSUpdate();

            // update logo rotation
            static float rot = 0.0f;
            d_logo_geometry->setRotation(CEGUI::Vector3(rot, 0, 0));
            rot += 180.0f * (elapsed / 1000.0f);
            if (rot > 360.0f)
                rot -= 360.0f;

            Win32AppHelper::doDirectInputEvents(pimpl->d_directInput);

            // get render target view
            // this is a bit wasteful, but done like this for now since the
            // resize code can change the view from under us.
            ID3D10RenderTargetView* rtview;
            pimpl->d_device->OMGetRenderTargets(1, &rtview, 0);

            // clear display
            pimpl->d_device->ClearRenderTargetView(rtview, clear_colour);

            // main CEGUI rendering call
            guiSystem.renderGUI();

            pimpl->d_swapChain->Present(0, 0);
            rtview->Release();
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
void CEGuiD3D10BaseApplication::doFPSUpdate(void)
{
    ++d_fps_frames;

    DWORD thisTime = GetTickCount();

    if (thisTime - d_fps_lastTime >= 1000)
    {
        // update FPS text to output
        sprintf(d_fps_textbuff , "FPS: %d", d_fps_frames);
        d_fps_value = d_fps_frames;
        d_fps_frames = 0;
        d_fps_lastTime = thisTime;
    }

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
