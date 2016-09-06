/***********************************************************************
    created:    Tue Mar 10 2009
    author:     Paul D Turner (parts based on code by Keith Mok)
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
#include "CEGUI/RendererModules/DirectFB/Texture.h"
#include "CEGUI/Exceptions.h"
#include "CEGUI/System.h"
#include "CEGUI/ImageCodec.h"

// Start of CEGUI namespace section
namespace CEGUI
{
//----------------------------------------------------------------------------//
IDirectFBSurface* DirectFBTexture::getDirectFBSurface() const
{
    return d_texture;
}

//----------------------------------------------------------------------------//
const String& DirectFBTexture::getName() const
{
    return d_name;
}

//----------------------------------------------------------------------------//
const Sizef& DirectFBTexture::getSize() const
{
    return d_size;
}

//----------------------------------------------------------------------------//
const Sizef& DirectFBTexture::getOriginalDataSize() const
{
    return d_dataSize;
}

//----------------------------------------------------------------------------//
const Vector2f& DirectFBTexture::getTexelScaling() const
{
    return d_texelScaling;
}

//----------------------------------------------------------------------------//
void DirectFBTexture::loadFromFile(const String& filename,
                                   const String& resourceGroup)
{
    // get and check existence of CEGUI::System object
    System* sys = System::getSingletonPtr();
    if (!sys)
        CEGUI_THROW(RendererException(
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
        CEGUI_THROW(RendererException(
            sys->getImageCodec().getIdentifierString() +
            " failed to load image '" + filename + "'."));
}

//----------------------------------------------------------------------------//
void DirectFBTexture::loadFromMemory(const void* buffer,
                                     const Sizef& buffer_size,
                                     PixelFormat pixel_format)
{
    if (!isPixelFormatSupported(pixel_format))
        CEGUI_THROW(InvalidRequestException(
            "Data was supplied in an unsupported pixel format."));

    cleanupDirectFBTexture();

    DFBSurfaceDescription desc;
    desc.flags = static_cast<DFBSurfaceDescriptionFlags>
        (DSDESC_WIDTH | DSDESC_HEIGHT | DSDESC_PIXELFORMAT);
    desc.width = buffer_size.d_width;
    desc.height = buffer_size.d_height;
    desc.pixelformat = DSPF_ARGB;

    if (d_directfb.CreateSurface(&d_directfb, &desc, &d_texture))
        CEGUI_THROW(RendererException("Failed to create surface."));

    char* dest;
    int pitch;

    if(d_texture->Lock(d_texture, DSLF_WRITE, reinterpret_cast<void**>(&dest), &pitch))
    {
        d_texture->Release(d_texture);
        d_texture = 0;
        CEGUI_THROW(RendererException("Directfb::Lock failed."));
    }

    // Copy data in.
    const size_t pix_sz = (pixel_format == PF_RGB) ? 3 : 4;
    const char* src = static_cast<const char*>(buffer);

    for (int i = 0; i < buffer_size.d_height; ++i)
    {
        for (int j = 0; j < buffer_size.d_width; ++j)
        {
            dest[j * 4 + 0] = src[j * pix_sz + 2];
            dest[j * 4 + 1] = src[j * pix_sz + 1];
            dest[j * 4 + 2] = src[j * pix_sz + 0];
            dest[j * 4 + 3] = (pix_sz == 3) ? 0xFF : src[j * pix_sz + 3];
        }

        dest += pitch;
        src += static_cast<size_t>(buffer_size.d_width) * pix_sz;
    }

    d_texture->Unlock(d_texture);

    // update size and scaling info
    int rw, rh;
    d_texture->GetSize(d_texture, &rw, &rh);
    d_size.d_width = static_cast<float>(rw);
    d_size.d_height = static_cast<float>(rh);
    d_dataSize = buffer_size;
    updateCachedScaleValues();
}

//----------------------------------------------------------------------------//
void DirectFBTexture::blitFromMemory(const void* sourceData, const Rectf& area)
{
    // TODO:
    CEGUI_THROW(RendererException("unimplemented!"));
}

//----------------------------------------------------------------------------//
void DirectFBTexture::blitToMemory(void* targetData)
{
    // TODO:
    CEGUI_THROW(RendererException("unimplemented!"));
}

//----------------------------------------------------------------------------//
DirectFBTexture::DirectFBTexture(IDirectFB& directfb, const String& name) :
    d_directfb(directfb),
    d_texture(0),
    d_size(0, 0),
    d_dataSize(0, 0),
    d_texelScaling(0, 0),
    d_name(name)
{
}

//----------------------------------------------------------------------------//
DirectFBTexture::DirectFBTexture(IDirectFB& directfb, const String& name,
                                 const String& filename,
                                 const String& resourceGroup) :
    d_directfb(directfb),
    d_texture(0),
    d_size(0, 0),
    d_dataSize(0, 0),
    d_texelScaling(0, 0),
    d_name(name)
{
    loadFromFile(filename, resourceGroup);
}

//----------------------------------------------------------------------------//
DirectFBTexture::DirectFBTexture(IDirectFB& directfb,
                                 const String& name, const Sizef& size) :
    d_directfb(directfb),
    d_texture(0),
    d_size(0, 0),
    d_dataSize(0, 0),
    d_texelScaling(0, 0),
    d_name(name)
{
    DFBSurfaceDescription desc;
    desc.flags = static_cast<DFBSurfaceDescriptionFlags>
        (DSDESC_WIDTH | DSDESC_HEIGHT | DSDESC_PIXELFORMAT);
    desc.width = size.d_width;
    desc.height = size.d_height;
    desc.pixelformat = DSPF_ARGB;

    if (d_directfb.CreateSurface(&d_directfb, &desc, &d_texture))
        CEGUI_THROW(RendererException(
            "Failed to create texture of specified size."));

    d_size = d_dataSize = size;
    updateCachedScaleValues();
}

//----------------------------------------------------------------------------//
DirectFBTexture::~DirectFBTexture()
{
    cleanupDirectFBTexture();
}

//----------------------------------------------------------------------------//
void DirectFBTexture::cleanupDirectFBTexture()
{
    if (!d_texture)
        return;

    d_texture->Release(d_texture);
    d_texture = 0;
    d_size = d_dataSize = Sizef(0,0);
    updateCachedScaleValues();
}

//----------------------------------------------------------------------------//
void DirectFBTexture::updateCachedScaleValues()
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
bool DirectFBTexture::isPixelFormatSupported(const PixelFormat fmt) const
{
    return fmt == PF_RGBA || fmt == PF_RGB;
}

//----------------------------------------------------------------------------//

} // End of  CEGUI namespace section
