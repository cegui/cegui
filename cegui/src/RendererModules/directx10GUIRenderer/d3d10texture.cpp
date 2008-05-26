/***********************************************************************
    filename:   d3d10texture.cpp
    created:    17/7/2004
    author:     Paul D Turner with D3D 9 Updates by Magnus Österlind
                D3D10 Port by Rajko Stojadinovic

    purpose:    Implements texture class for D3D10.0 system
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
#include "d3d10texture.h"
#include "d3d10renderer.h"
#include "CEGUIExceptions.h"
#include "CEGUISystem.h"

#include <d3dx10.h>
#include <dxerr.h>
#undef max

#define D3DCOLOR_ARGB(a,r,g,b) \
	((ulong)((((a)&0xff)<<24)|(((r)&0xff)<<16)|(((g)&0xff)<<8)|((b)&0xff)))
#define D3DCOLOR_RGBA(r,g,b,a) D3DCOLOR_ARGB(a,r,g,b)


// Start of CEGUI namespace section
namespace CEGUI
{
//----------------------------------------------------------------------------//
DirectX10Texture::DirectX10Texture(Renderer* owner) :
    Texture(owner),
    d_d3dtexture(0),
    d_srview(0),
    d_isMemoryTexture(true),
    d_width(0),
    d_height(0),
    d_orgWidth(0),
    d_orgHeight(0)
{
}

//----------------------------------------------------------------------------//
DirectX10Texture::~DirectX10Texture(void)
{
    freeD3DTexture();
}

//----------------------------------------------------------------------------//
void DirectX10Texture::loadFromFile(const String& filename,
    const String& resourceGroup)
{
    freeD3DTexture();

    // load the file via the resource provider
    RawDataContainer texFile;
    System::getSingleton().getResourceProvider()->
        loadRawDataContainer(filename, texFile, resourceGroup);

    ID3D10Resource* pRes = 0;

    D3DX10_IMAGE_LOAD_INFO loadInfo;
    ZeroMemory(&loadInfo,sizeof(loadInfo));
    loadInfo.Width = D3DX10_DEFAULT;
    loadInfo.Height = D3DX10_DEFAULT;
    loadInfo.Depth = D3DX10_DEFAULT;
    loadInfo.MipLevels = 1;
    loadInfo.Usage = D3D10_USAGE_IMMUTABLE;
    loadInfo.BindFlags = D3D10_BIND_SHADER_RESOURCE;
    loadInfo.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    loadInfo.Filter = D3DX10_FILTER_NONE;
    loadInfo.MipFilter = D3DX10_FILTER_NONE;

    D3DX10_IMAGE_INFO imageInfo;

    HRESULT hr =
        D3DX10GetImageInfoFromMemory(texFile.getDataPtr(),
                                     static_cast<UINT>(texFile.getSize()), 0,
                                     &imageInfo, 0);

    if (SUCCEEDED(hr))
    {
        d_orgHeight = imageInfo.Height;
        d_orgWidth = imageInfo.Width;

        loadInfo.pSrcInfo = &imageInfo;

        hr = D3DX10CreateTextureFromMemory(
                ((DirectX10Renderer*)getRenderer())->getDevice(),
                texFile.getDataPtr(), static_cast<UINT>(texFile.getSize()), 
                &loadInfo, 0, &pRes, 0);

        if (SUCCEEDED(hr))
        {
            pRes->QueryInterface(__uuidof( ID3D10Texture2D ), (LPVOID*)&d_d3dtexture);

            // now obtain details of the size of the texture
            D3D10_TEXTURE2D_DESC surfDesc;
            d_d3dtexture->GetDesc(&surfDesc);

            d_width = static_cast<ushort>(surfDesc.Width);
            d_height = static_cast<ushort>(surfDesc.Height);

            d_filename = filename;
            d_resourceGroup = resourceGroup;
            d_isMemoryTexture = false;

            D3D10_SHADER_RESOURCE_VIEW_DESC srvDesc;
            srvDesc.Format = surfDesc.Format;
            srvDesc.ViewDimension = D3D10_SRV_DIMENSION_TEXTURE2D;
            srvDesc.Texture2D.MostDetailedMip = 0;
            srvDesc.Texture2D.MipLevels = surfDesc.MipLevels;
            ((DirectX10Renderer*)getRenderer())->getDevice()->
                CreateShaderResourceView(pRes, &srvDesc, &d_srview);
            pRes->Release();
        }
    }
    else
    {
        throw RendererException("Failed to create Texture object from file '" +
                                filename + "'.  Additional Info: " +
                                DXGetErrorString(hr));
    }

    System::getSingleton().getResourceProvider()->
        unloadRawDataContainer(texFile);
}

//----------------------------------------------------------------------------//
void DirectX10Texture::loadFromMemory(const void* buffPtr, uint buffWidth,
    uint buffHeight, Texture::PixelFormat pixFormat)
{
    using namespace std;

    // release old texture
    freeD3DTexture();

    // calculate square size big enough for whole memory buffer
    uint tex_size = ceguimax(buffWidth, buffHeight);

    // create a texture
    // TODO: Check resulting pixel format and react appropriately.
    DXGI_FORMAT pixfmt;
    switch (pixFormat)
    {
    case PF_RGB:
        pixfmt = DXGI_FORMAT_R8G8B8A8_UNORM;
        break;
    case PF_RGBA:
        pixfmt = DXGI_FORMAT_R8G8B8A8_UNORM;
        break;
    default:
        throw RendererException("Failed to load texture from memory: "
                                "Invalid pixelformat.");
    }

    // copy data from buffer into texture
    ulong* dst = (ulong*)new char[tex_size * tex_size * 4];
    ZeroMemory(dst, tex_size * tex_size * 4);
    ulong* goods = dst;
    ulong* src = (ulong*)buffPtr;

    // RGBA
    if (pixFormat == PF_RGBA)
    {
        for (uint i = 0; i < buffHeight; ++i)
        {
            for (uint j = 0; j < buffWidth; ++j)
            {
                // we don't need endian safety on Microsoft
                uchar r = (uchar)(src[j] & 0xFF);
                uchar g = (uchar)((src[j] >> 8) & 0xFF);
                uchar b = (uchar)((src[j] >> 16) & 0xFF);
                uchar a = (uchar)((src[j] >> 24) & 0xFF);

                dst[j] = D3DCOLOR_ARGB(a, r, g, b);
            }

            dst += tex_size;
            src += buffWidth;
        }
    }
    // RGB
    else
    {
        for (uint i = 0; i < buffHeight; ++i)
        {
            for (uint j = 0; j < buffWidth; ++j)
            {
                uchar r = (uchar)(src[j] & 0xFF);
                uchar g = (uchar)((src[j] >> 8) & 0xFF);
                uchar b = (uchar)((src[j] >> 16)  & 0xFF);
                uchar a = 255;

                dst[j] = D3DCOLOR_ARGB(a, r, g, b);
            }

            dst += tex_size;
            src += buffWidth;
        }
    }

    D3D10_TEXTURE2D_DESC descTex;
    ZeroMemory(&descTex, sizeof(descTex));
    descTex.Width = tex_size;
    descTex.Height = tex_size;
    descTex.ArraySize = 1;
    descTex.SampleDesc.Count = 1;
    descTex.SampleDesc.Quality = 0;
    descTex.Format = pixfmt;
    descTex.Usage = D3D10_USAGE_DEFAULT;
    descTex.BindFlags = D3D10_BIND_SHADER_RESOURCE;
    descTex.CPUAccessFlags = 0;
    descTex.MiscFlags = 0;
    descTex.MipLevels = 1;

    D3D10_SUBRESOURCE_DATA data[1];
    ZeroMemory(&data, sizeof(D3D10_SUBRESOURCE_DATA));
    data[0].pSysMem = goods;
    data[0].SysMemPitch = 4 * tex_size;

    HRESULT hr = ((DirectX10Renderer*)getRenderer())->getDevice()->
        CreateTexture2D(&descTex, data, &d_d3dtexture);

    if (FAILED(hr))
    {
        throw RendererException("Failed to load texture from memory: "
                                "D3D Texture creation failed.");
    }
    else
    {
        d_orgWidth = buffWidth;
        d_orgHeight = buffHeight;
        obtainActualTextureSize();
    }

    ID3D10Resource*  pRes = 0;
    d_d3dtexture->QueryInterface(__uuidof( ID3D10Resource ), (LPVOID*)&pRes);

    D3D10_SHADER_RESOURCE_VIEW_DESC srvDesc;
    srvDesc.Format = descTex.Format;
    srvDesc.ViewDimension = D3D10_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MostDetailedMip = 0;
    srvDesc.Texture2D.MipLevels = descTex.MipLevels;
    ((DirectX10Renderer*)getRenderer())->getDevice()->
        CreateShaderResourceView(pRes, &srvDesc, &d_srview);
    pRes->Release();
}

//----------------------------------------------------------------------------//
void DirectX10Texture::freeD3DTexture(void)
{
    if (d_d3dtexture)
    {
        d_d3dtexture->Release();
        d_d3dtexture = 0;
    }

    if (d_srview)
    {
        d_srview->Release();
        d_srview = 0;
    }

    d_filename.clear();
    d_isMemoryTexture = true;
}

//----------------------------------------------------------------------------//
void DirectX10Texture::setD3DTextureSize(uint size)
{
    freeD3DTexture();

    D3D10_TEXTURE2D_DESC descTex;
    descTex.Width = size;
    descTex.Height = size;
    descTex.ArraySize = 1;
    descTex.SampleDesc.Count = 1;
    descTex.SampleDesc.Quality = 0;
    descTex.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    descTex.Usage = D3D10_USAGE_DEFAULT;
    descTex.BindFlags = D3D10_BIND_SHADER_RESOURCE;
    descTex.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE | D3D10_CPU_ACCESS_READ;
    descTex.MiscFlags = 0;
    descTex.MipLevels = 1;

    HRESULT hr = ((DirectX10Renderer*)getRenderer())->getDevice()->
        CreateTexture2D(&descTex, 0, &d_d3dtexture);

    if (FAILED(hr))
    {
        throw RendererException("Failed to create texture of specified size: "
                                "D3D Texture creation failed.");
    }
    else
    {
        d_orgWidth = d_orgHeight = size;
        obtainActualTextureSize();

        ID3D10Resource*  pRes    = 0;
        d_d3dtexture->QueryInterface(__uuidof( ID3D10Resource ), (LPVOID*)&pRes);

        D3D10_SHADER_RESOURCE_VIEW_DESC srvDesc;
        srvDesc.Format = descTex.Format;
        srvDesc.ViewDimension = D3D10_SRV_DIMENSION_TEXTURE2D;
        srvDesc.Texture2D.MostDetailedMip = 0;
        srvDesc.Texture2D.MipLevels = descTex.MipLevels;
        ((DirectX10Renderer*)getRenderer())->getDevice()->
            CreateShaderResourceView(pRes, &srvDesc, &d_srview);
        pRes->Release();
    }
}

//----------------------------------------------------------------------------//
void DirectX10Texture::obtainActualTextureSize(void)
{
    // now obtain details of the size of the texture
    D3D10_TEXTURE2D_DESC surfDesc;
    d_d3dtexture->GetDesc(&surfDesc);

    d_width = static_cast<ushort>(surfDesc.Width);
    d_height = static_cast<ushort>(surfDesc.Height);
}

//----------------------------------------------------------------------------//

} // End of  CEGUI namespace section
