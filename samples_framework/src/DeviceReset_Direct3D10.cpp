/***********************************************************************
    created:    25/5/2008
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

#ifdef CEGUI_SAMPLES_RENDERER_DIRECT3D10_ACTIVE

#include "CEGUI/CEGUI.h"
#include "CEGUI/RendererModules/Direct3D10/Renderer.h"
#define WIN32_LEAN_AND_MEAN
#include <d3d10.h>
//----------------------------------------------------------------------------//
// The following function is basically a nasty hack; we just needed to do the
// preD3DReset call, the actual device reset, and then the postD3DReset call -
// the issue is that much of the information is not readily available here to us
// so we jump through a couple of hoops to obtain that information (and in the
// case of the D3DPRESENT_PARAMETERS recreate it from scratch).
//
// The reason this is in it's own file is that we need to be able to handle
// multiple different versions of this function for the D3D versions that the
// sample framework supports, though we can only include one main d3d header
// at a time, so these funcs live in their own files.
//----------------------------------------------------------------------------//

//----------------------------------------------------------------------------//
void DeviceReset_Direct3D10(HWND window, CEGUI::Renderer* renderer)
{
    // this is the 'other side' of our earlier hack to save from having to
    // totally rewrite the samples framework.
    //
    // Extract the pointer to the CEGuiD3D10BaseApplication from the window
    //CEGuiD3D10BaseApplication* app =
    //    reinterpret_cast<CEGuiD3D10BaseApplication*>(
    //        GetWindowLongPtr(window,GWLP_USERDATA));

    IDXGISwapChain* swap_chain = reinterpret_cast<IDXGISwapChain*>(
        GetWindowLongPtr(window, GWLP_USERDATA));

    // if the swap_chain pointer is 0, something obviously is amiss, so bail
    if (!swap_chain)
        return;

    CEGUI::Direct3D10Renderer* d3d_renderer =
        static_cast<CEGUI::Direct3D10Renderer*>(renderer);

    ID3D10Device& d3d_device = d3d_renderer->getDirect3DDevice();

    ID3D10RenderTargetView* rtview;
    d3d_device.OMGetRenderTargets(1, &rtview, 0);

    // we release once for the reference we just asked for
    rtview->Release();
    // we release again for the original reference made at creation.
    rtview->Release();

    if (FAILED(swap_chain->ResizeBuffers(1, 0, 0, 
                                         DXGI_FORMAT_R8G8B8A8_UNORM, 0)))
    {
        CEGUI::Logger::getSingleton().logEvent("Direct3D 10: Failed to resize "
            "swap chain buffers.", CEGUI::Errors);

        return;
    }

    RECT wnd_rect;
    GetClientRect(window, &wnd_rect);

    int width  = wnd_rect.right;// - wnd_rect.left;
    int height = wnd_rect.bottom;// - wnd_rect.top;

    HRESULT res;
    // obtain handle to thr back buffer of the swap chain
    ID3D10Texture2D* back_buffer;
    res = swap_chain->GetBuffer(0, __uuidof(ID3D10Texture2D),
                                (LPVOID*)&back_buffer);

    if (SUCCEEDED(res))
    {
        // create render target view using the back buffer
        res = d3d_device.CreateRenderTargetView(back_buffer, 0, &rtview);

        // release handle to buffer - we have done all we needed to with it.
        back_buffer->Release();

        if (SUCCEEDED(res))
        {
            // bind the back-buffer render target to get the output.
            d3d_device.OMSetRenderTargets(1, &rtview, 0);

            // set a basic viewport.
            D3D10_VIEWPORT view_port;
            view_port.Width    = width;
            view_port.Height   = height;
            view_port.MinDepth = 0.0f;
            view_port.MaxDepth = 1.0f;
            view_port.TopLeftX = 0;
            view_port.TopLeftY = 0;
            d3d_device.RSSetViewports(1, &view_port);

            // notify CEGUI of change.
            CEGUI::System::getSingleton().notifyDisplaySizeChanged(
                CEGUI::Sizef((float)width, (float)height));
        }
    }
}

//----------------------------------------------------------------------------//

#endif
