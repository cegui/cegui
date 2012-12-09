/***********************************************************************
    filename:   CEGUIDirect3D9Texture.cpp
    created:    Mon Feb 9 2009
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
#define NOMINMAX
#include <d3dx9.h>
#include "CEGUIDirect3D9Texture.h"
#include "CEGUIExceptions.h"
#include "CEGUISystem.h"
#include "CEGUIImageCodec.h"

// Start of CEGUI namespace section
namespace CEGUI
{
//----------------------------------------------------------------------------//
// Internal helper class to wrap the internal handling needed to copy from a
// D3D9 texture.
class D3DSurfaceBlitter
{
public:
    //------------------------------------------------------------------------//
    D3DSurfaceBlitter(LPDIRECT3DDEVICE9 d3d_device, LPDIRECT3DTEXTURE9 tex) :
        d_device(d3d_device),
        d_texture(tex),
        d_renderTarget(0),
        d_offscreen(0)
    {
        populateTextureSurfaceDescription();
    }

    //------------------------------------------------------------------------//
    ~D3DSurfaceBlitter()
    {
        cleanupSurfaces();
    }

    //------------------------------------------------------------------------//
    void blitToMemory(uint32* dest)
    {
        if (!d_texture)
            return;

        lockRect(0, true);

        blitD3DCOLORSurfaceToRGBA(
            static_cast<uint32*>(d_lockedRect.pBits), dest,
            Size(static_cast<float>(d_surfDesc.Width),
                 static_cast<float>(d_surfDesc.Height)),
            d_lockedRect.Pitch);

        unlockRect();
    }

    //------------------------------------------------------------------------//
    bool isRenderTarget() const
    {
        return d_surfDesc.Usage == D3DUSAGE_RENDERTARGET;
    }

    //------------------------------------------------------------------------//

private:
    LPDIRECT3DDEVICE9 d_device;
    LPDIRECT3DTEXTURE9 d_texture;
    LPDIRECT3DSURFACE9 d_renderTarget;
    LPDIRECT3DSURFACE9 d_offscreen;
    D3DSURFACE_DESC d_surfDesc;
    D3DLOCKED_RECT d_lockedRect;
    RECT d_fullArea;

    //------------------------------------------------------------------------//
    void initialiseSurfaces()
    {
        if (!d_offscreen)
            createOffscreenSurface();

        if (!d_renderTarget)
            getTextureSurface();
    }

    //------------------------------------------------------------------------//
    void cleanupSurfaces()
    {
        if (d_renderTarget)
        {
            d_renderTarget->Release();
            d_renderTarget = 0;
        }

        if (d_offscreen)
        {
            d_offscreen->Release();
            d_offscreen = 0;
        }
    }

    //------------------------------------------------------------------------//
    void lockRect(RECT* area = 0, bool for_reading = false)
    {
        if (isRenderTarget())
        {
            initialiseSurfaces();

            if (for_reading)
                getRenderTargetData();

            lockSurfaceRect(area);
        }
        else
        {
            lockTextureRect(area);
        }
    }

    //------------------------------------------------------------------------//
    void unlockRect(RECT* area = 0, bool needs_update = false)
    {
        if (isRenderTarget())
        {
            d_offscreen->UnlockRect();

            if (needs_update)
                updateRenderTarget(area);
        }
        else
            d_texture->UnlockRect(0);
    }

    //------------------------------------------------------------------------//
    void populateTextureSurfaceDescription()
    {
        if (FAILED(d_texture->GetLevelDesc(0, &d_surfDesc)))
            CEGUI_THROW(RendererException(
                "IDirect3DTexture9::GetLevelDesc failed."));

        d_fullArea.left = 0;
        d_fullArea.top = 0;
        d_fullArea.right = d_surfDesc.Width;
        d_fullArea.bottom = d_surfDesc.Height;
    }

    //------------------------------------------------------------------------//
    void createOffscreenSurface()
    {
        if (FAILED(d_device->CreateOffscreenPlainSurface(
            d_surfDesc.Width, d_surfDesc.Height, d_surfDesc.Format,
            D3DPOOL_SYSTEMMEM, &d_offscreen, 0)))
        {
            CEGUI_THROW(RendererException(
                "IDirect3DDevice9::CreateOffscreenPlainSurface failed."));
        }
    }

    //------------------------------------------------------------------------//
    void getTextureSurface()
    {
        if (FAILED(d_texture->GetSurfaceLevel(0, &d_renderTarget)))
            CEGUI_THROW(RendererException(
                "IDirect3DTexture9::GetSurfaceLevel failed."));
    }

    //------------------------------------------------------------------------//
    void lockSurfaceRect(RECT* area)
    {
        if (FAILED(d_offscreen->LockRect(&d_lockedRect, area, 0)))
            CEGUI_THROW(RendererException(
                "IDirect3DSurface9::LockRect failed."));
    }

    //------------------------------------------------------------------------//
    void lockTextureRect(RECT* area)
    {
        if (FAILED(d_texture->LockRect(0, &d_lockedRect, area, 0)))
            CEGUI_THROW(RendererException(
                "IDirect3DTexture9::LockRect failed."));
    }

    //------------------------------------------------------------------------//
    void updateRenderTarget(RECT* area)
    {
        POINT pt = {area ? area->left : 0, area ? area->top : 0};
        if (FAILED(d_device->UpdateSurface(d_offscreen,
                                           area ? area : &d_fullArea,
                                           d_renderTarget, &pt)))
        {
            CEGUI_THROW(RendererException(
                "IDirect3DDevice9::UpdateSurface failed."));
        }
    }

    //------------------------------------------------------------------------//
    void getRenderTargetData()
    {
        if (FAILED(d_device->GetRenderTargetData(d_renderTarget, d_offscreen)))
            CEGUI_THROW(RendererException(
                "IDirect3DDevice9::GetRenderTargetData failed."));
    }

    //------------------------------------------------------------------------//
    static void blitD3DCOLORSurfaceToRGBA(const uint32* src, uint32* dst,
                                          const Size& sz, size_t source_pitch)
    {
        for (uint i = 0; i < sz.d_height; ++i)
        {
            for (uint j = 0; j < sz.d_width; ++j)
            {
                const uint32 pixel = src[j];
                const uint32 tmp = pixel & 0x00FF00FF;
                dst[j] = pixel & 0xFF00FF00 | (tmp << 16) | (tmp >> 16);
            }

            src += source_pitch / sizeof(uint32);
            dst += static_cast<uint32>(sz.d_width);
        }
    }
};

//----------------------------------------------------------------------------//
Direct3D9Texture::Direct3D9Texture(Direct3D9Renderer& owner) :
    d_owner(owner),
    d_texture(0),
    d_size(0, 0),
    d_dataSize(0, 0),
    d_texelScaling(0, 0),
    d_savedSurfaceDescValid(false)
{
}

//----------------------------------------------------------------------------//
Direct3D9Texture::Direct3D9Texture(Direct3D9Renderer& owner,
                                   const String& filename,
                                   const String& resourceGroup) :
    d_owner(owner),
    d_texture(0),
    d_size(0, 0),
    d_dataSize(0, 0),
    d_texelScaling(0, 0),
    d_savedSurfaceDescValid(false)
{
    loadFromFile(filename, resourceGroup);
}

//----------------------------------------------------------------------------//
Direct3D9Texture::Direct3D9Texture(Direct3D9Renderer& owner, const Size& sz) :
    d_owner(owner),
    d_texture(0),
    d_size(0, 0),
    d_dataSize(sz),
    d_texelScaling(0, 0),
    d_savedSurfaceDescValid(false)
{
    Size tex_sz(d_owner.getAdjustedSize(sz));

    HRESULT hr = D3DXCreateTexture(d_owner.getDevice(),
                                   static_cast<UINT>(tex_sz.d_width),
                                   static_cast<UINT>(tex_sz.d_height),
                                   1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED,
                                   &d_texture);

    if (FAILED(hr))
        CEGUI_THROW(RendererException(
            "Direct3D9Texture - Failed to create texture of specified size: "
            "D3D Texture creation failed."));

    updateTextureSize();
    updateCachedScaleValues();
}

//----------------------------------------------------------------------------//
Direct3D9Texture::Direct3D9Texture(Direct3D9Renderer& owner,
                                   LPDIRECT3DTEXTURE9 tex) :
    d_owner(owner),
    d_texture(0),
    d_size(0, 0),
    d_dataSize(0, 0),
    d_texelScaling(0, 0),
    d_savedSurfaceDescValid(false)
{
    setDirect3D9Texture(tex);
}

//----------------------------------------------------------------------------//
Direct3D9Texture::~Direct3D9Texture()
{
    cleanupDirect3D9Texture();
}

//----------------------------------------------------------------------------//
void Direct3D9Texture::setDirect3D9Texture(LPDIRECT3DTEXTURE9 tex)
{
    if (d_texture != tex)
    {
        cleanupDirect3D9Texture();
        d_dataSize.d_width = d_dataSize.d_height = 0;

        d_texture = tex;
        if (d_texture)
            d_texture->AddRef();
    }

    updateTextureSize();
    d_dataSize = d_size;
    updateCachedScaleValues();
}

//----------------------------------------------------------------------------//
LPDIRECT3DTEXTURE9 Direct3D9Texture::getDirect3D9Texture() const
{
    return d_texture;
}

//----------------------------------------------------------------------------//
const Size& Direct3D9Texture::getSize() const
{
    return d_size;
}

//----------------------------------------------------------------------------//
const Size& Direct3D9Texture::getOriginalDataSize() const
{
    return d_dataSize;
}

//----------------------------------------------------------------------------//
const Vector2& Direct3D9Texture::getTexelScaling() const
{
    return d_texelScaling;
}

//----------------------------------------------------------------------------//
void Direct3D9Texture::loadFromFile(const String& filename,
                                    const String& resourceGroup)
{
    // get and check existence of CEGUI::System object
    System* sys = System::getSingletonPtr();
    if (!sys)
        CEGUI_THROW(RendererException("Direct3D9Texture::loadFromFile - "
            "CEGUI::System object has not been created!"));

    // load file to memory via resource provider
    RawDataContainer texFile;
    sys->getResourceProvider()->loadRawDataContainer(filename, texFile,
            resourceGroup);

    Texture* res = sys->getImageCodec().load(texFile, this);

    // unload file data buffer
    sys->getResourceProvider()->unloadRawDataContainer(texFile);

    if (!res)
        // It's an error
        CEGUI_THROW(RendererException("Direct3D9Texture::loadFromFile - " +
            sys->getImageCodec().getIdentifierString() +
            " failed to load image '" + filename + "'."));
}

//----------------------------------------------------------------------------//
void Direct3D9Texture::loadFromMemory(const void* buffer,
                                      const Size& buffer_size,
                                      PixelFormat pixel_format)
{
    cleanupDirect3D9Texture();

    // create a texture
    // TODO: Check resulting pixel format and react appropriately.
    D3DFORMAT pixfmt;
    switch (pixel_format)
    {
    case PF_RGB:
        pixfmt = D3DFMT_R8G8B8;
        break;
    case PF_RGBA:
        pixfmt = D3DFMT_A8B8G8R8;
        break;
    default:
        CEGUI_THROW(RendererException("Direct3D9Texture::loadFromMemory failed: "
            "Invalid PixelFormat value specified."));
    }

    Size tex_sz(d_owner.getAdjustedSize(buffer_size));

    HRESULT hr = D3DXCreateTexture(d_owner.getDevice(),
                                   static_cast<UINT>(tex_sz.d_width),
                                   static_cast<UINT>(tex_sz.d_height),
                                   1, 0, pixfmt, D3DPOOL_MANAGED, &d_texture);

    if (FAILED(hr))
        CEGUI_THROW(RendererException("Direct3D9Texture::loadFromMemory failed: "
            "Direct3D9 texture creation failed."));

    d_dataSize = buffer_size;
    updateTextureSize();
    updateCachedScaleValues();

    // lock the D3D texture
    D3DLOCKED_RECT rect;
    hr = d_texture->LockRect(0, &rect, 0, 0);

    if (FAILED(hr))
    {
        d_texture->Release();
        d_texture = 0;

        CEGUI_THROW(RendererException("Direct3D9Texture::loadFromMemory failed: "
            "IDirect3DTexture9::LockRect failed."));
    }

    // copy data from buffer into texture
    ulong* dst = static_cast<ulong*>(rect.pBits);
    const ulong* src = static_cast<const ulong*>(buffer);

    // RGBA
    if (pixel_format == PF_RGBA)
    {
        for (uint i = 0; i < buffer_size.d_height; ++i)
        {
            for (uint j = 0; j < buffer_size.d_width; ++j)
            {
                // we dont need endian safety on microsoft
                uchar r = static_cast<uchar>(src[j] & 0xFF);
                uchar g = static_cast<uchar>((src[j] >> 8) & 0xFF);
                uchar b = static_cast<uchar>((src[j] >> 16)  & 0xFF);
                uchar a = static_cast<uchar>((src[j] >> 24) & 0xFF);

                dst[j] = D3DCOLOR_ARGB(a, r, g, b);
            }

            dst += rect.Pitch / sizeof(ulong);
            src += static_cast<ulong>(buffer_size.d_width);
        }
    }
    // RGB
    else
    {
        for (uint i = 0; i < buffer_size.d_height; ++i)
        {
            for (uint j = 0; j < buffer_size.d_width; ++j)
            {
                uchar r = reinterpret_cast<const uchar*>(src)[j * 3];
                uchar g = reinterpret_cast<const uchar*>(src)[j * 3 + 1];
                uchar b = reinterpret_cast<const uchar*>(src)[j * 3 + 2];
                uchar a = 0xFF;

                dst[j] = D3DCOLOR_ARGB(a, r, g, b);
            }

            dst += rect.Pitch / sizeof(ulong);
            src = reinterpret_cast<const ulong*>
                  (reinterpret_cast<const uchar*>(src) +
                   static_cast<int>(buffer_size.d_width) * 3);
        }
    }

    d_texture->UnlockRect(0);
}

//----------------------------------------------------------------------------//
void Direct3D9Texture::saveToMemory(void* buffer)
{
    D3DSurfaceBlitter blitter(d_owner.getDevice(), d_texture);
    blitter.blitToMemory(static_cast<uint32*>(buffer));
}

//----------------------------------------------------------------------------//
void Direct3D9Texture::cleanupDirect3D9Texture()
{
    if (d_texture)
    {
        d_texture->Release();
        d_texture = 0;
    }
}

//----------------------------------------------------------------------------//
void Direct3D9Texture::updateCachedScaleValues()
{
    //
    // calculate what to use for x scale
    //
    const float orgW = d_dataSize.d_width;
    const float texW = d_size.d_width;

    // if texture and original data width are the same, scale is based
    // on the original size.
    // if texture is wider (and source data was not stretched), scale
    // is based on the size of the resulting texture.
    d_texelScaling.d_x = 1.0f / ((orgW == texW) ? orgW : texW);

    //
    // calculate what to use for y scale
    //
    const float orgH = d_dataSize.d_height;
    const float texH = d_size.d_height;

    // if texture and original data height are the same, scale is based
    // on the original size.
    // if texture is taller (and source data was not stretched), scale
    // is based on the size of the resulting texture.
    d_texelScaling.d_y = 1.0f / ((orgH == texH) ? orgH : texH);
}

//----------------------------------------------------------------------------//
void Direct3D9Texture::updateTextureSize()
{
    D3DSURFACE_DESC surfDesc;

    // obtain details of the size of the texture
    if (d_texture && SUCCEEDED(d_texture->GetLevelDesc(0, &surfDesc)))
    {
        d_size.d_width  = static_cast<float>(surfDesc.Width);
        d_size.d_height = static_cast<float>(surfDesc.Height);
    }
    // use the original size if query failed.
    // NB: This should probably be an exception.
    else
        d_size = d_dataSize;
}

//----------------------------------------------------------------------------//
void Direct3D9Texture::setOriginalDataSize(const Size& sz)
{
    d_dataSize = sz;
    updateCachedScaleValues();
}

//----------------------------------------------------------------------------//
void Direct3D9Texture::preD3DReset()
{
    // if already saved surface info, or we have no texture, do nothing
    if (d_savedSurfaceDescValid || !d_texture)
        return;

    // get info about our texture
    d_texture->GetLevelDesc(0, &d_savedSurfaceDesc);

    // if texture is managed, we have nothing more to do
    if (d_savedSurfaceDesc.Pool == D3DPOOL_MANAGED)
        return;

    // otherwise release texture.
    d_texture->Release();
    d_texture = 0;
    d_savedSurfaceDescValid = true;
}

//----------------------------------------------------------------------------//
void Direct3D9Texture::postD3DReset()
{
    // if texture has no saved surface info, we do nothing.
    if (!d_savedSurfaceDescValid)
        return;

    // otherwise, create a new texture using saved details.
    d_owner.getDevice()->
        CreateTexture(d_savedSurfaceDesc.Width,
                      d_savedSurfaceDesc.Height,
                      1, d_savedSurfaceDesc.Usage, d_savedSurfaceDesc.Format,
                      d_savedSurfaceDesc.Pool, &d_texture, 0);

    d_savedSurfaceDescValid = false;
}

//----------------------------------------------------------------------------//

} // End of  CEGUI namespace section
