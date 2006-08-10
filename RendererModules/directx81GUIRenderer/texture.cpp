/***********************************************************************
	filename: 	texture.cpp
	created:	10/4/2004
	author:		Paul D Turner
	
	purpose:	Implements texture class for D3D8.1 system
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
#include "texture.h"
#include "renderer.h"
#include "CEGUIExceptions.h"
#include "CEGUISystem.h"

#include <d3dx8.h>
#include <dxerr8.h>

// remove Microsoft idiocy
#undef max
#undef min


// Start of CEGUI namespace section
namespace CEGUI
{

/*************************************************************************
	Constructor
*************************************************************************/
DirectX81Texture::DirectX81Texture(Renderer* owner) :
	Texture(owner),
	d_d3dtexture(0),
	d_isMemoryTexture(true),
    d_width(0),
    d_height(0),
    d_orgWidth(0),
    d_orgHeight(0)
{
}

/*************************************************************************
	Destructor
*************************************************************************/
DirectX81Texture::~DirectX81Texture(void)
{
	freeD3DTexture();
}

/*************************************************************************
	Load texture from file.  Texture is made to be same size as image in
	file.
*************************************************************************/
void DirectX81Texture::loadFromFile(const String& filename, const String& resourceGroup)
{
	freeD3DTexture();

	// load the file via the resource provider
	RawDataContainer texFile;
	System::getSingleton().getResourceProvider()->loadRawDataContainer(filename, texFile, resourceGroup);
	
	D3DXIMAGE_INFO texInfo;
	HRESULT hr = D3DXCreateTextureFromFileInMemoryEx(((DirectX81Renderer*)getRenderer())->getDevice(), texFile.getDataPtr(),
            static_cast<UINT>(texFile.getSize()), D3DX_DEFAULT, D3DX_DEFAULT, 1, 0, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT,
            D3DX_DEFAULT, D3DX_DEFAULT, 0, &texInfo, 0, &d_d3dtexture);

	System::getSingleton().getResourceProvider()->unloadRawDataContainer(texFile);

	if (SUCCEEDED(hr))
	{
        // these are the size of the image
		d_orgWidth	= static_cast<ushort>(texInfo.Width);
		d_orgHeight	= static_cast<ushort>(texInfo.Height);

        obtainActualTextureSize();

		d_filename = filename;
        d_resourceGroup = resourceGroup;
		d_isMemoryTexture = false;
	}
	else
	{
		throw RendererException("Failed to create Texture object from file '" + filename + "'.  Additional Info: " + DXGetErrorString8(hr));
	}

}


/*************************************************************************
	Load texture from raw memory.	
*************************************************************************/
void DirectX81Texture::loadFromMemory(const void* buffPtr, uint buffWidth, uint buffHeight, Texture::PixelFormat pixFormat)
{
	using namespace std;

	// release old texture
	freeD3DTexture();

	// calculate square size big enough for whole memory buffer
	uint tex_size = ceguimax(buffWidth, buffHeight);

	// create a texture
	// TODO: Check resulting pixel format and react appropriately.
	D3DFORMAT pixfmt;
	switch (pixFormat)
	{
	case PF_RGB:
	    pixfmt = D3DFMT_R8G8B8;
	    break;
	case PF_RGBA:
	    pixfmt = D3DFMT_A8R8G8B8;
	    break;
	default:
	    throw RendererException("Failed to load texture from memory: Invalid pixelformat.");
	}
	HRESULT hr = D3DXCreateTexture(((DirectX81Renderer*)getRenderer())->getDevice(), tex_size, tex_size, 1, 0, pixfmt, D3DPOOL_MANAGED, &d_d3dtexture);

	if (FAILED(hr))
	{
		throw RendererException("Failed to load texture from memory: D3D Texture creation failed.");
	}
	else
	{
        d_orgWidth = buffWidth;
        d_orgHeight = buffHeight;
        obtainActualTextureSize();

		// lock the D3D texture
		D3DLOCKED_RECT	rect;
		hr = d_d3dtexture->LockRect(0, &rect, 0, 0);

		if (FAILED(hr))
		{
			d_d3dtexture->Release();
			d_d3dtexture = 0;

			throw RendererException("Failed to load texture from memory: IDirect3DTexture8::LockRect failed.");
		}
		else
		{
			// copy data from buffer into texture
			ulong* dst = (ulong*)rect.pBits;
			ulong* src = (ulong*)buffPtr;

            // RGBA
            if (pixFormat == PF_RGBA)
            {
                for (uint i = 0; i < buffHeight; ++i)
			    {
				    for (uint j = 0; j < buffWidth; ++j)
				    {
				        // we dont need endian safety on microsoft
					    uchar r = (uchar)(src[j] & 0xFF);
					    uchar g = (uchar)((src[j] >> 8) & 0xFF);
					    uchar b = (uchar)((src[j] >> 16)  & 0xFF);
					    uchar a = (uchar)((src[j] >> 24) & 0xFF);

					    dst[j] = D3DCOLOR_ARGB(a, r, g, b);
				    }

				    dst += rect.Pitch / sizeof(ulong);
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
				        dst[j] = src[j];
				    }

				    dst += rect.Pitch / sizeof(ulong);
				    src += buffWidth;
			    }
            }

			d_d3dtexture->UnlockRect(0);
		}

	}

}


/*************************************************************************
	safely release D3D texture associated with this Texture
*************************************************************************/
void DirectX81Texture::freeD3DTexture(void)
{
	if (d_d3dtexture)
	{
		d_d3dtexture->Release();
		d_d3dtexture = 0;
	}

	d_filename.clear();
	d_isMemoryTexture = true;
}


/*************************************************************************
	allocate a D3D texture >= 'size'.  Previous D3D texture is lost
*************************************************************************/
void DirectX81Texture::setD3DTextureSize(uint size)
{
	freeD3DTexture();

	HRESULT hr = D3DXCreateTexture(((DirectX81Renderer*)getRenderer())->getDevice(), size, size, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &d_d3dtexture);

	if (FAILED(hr))
	{
		throw RendererException("Failed to create texture of specified size: D3D Texture creation failed.");
	}
	else
	{
        d_orgWidth = d_orgHeight = size;
        obtainActualTextureSize();
	}

}


/*************************************************************************
	Direct3D support method that must be called prior to a Reset call
	on the Direct3DDevice.
*************************************************************************/
void DirectX81Texture::preD3DReset(void)
{
	// textures not based on files are in the managed pool, so we do
	// not worry about those.
	if (!d_isMemoryTexture)
	{
		// release the d3d texture
		if (d_d3dtexture)
		{
			if (FAILED(d_d3dtexture->Release()))
			{
				throw RendererException("DirectX81Texture::preD3DReset - failed to release the Direct3DTexture8 object for this texture.");
			}

			d_d3dtexture = 0;
		}

	}

}


/*************************************************************************
	Direct3D support method that must be called after a Reset call on the
	Direct3DDevice.
*************************************************************************/
void DirectX81Texture::postD3DReset(void)
{
	// textures not based on files are in the managed pool, so we do
	// not worry about those.
	if (!d_isMemoryTexture)
	{
		// The reason this copy is made is that d_filename will be reset once we
		// call loadFromFile and loading would always fail due to invalid filenames.
		String name(d_filename);

		// re-load the texture
		loadFromFile(name, d_resourceGroup);
	}

}

/*************************************************************************
    Obtains actual texture size and fills in d_width and d_height.
*************************************************************************/
void DirectX81Texture::obtainActualTextureSize(void)
{
    // now obtain details of the size of the texture
    D3DSURFACE_DESC surfDesc;
    HRESULT hr = d_d3dtexture->GetLevelDesc(0, &surfDesc);

    if (SUCCEEDED(hr))
    {
        d_width = static_cast<ushort>(surfDesc.Width);
        d_height = static_cast<ushort>(surfDesc.Height);
    }
    // just use the original sizes again.  This should probably be an exception.
    else
    {
        d_width = d_orgWidth;
        d_height = d_orgHeight;
    }
}

} // End of  CEGUI namespace section
