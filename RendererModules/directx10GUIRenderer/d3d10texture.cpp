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
#include <algorithm>
//----------------------------------------------------------------------------//
#define D3DCOLOR_ARGB(a,r,g,b) \
	((ulong)((((a)&0xff)<<24)|(((r)&0xff)<<16)|(((g)&0xff)<<8)|((b)&0xff)))
#define D3DCOLOR_RGBA(r,g,b,a) D3DCOLOR_ARGB(a,r,g,b)


// Start of CEGUI namespace section
namespace CEGUI
{
//----------------------------------------------------------------------------//
// The following is a bunch of stuff used to add TGA loading back, since MS
// decided to drop support for it
//----------------------------------------------------------------------------//
#define TGA_RGB 2  // This tells us it's a normal RGB (really BGR) file
#define TGA_A	3  // This tells us it's a ALPHA file
#define TGA_RLE	10 // This tells us that the targa is Run-Length Encoded (RLE)

//!This is our image structure for our targa data
struct ImageTGA
{
    //! The channels in the image (3 = RGB : 4 = RGBA)
    int channels;
    //! The width of the image in pixels
    int sizeX;
    //! The height of the image in pixels
    int sizeY;
    //! The image pixel data
    unsigned char *data;
};

//! flips data for tImageTGA 'img'
void flipImageTGA(ImageTGA* img);
//! load a TGA from a byte buffer 
ImageTGA* loadTGA(const unsigned char* buffer, size_t buffer_size);
//! return whether filename has .tga extension
bool filenameHasTGAExtension(const String& filename);
//! top level function for loading targa files to a texture
void loadTextureFromTargaFile(DirectX10Texture* tex, const String& filename,
                              const String& resourceGroup);

//----------------------------------------------------------------------------//
// End of definitions for TGA loading support
//----------------------------------------------------------------------------//


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
    // handle TGA special case
    if (filenameHasTGAExtension(filename))
    {
        loadTextureFromTargaFile(this, filename, resourceGroup);
        return;
    }

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
                uchar r = ((uchar*)src)[j * 3];
                uchar g = ((uchar*)src)[j * 3 + 1];
                uchar b = ((uchar*)src)[j * 3 + 2];
                uchar a = 255;

                dst[j] = D3DCOLOR_ARGB(a, r, g, b);
            }

            dst += tex_size;
            src = (ulong*)(((uchar*)src) + buffWidth * 3);
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
void flipImageTGA(ImageTGA* img)
{
    
	int pitch = img->sizeX * img->channels;
    
	// flip the image bits...
	for (int line = 0; line < img->sizeY / 2; ++line)
	{
		int srcOffset = (line * pitch);
		int dstOffest = ((img->sizeY - line - 1) * pitch);

		for (int colBit = 0; colBit < pitch; ++colBit)
		{
			uchar tmp = img->data[dstOffest + colBit];
			img->data[dstOffest + colBit] = img->data[srcOffset + colBit];
			img->data[srcOffset + colBit] = tmp;
		}

	}
}

//----------------------------------------------------------------------------//
///////////////////////////////// LOAD TGA \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This loads the TGA file and returns it's data in a tImageTGA struct
/////
/////   Modified by Paul D Turner to accept a raw data buffer & it's length
/////   as input.
/////
///////////////////////////////// LOAD TGA \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
// Took this code from http://www.gametutorials.com still ne
// tImageTGA *LoadTGA(const char *filename)
//
// This is our cool function that loads the targa (TGA) file, then returns it's
// data.  
// This tutorial supports 16, 24 and 32 bit images, along with RLE compression.
//
//
// Ben Humphrey (DigiBen)
// Game Programmer
// DigiBen@GameTutorials.com
// Co-Web Host of www.GameTutorials.com
ImageTGA* loadTGA(const unsigned char* buffer,
    size_t buffer_size)
{
    typedef unsigned char byte;
    
    ImageTGA *pImageData = 0;		// This stores our important image data
	short width = 0, height = 0;	// The dimensions of the image
	byte length = 0;				// The length in bytes to the pixels
	byte imageType = 0;	     		// The image type (RLE, RGB, Alpha...)

	byte bits = 0;	    			// The bits per pixel for the image (16, 24, 32)
	int channels = 0;				// The channels of the image (3 = RGA : 4 = RGBA)
	int stride = 0;					// The stride (channels * width)
	int i = 0;						// A counter

    // This function loads in a TARGA (.TGA) file and returns its data to be
	// used as a texture or what have you.  This currently loads in a 16, 24
	// and 32-bit targa file, along with RLE compressed files.  Eventually you
	// will want to do more error checking to make it more robust.  This is
	// also a perfect start to go into a modular class for an engine.
	// Basically, how it works is, you read in the header information, then
	// move your file pointer to the pixel data.  Before reading in the pixel
	// data, we check to see the if it's an RLE compressed image.  This is because
	// we will handle it different.  If it isn't compressed, then we need another
	// check to see if we need to convert it from 16-bit to 24 bit.  24-bit and
	// 32-bit textures are very similar, so there's no need to do anything special.
	// We do, however, read in an extra bit for each color.

    	// Allocate the structure that will hold our eventual image data (must free it!)
	pImageData = new ImageTGA;

	// Read in the length in bytes from the header to the pixel data
	memcpy(&length, buffer, sizeof(byte));
	buffer += sizeof(byte);

	// Jump over one byte
	++buffer;

	// Read in the imageType (RLE, RGB, etc...)
	memcpy(&imageType, buffer, sizeof(byte));
	buffer += sizeof(byte);

	// Skip past general information we don't care about
	buffer += 9;

	// Read the width, height and bits per pixel (16, 24 or 32)
	memcpy(&width, buffer, sizeof(short));
	buffer += sizeof(short);
	memcpy(&height, buffer, sizeof(short));
	buffer += sizeof(short);
#ifdef __BIG_ENDIAN__
    width = ((width & 0xFF00)>>8) | ((width & 0x00FF)<<8);
    height = ((height & 0xFF00)>>8) | ((height & 0x00FF)<<8);
#endif
	memcpy(&bits, buffer, sizeof(byte));
	buffer += sizeof(byte);

	// Now we move the file pointer to the pixel data
	buffer += length + 1;

	// Check if the image is RLE compressed or not
	if(imageType != TGA_RLE)
	{
		// Check if the image is a 24 or 32-bit image
		if(bits == 24 || bits == 32)
		{
			// Calculate the channels (3 or 4) - (use bits >> 3 for more speed).
			// Next, we calculate the stride and allocate enough memory for the pixels.
			channels = bits / 8;
			stride = channels * width;
			pImageData->data = new unsigned char[stride * height];

			// Load in all the pixel data line by line
			for(int y = 0; y < height; y++)
			{
				// Store a pointer to the current line of pixels
				unsigned char *pLine = &(pImageData->data[stride * y]);

				// Read in the current line of pixels
				memcpy(pLine, buffer, stride);
				buffer += stride;
			}
		}
		// Check if the image is a 16 bit image (RGB stored in 1 unsigned short)
		else if(bits == 16)
		{
			unsigned short pixels = 0;
			unsigned char r=0, g=0, b=0;

			// Since we convert 16-bit images to 24 bit, we hardcode the channels to 3.
			// We then calculate the stride and allocate memory for the pixels.
			channels = 3;
			stride = channels * width;
			pImageData->data = new unsigned char[stride * height];

			// Load in all the pixel data pixel by pixel
			for(int i = 0; i < width*height; i++)
			{
				// Read in the current pixel
				memcpy(&pixels, buffer, sizeof(unsigned short));
				buffer += sizeof(unsigned short);

				// To convert a 16-bit pixel into an R, G, B, we need to
				// do some masking and such to isolate each color value.
				// 0x1f = 11111 in binary, so since 5 bits are reserved in
				// each unsigned short for the R, G and B, we bit shift and mask
				// to find each value.  We then bit shift up by 3 to get the full color.
				b = static_cast<unsigned char>((pixels & 0x1f) << 3);
				g = static_cast<unsigned char>(((pixels >> 5) & 0x1f) << 3);
				r = static_cast<unsigned char>(((pixels >> 10) & 0x1f) << 3);

				// This essentially assigns the color to our array
				pImageData->data[i * 3 + 0] = r;
				pImageData->data[i * 3 + 1] = g;
				pImageData->data[i * 3 + 2] = b;
			}
		}	
		// Else return a NULL for a bad or unsupported pixel format
		else
			return 0;
	}
	// Else, it must be Run-Length Encoded (RLE)
	else
	{
		// First, let me explain real quickly what RLE is.  
		// For further information, check out Paul Bourke's intro article at: 
		// http://astronomy.swin.edu.au/~pbourke/dataformats/rle/
		// 
		// Anyway, we know that RLE is a basic type compression.  It takes
		// colors that are next to each other and then shrinks that info down
		// into the color and a integer that tells how much of that color is used.
		// For instance:
		// aaaaabbcccccccc would turn into a5b2c8
		// Well, that's fine and dandy and all, but how is it down with RGB colors?
		// Simple, you read in an color count (rleID), and if that number is less than 128,
		// it does NOT have any optimization for those colors, so we just read the next
		// pixels normally.  Say, the color count was 28, we read in 28 colors like normal.
		// If the color count is over 128, that means that the next color is optimized and
		// we want to read in the same pixel color for a count of (colorCount - 127).
		// It's 127 because we add 1 to the color count, as you'll notice in the code.

		// Create some variables to hold the rleID, current colors read, channels, & stride.
		unsigned char rleID = 0;
		int colorsRead = 0;
		channels = bits / 8;
		stride = channels * width;

		// Next we want to allocate the memory for the pixels and create an array,
		// depending on the channel count, to read in for each pixel.
		pImageData->data = new unsigned char[stride * height];
		byte *pColors = new byte [channels];

		// Load in all the pixel data
		while(i < width*height)
		{
			// Read in the current color count + 1
			memcpy(&rleID, buffer, sizeof(unsigned char));
			buffer += sizeof(unsigned char);

			// Check if we don't have an encoded string of colors
			if(rleID < 128)
			{
				// Increase the count by 1
				rleID++;

				// Go through and read all the unique colors found
				while(rleID)
				{
					// Read in the current color
					memcpy(pColors, buffer, sizeof(byte) * channels);
					buffer += sizeof(byte) * channels;

					// Store the current pixel in our image array
					pImageData->data[colorsRead + 0] = pColors[0];
					pImageData->data[colorsRead + 1] = pColors[1];
					pImageData->data[colorsRead + 2] = pColors[2];

					// If we have a 4 channel 32-bit image, assign one more for the alpha
					if(bits == 32)
						pImageData->data[colorsRead + 3] = pColors[3];

					// Increase the current pixels read, decrease the amount
					// of pixels left, and increase the starting index for the next pixel.
					i++;
					rleID--;
					colorsRead += channels;
				}
			}
			// Else, let's read in a string of the same character
			else
			{
				// Minus the 128 ID + 1 (127) to get the color count that needs to be read
				rleID -= 127;

				// Read in the current color, which is the same for a while
				memcpy(pColors, buffer, sizeof(byte) * channels);
				buffer += sizeof(byte) * channels;

				// Go and read as many pixels as are the same
				while(rleID)
				{
					// Assign the current pixel to the current index in our pixel array
					pImageData->data[colorsRead + 0] = pColors[0];
					pImageData->data[colorsRead + 1] = pColors[1];
					pImageData->data[colorsRead + 2] = pColors[2];

					// If we have a 4 channel 32-bit image, assign one more for the alpha
					if(bits == 32)
						pImageData->data[colorsRead + 3] = pColors[3];

					// Increase the current pixels read, decrease the amount
					// of pixels left, and increase the starting index for the next pixel.
					i++;
					rleID--;
					colorsRead += channels;
				}

			}

		}
		// Free up pColors
		delete[] pColors;
	}
	// Fill in our tImageTGA structure to pass back
	pImageData->channels = channels;
	pImageData->sizeX    = width;
	pImageData->sizeY    = height;

	// Return the TGA data (remember, you must free this data after you are done)
	return pImageData;
}

//----------------------------------------------------------------------------//
bool filenameHasTGAExtension(const String& filename)
{
    // must be >=4 chars to have .tga extension!
    if (filename.length() < 4)
        return false;

    std::string file_ext = filename.substr(filename.length() - 4, 4).c_str();
    // get extension as all lowercase letters
    std::transform(file_ext.begin(), file_ext.end(), file_ext.begin(), tolower);
    // return if extension is what we were looking for
    return file_ext == ".tga";
}

//----------------------------------------------------------------------------//
void loadTextureFromTargaFile(DirectX10Texture* tex, const String& filename,
    const String& resourceGroup)
{
	// load file to memory via resource provider
	RawDataContainer texFile;
	System::getSingleton().getResourceProvider()->
        loadRawDataContainer(filename, texFile, resourceGroup);

    ImageTGA* img = loadTGA(texFile.getDataPtr(), texFile.getSize());

    // unload file data buffer
	System::getSingleton().getResourceProvider()->unloadRawDataContainer(texFile);

    if (!img)
		throw RendererException("DirectX10Texture object failed to load TGA " 
                                "file: " + filename + "' from resource group: "
                                + resourceGroup);

    flipImageTGA(img);
    Texture::PixelFormat fmt = (img->channels == 3) ? Texture::PF_RGB :
                                                      Texture::PF_RGBA;

    tex->loadFromMemory(img->data, img->sizeX, img->sizeY, fmt);

    // cleanup
    if (img->data)							
        delete[] img->data; 

    delete img;
}

} // End of  CEGUI namespace section
