/***********************************************************************
    filename:   directfb-texture.cpp
    author:     Keith Mok
*************************************************************************/
/***************************************************************************
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
#include "directfb-texture.h"
#include "CEGUIExceptions.h"
#include "CEGUISystem.h"
#include "CEGUIImageCodec.h"
// Start of CEGUI namespace section
namespace CEGUI
{

/*************************************************************************
    Constructor
*************************************************************************/
DirectfbTexture::DirectfbTexture(Renderer* owner) :
    Texture(owner)
{
    d_fbtexture = 0;
    d_width = 0;
    d_height = 0;
}

/*************************************************************************
    Destructor
*************************************************************************/
DirectfbTexture::~DirectfbTexture(void)
{
    freeFBTexture();
}


/*************************************************************************
    Load texture from file.  Texture is made to be same size as image in
    file.
*************************************************************************/
void DirectfbTexture::loadFromFile(const String& filename, const String& resourceGroup)
{
    // Note from PDT:
    // There is somewhat tight coupling here between OpenGLTexture and the
    // ImageCodec classes - we have intimate knowledge of how they are
    // implemented and that knowledge is relied upon in an unhealthy way; this
    // should be addressed at some stage.

    DirectfbRenderer* renderer =  static_cast<DirectfbRenderer*>(getRenderer());
    // load file to memory via resource provider
    RawDataContainer texFile;
    System::getSingleton().getResourceProvider()->loadRawDataContainer(filename, texFile, resourceGroup);
    Texture* res = renderer->getImageCodec().load(texFile, this);
    // unload file data buffer
    System::getSingleton().getResourceProvider()->unloadRawDataContainer(texFile);
    if (res == 0)
    {
        // It's an error
        throw RendererException("DirectfbTexture::loadFromFile - " +
                renderer->getImageCodec().getIdentifierString() +
                " failed to load image '" + filename + "'.");
    }

}


/*************************************************************************
    Load texture from raw memory.
*************************************************************************/
void DirectfbTexture::loadFromMemory(const void* buffPtr, uint buffWidth, uint buffHeight, Texture::PixelFormat pixFormat)
{
    freeFBTexture();
        DFBResult hr;
    DFBSurfaceDescription desc;
    DFBSurfacePixelFormat format;
    IDirectFB *dfb;

    dfb = ((DirectfbRenderer*)getRenderer())->getDevice();

    switch (pixFormat)
    {
        case PF_RGB:
            format = DSPF_RGB24;
            break;
        case PF_RGBA:
            format = DSPF_ARGB;
            break;
        default:
            throw RendererException("Invalid pixFormat pass to DirectfbTexture.");
            break;
    };

    desc.flags = (DFBSurfaceDescriptionFlags)(DSDESC_WIDTH|DSDESC_HEIGHT|DSDESC_PIXELFORMAT);
    desc.width = buffWidth;
    desc.height = buffHeight;
    desc.pixelformat = format;

    hr = dfb->CreateSurface(dfb, &desc, &d_fbtexture);

        if (hr)
        {
                throw RendererException("Failed to create texture of specified size: Directfb Texture creation failed.");
        }
        else
        {
        uint32 *ptr;
        int pitch;
        hr = d_fbtexture->Lock(d_fbtexture, DSLF_WRITE, (void**)&ptr, &pitch);
        if(hr)
        {
            d_fbtexture->Release(d_fbtexture);
            d_fbtexture = 0;
                        throw RendererException("Failed to load texture from memory: Directfb::Lock failed.");
        }
        else
        {
                        // copy data from buffer into texture

            // RGBA
            if (pixFormat == PF_RGBA)
            {
                uint32* dst = ptr;
                uint32* src = (uint32*)buffPtr;
                for (uint i = 0; i < buffHeight; ++i)
                {
                    for (uint j = 0; j < buffWidth; ++j)
                    {
                        uchar r = (uchar)(src[j] & 0xFF);
                        uchar g = (uchar)((src[j] >> 8) & 0xFF);
                        uchar b = (uchar)((src[j] >> 16)  & 0xFF);
                        uchar a = (uchar)((src[j] >> 24) & 0xFF);

                        dst[j] = (((a) << 24) | ((r) << 16) | ((g) << 8)  | (b));
                    }

                    dst += pitch / sizeof(uint32);
                    src += buffWidth;
                }
            }
            // RGB
            else
            {
                uint8* dst = (uint8*)ptr;
                uint8* src = (uint8*)buffPtr;
                for (uint i = 0; i < buffHeight; ++i)
                {
                    for (uint j = 0; j < buffWidth*3; j+=3)
                    {
                        dst[j] = src[j+2];
                        dst[j+1] = src[j+1];
                        dst[j+2] = src[j];
                    }
                    dst += pitch;
                    src += buffWidth*3;
                }
            }

            d_fbtexture->Unlock(d_fbtexture);
        }
        }
    d_width = buffWidth;
    d_height = buffHeight;
}


/*************************************************************************
 *         safely release Directfb texture associated with this Texture
 **************************************************************************/
void DirectfbTexture::freeFBTexture(void)
{
        if (d_fbtexture)
        {
                d_fbtexture->Release(d_fbtexture);
                d_fbtexture = 0;
        }
        d_width = d_height = 0;
}


/*************************************************************************
    Previous texture is lost
*************************************************************************/
void DirectfbTexture::setFBTextureSize(uint size)
{
    freeFBTexture();

        DFBResult hr;
    DFBSurfaceDescription desc;
    IDirectFB *dfb;

    dfb = ((DirectfbRenderer*)getRenderer())->getDevice();
    desc.flags = (DFBSurfaceDescriptionFlags)(DSDESC_WIDTH|DSDESC_HEIGHT|DSDESC_PIXELFORMAT);
    desc.width = size;
    desc.height = size;
    desc.pixelformat = DSPF_ARGB;

    hr = dfb->CreateSurface(dfb, &desc, &d_fbtexture);

        if (hr)
        {
                throw RendererException("Failed to create texture of specified size: Directfb Texture creation failed.");
        }
        else
        {
        d_width = d_height = size;
        }
}

} // End of  CEGUI namespace section
