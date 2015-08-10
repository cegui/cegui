/***********************************************************************
    created:    Sat Mar 7 2009
    author:     Paul D Turner (parts based on code by Rajko Stojadinovic)
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2011 Paul D Turner & The CEGUI Development Team
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
#include "CEGUI/RendererModules/Direct3D10/TextureTarget.h"
#include "CEGUI/RendererModules/Direct3D10/Texture.h"
#include "CEGUI/PropertyHelper.h"

// Start of CEGUI namespace section
namespace CEGUI
{
//----------------------------------------------------------------------------//
const float Direct3D10TextureTarget::DEFAULT_SIZE = 128.0f;
uint Direct3D10TextureTarget::s_textureNumber = 0;

//----------------------------------------------------------------------------//
Direct3D10TextureTarget::Direct3D10TextureTarget(Direct3D10Renderer& owner) :
    Direct3D10RenderTarget<TextureTarget>(owner),
    d_texture(0),
    d_renderTargetView(0),
    d_previousRenderTargetView(0),
    d_previousDepthStencilView(0)
{
    // this essentially creates a 'null' CEGUI::Texture
    d_CEGUITexture = &static_cast<Direct3D10Texture&>(
        d_owner.createTexture(generateTextureName()));

    // setup area and cause the initial texture to be generated.
    declareRenderSize(Sizef(DEFAULT_SIZE, DEFAULT_SIZE));
}

//----------------------------------------------------------------------------//
Direct3D10TextureTarget::~Direct3D10TextureTarget()
{
    cleanupRenderTexture();
    d_owner.destroyTexture(*d_CEGUITexture);
}

//----------------------------------------------------------------------------//
void Direct3D10TextureTarget::activate()
{
    enableRenderTexture();
    Direct3D10RenderTarget::activate();
}

//----------------------------------------------------------------------------//
void Direct3D10TextureTarget::deactivate()
{
    Direct3D10RenderTarget::deactivate();
    disableRenderTexture();
}

//----------------------------------------------------------------------------//
bool Direct3D10TextureTarget::isImageryCache() const
{
    return true;
}

//----------------------------------------------------------------------------//
void Direct3D10TextureTarget::clear()
{
    const float colour[] = { 0, 0, 0, 0 };
    d_device.ClearRenderTargetView(d_renderTargetView, colour);
}

//----------------------------------------------------------------------------//
Texture& Direct3D10TextureTarget::getTexture() const
{
    return *d_CEGUITexture;
}

//----------------------------------------------------------------------------//
void Direct3D10TextureTarget::declareRenderSize(const Sizef& sz)
{
    // exit if current size is enough
    if ((d_area.getWidth() >= sz.d_width) && (d_area.getHeight() >=sz.d_height))
        return;

    setArea(Rectf(d_area.getPosition(), sz));
    resizeRenderTexture();
    clear();
}

//----------------------------------------------------------------------------//
bool Direct3D10TextureTarget::isRenderingInverted() const
{
    return false;
}

//----------------------------------------------------------------------------//
void Direct3D10TextureTarget::initialiseRenderTexture()
{
    // Create the render target texture
    D3D10_TEXTURE2D_DESC tex_desc;
    ZeroMemory(&tex_desc, sizeof(tex_desc));
    tex_desc.Width = static_cast<UINT>(d_area.getSize().d_width);
    tex_desc.Height = static_cast<UINT>(d_area.getSize().d_height);
    tex_desc.MipLevels = 1;
    tex_desc.ArraySize = 1;
    tex_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    tex_desc.SampleDesc.Count = 1;
    tex_desc.Usage = D3D10_USAGE_DEFAULT;
    tex_desc.BindFlags = D3D10_BIND_RENDER_TARGET | D3D10_BIND_SHADER_RESOURCE;
    d_device.CreateTexture2D(&tex_desc, 0, &d_texture);

    // create render target view, so we can render to the thing
    D3D10_RENDER_TARGET_VIEW_DESC rtv_desc;
    rtv_desc.Format = tex_desc.Format;
    rtv_desc.ViewDimension = D3D10_RTV_DIMENSION_TEXTURE2D;
    rtv_desc.Texture2D.MipSlice = 0;
    d_device.CreateRenderTargetView(d_texture, &rtv_desc, &d_renderTargetView);

    d_CEGUITexture->setDirect3DTexture(d_texture);
    d_CEGUITexture->setOriginalDataSize(d_area.getSize());
}

//----------------------------------------------------------------------------//
void Direct3D10TextureTarget::cleanupRenderTexture()
{
    if (d_renderTargetView)
    {
        d_renderTargetView->Release();
        d_renderTargetView = 0;
    }
    if (d_texture)
    {
        d_CEGUITexture->setDirect3DTexture(0);
        d_texture->Release();
        d_texture = 0;
    }
}

//----------------------------------------------------------------------------//
void Direct3D10TextureTarget::resizeRenderTexture()
{
    cleanupRenderTexture();
    initialiseRenderTexture();
}

//----------------------------------------------------------------------------//
void Direct3D10TextureTarget::enableRenderTexture()
{
    d_device.OMGetRenderTargets(1, &d_previousRenderTargetView,
                                &d_previousDepthStencilView);

    d_device.OMSetRenderTargets(1, &d_renderTargetView, 0);
}

//----------------------------------------------------------------------------//
void Direct3D10TextureTarget::disableRenderTexture()
{
    if (d_previousRenderTargetView)
        d_previousRenderTargetView->Release();
    if (d_previousDepthStencilView)
        d_previousDepthStencilView->Release();

    d_device.OMSetRenderTargets(1, &d_previousRenderTargetView,
                                d_previousDepthStencilView);

    d_previousRenderTargetView = 0;
    d_previousDepthStencilView = 0;
}

//----------------------------------------------------------------------------//
String Direct3D10TextureTarget::generateTextureName()
{
    String tmp("_d3d10_tt_tex_");
    tmp.append(PropertyHelper<uint>::toString(s_textureNumber++));

    return tmp;
}

//----------------------------------------------------------------------------//

} // End of  CEGUI namespace section

//----------------------------------------------------------------------------//
// Implementation of template base class
#include "./RenderTarget.inl"

