/***********************************************************************
    filename:   CEGuiD3D9BaseApplication.cpp
    created:    24/9/2004
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
#ifdef HAVE_CONFIG_H
#   include "config.h"
#endif

#ifdef CEGUI_SAMPLES_USE_DIRECTX_9

#include "CEGuiD3D9BaseApplication.h"

#include <d3d9.h>
#include <dinput.h>
// undefine Microsoft macro evilness
#undef min
#undef max

#include "RendererModules/Direct3D9/CEGUIDirect3D9Renderer.h"
#include "CEGuiSample.h"
#include "Win32AppHelper.h"
#include "CEGUI.h"
#include "CEGUIRenderingRoot.h"
#include <stdexcept>


/*************************************************************************
    Impl struct
*************************************************************************/
struct CEGuiBaseApplicationImpl
{
    HWND d_window;
    LPDIRECT3D9 d_D3D;
    LPDIRECT3DDEVICE9 d_3DDevice;
    D3DPRESENT_PARAMETERS d_ppars;
    CEGUI::Direct3D9Renderer* d_renderer;
    Win32AppHelper::DirectInputState d_directInput;
};


/*************************************************************************
    Constructor.
*************************************************************************/
CEGuiD3D9BaseApplication::CEGuiD3D9BaseApplication() :
        pimpl(new CEGuiBaseApplicationImpl),
        d_lastFrameTime(GetTickCount()),
        d_fps_lastTime(d_lastFrameTime),
        d_fps_frames(0),
        d_fps_value(0)
{
    if (pimpl->d_window = Win32AppHelper::createApplicationWindow(800, 600))
    {
        if (initialiseDirect3D(800, 600, D3DADAPTER_DEFAULT, true))
        {
            if (Win32AppHelper::initialiseDirectInput(pimpl->d_window, pimpl->d_directInput))
            {
                pimpl->d_renderer =
                    &CEGUI::Direct3D9Renderer::bootstrapSystem(pimpl->d_3DDevice);

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
                        CEGUI::Event::Subscriber(&CEGuiD3D9BaseApplication::overlayHandler,
                                                 this));

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

/*************************************************************************
    Destructor.
*************************************************************************/
CEGuiD3D9BaseApplication::~CEGuiD3D9BaseApplication()
{
    Win32AppHelper::mouseLeaves();

    // cleanup gui system
    CEGUI::Direct3D9Renderer::destroySystem();

    Win32AppHelper::cleanupDirectInput(pimpl->d_directInput);

    // cleanup direct 3d systems
    pimpl->d_3DDevice->Release();
    pimpl->d_D3D->Release();

    DestroyWindow(pimpl->d_window);

    delete pimpl;
}

//----------------------------------------------------------------------------//
bool CEGuiD3D9BaseApplication::overlayHandler(const CEGUI::EventArgs& args)
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

/*************************************************************************
    Start the base application
*************************************************************************/
bool CEGuiD3D9BaseApplication::execute(CEGuiSample* sampleApp)
{
    sampleApp->initialiseSample();

    //
    //  This is basically a modified Win32 message pump
    //
    bool idle;
    HRESULT coop;

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

            doFPSUpdate();

            // update logo rotation
            static float rot = 0.0f;
            d_logo_geometry->setRotation(CEGUI::Vector3(rot, 0, 0));
            rot += 180.0f * (elapsed / 1000.0f);
            if (rot > 360.0f)
                rot -= 360.0f;

            Win32AppHelper::doDirectInputEvents(pimpl->d_directInput);

            // draw display
            if (FAILED(pimpl->d_3DDevice->BeginScene()))
                continue;
            pimpl->d_3DDevice->Clear(0, 0, D3DCLEAR_TARGET,
                                     D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
            guiSystem.renderGUI();
            pimpl->d_3DDevice->EndScene();

            pimpl->d_3DDevice->Present(0, 0, 0, 0);
        }

        // check if the application is quitting, and break the loop next time
        // around if so.
        if (isQuitting())
            PostQuitMessage(0);
    }

    return true;
}


/*************************************************************************
    Performs any required cleanup of the base application system.
*************************************************************************/
void CEGuiD3D9BaseApplication::cleanup()
{
    // nothing to do here.
}


/*************************************************************************
    Initialise Direct3D system.
*************************************************************************/
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


/*************************************************************************
    Do reset of Direct3D device
*************************************************************************/
bool CEGuiD3D9BaseApplication::resetDirect3D(void)
{
    // perform ops needed prior to reset
    pimpl->d_renderer->preD3DReset();

    if (SUCCEEDED(pimpl->d_3DDevice->Reset(&pimpl->d_ppars)))
    {
        // re-build stuff now reset has been done.
        pimpl->d_renderer->postD3DReset();

        return true;
    }

    return false;
}

//----------------------------------------------------------------------------//
void CEGuiD3D9BaseApplication::doFPSUpdate(void)
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

#endif
