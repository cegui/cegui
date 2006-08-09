/***********************************************************************
    filename:   CEGuiD3D81BaseApplication.cpp
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

#ifdef CEGUI_SAMPLES_USE_DIRECTX_8

#include "CEGuiD3D81BaseApplication.h"

#include <d3d8.h>
#include <dinput.h>
// undefine Microsoft macro evilness
#undef min
#undef max

#include "RendererModules/directx81GUIRenderer/renderer.h"
#include "CEGuiSample.h"
#include "Win32AppHelper.h"
#include "CEGUI.h"
#include "CEGUIDefaultResourceProvider.h"

#include <stdexcept>

#ifdef _MSC_VER
# if defined(DEBUG) || defined (_DEBUG)
#   pragma comment (lib, "DirectX81GUIRenderer_d.lib")
# else
#   pragma comment (lib, "DirectX81GUIRenderer.lib")
# endif
#endif

/*************************************************************************
    Impl struct
*************************************************************************/
struct CEGuiD3D81BaseApplicationImpl
{
    HWND d_window;
    LPDIRECT3D8 d_D3D;
    LPDIRECT3DDEVICE8 d_3DDevice;
    D3DPRESENT_PARAMETERS d_ppars;
    CEGUI::DirectX81Renderer* d_renderer;
    Win32AppHelper::DirectInputState d_directInput;
};


/*************************************************************************
    Constructor
*************************************************************************/
CEGuiD3D81BaseApplication::CEGuiD3D81BaseApplication() :
        pimpl(new CEGuiD3D81BaseApplicationImpl),
        d_lastTime(GetTickCount()),
        d_frames(0),
        d_FPS(0)
{
    if (pimpl->d_window = Win32AppHelper::createApplicationWindow(800, 600))
    {
        if (initialiseDirect3D(800, 600, D3DADAPTER_DEFAULT, true))
        {
            if (Win32AppHelper::initialiseDirectInput(pimpl->d_window, pimpl->d_directInput))
            {
                pimpl->d_renderer = new CEGUI::DirectX81Renderer(pimpl->d_3DDevice, 3000);

                // initialise the gui system
                new CEGUI::System(pimpl->d_renderer);

                // initialise the required dirs for the DefaultResourceProvider
                CEGUI::DefaultResourceProvider* rp = static_cast<CEGUI::DefaultResourceProvider*>
                    (CEGUI::System::getSingleton().getResourceProvider());
        
                rp->setResourceGroupDirectory("schemes", "../datafiles/schemes/");
                rp->setResourceGroupDirectory("imagesets", "../datafiles/imagesets/");
                rp->setResourceGroupDirectory("fonts", "../datafiles/fonts/");
                rp->setResourceGroupDirectory("layouts", "../datafiles/layouts/");
                rp->setResourceGroupDirectory("looknfeels", "../datafiles/looknfeel/");
                rp->setResourceGroupDirectory("lua_scripts", "../datafiles/lua_scripts/");
#if defined(CEGUI_WITH_XERCES) && (CEGUI_DEFAULT_XMLPARSER == XercesParser)
                rp->setResourceGroupDirectory("schemas", "../../XMLRefSchema/");
#endif

                CEGUI::Logger::getSingleton().setLoggingLevel(CEGUI::Informative);

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

    throw std::runtime_error("Win32 DirectX 8.1 application failed to initialise.");
}


/*************************************************************************
    Destructor.
*************************************************************************/
CEGuiD3D81BaseApplication::~CEGuiD3D81BaseApplication()
{
    Win32AppHelper::mouseLeaves();

    // cleanup gui system
	delete CEGUI::System::getSingletonPtr();
    delete pimpl->d_renderer;

    Win32AppHelper::cleanupDirectInput(pimpl->d_directInput);

    // cleanup direct 3d systems
    pimpl->d_3DDevice->Release();
    pimpl->d_D3D->Release();

    DestroyWindow(pimpl->d_window);
}


/*************************************************************************
    Start the base application
*************************************************************************/
bool CEGuiD3D81BaseApplication::execute(CEGuiSample* sampleApp)
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

            if (FAILED(pimpl->d_3DDevice->BeginScene()))
            {
                continue;
            }

            // main part of idle loop
            updateFPS();
            char fpsbuff[16];
            sprintf(fpsbuff, "FPS: %d", d_FPS);

            Win32AppHelper::doDirectInputEvents(pimpl->d_directInput);

            // draw display
			CEGUI::System& guiSystem = CEGUI::System::getSingleton();
            pimpl->d_3DDevice->Clear(0, 0, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
			guiSystem.renderGUI();

            // render FPS:
            CEGUI::Font* fnt = guiSystem.getDefaultFont();
            if (fnt)
            {
                guiSystem.getRenderer()->setQueueingEnabled(false);
                fnt->drawText(fpsbuff, CEGUI::Vector3(0, 0, 0), guiSystem.getRenderer()->getRect());
            }

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
void CEGuiD3D81BaseApplication::cleanup()
{
    // nothing to do here.
}


/*************************************************************************
    Initialise Direct3D system.
*************************************************************************/
bool CEGuiD3D81BaseApplication::initialiseDirect3D(unsigned int width, unsigned int height, unsigned int adapter, bool windowed)
{
    pimpl->d_D3D = Direct3DCreate8(D3D_SDK_VERSION);

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
        pimpl->d_ppars.Windowed         = windowed;

        if (!windowed)
        {
            pimpl->d_ppars.BackBufferWidth          = width;
            pimpl->d_ppars.BackBufferHeight         = height;
            pimpl->d_ppars.BackBufferCount          = 1;
            pimpl->d_ppars.MultiSampleType          = D3DMULTISAMPLE_NONE;
            pimpl->d_ppars.FullScreen_PresentationInterval  = D3DPRESENT_INTERVAL_IMMEDIATE;
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
bool CEGuiD3D81BaseApplication::resetDirect3D(void)
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

#endif
