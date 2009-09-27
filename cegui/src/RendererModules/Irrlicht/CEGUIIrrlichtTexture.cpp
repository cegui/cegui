/***********************************************************************
    filename:   CEGUIIrrlichtTexture.cpp
    created:    Tue Mar 3 2009
    author:     Paul D Turner (based on original code by Thomas Suter)
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
#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "CEGUIIrrlichtTexture.h"
#include "CEGUISystem.h"
#include "CEGUIExceptions.h"
#include "CEGUIImageCodec.h"
#include <irrlicht.h>

// Start of CEGUI namespace section
namespace CEGUI
{
//----------------------------------------------------------------------------//
uint32 IrrlichtTexture::d_textureNumber = 0;

//----------------------------------------------------------------------------//
void IrrlichtTexture::setIrrlichtTexture(irr::video::ITexture* tex)
{
    d_texture = tex;

    if (d_texture)
    {
        d_size = d_dataSize = Size(
                static_cast<float>(d_texture->getSize().Width),
                static_cast<float>(d_texture->getSize().Height));

        updateCachedScaleValues();
    }
}

//----------------------------------------------------------------------------//
irr::video::ITexture* IrrlichtTexture::getIrrlichtTexture() const
{
    return d_texture;
}

//----------------------------------------------------------------------------//
const Size& IrrlichtTexture::getSize() const
{
    return d_size;
}

//----------------------------------------------------------------------------//
const Size& IrrlichtTexture::getOriginalDataSize() const
{
    return d_dataSize;
}

//----------------------------------------------------------------------------//
const Vector2& IrrlichtTexture::getTexelScaling() const
{
    return d_texelScaling;
}

//----------------------------------------------------------------------------//
void IrrlichtTexture::loadFromFile(const String& filename,
                                   const String& resourceGroup)
{
    // get and check existence of CEGUI::System object
    System* sys = System::getSingletonPtr();
    if (!sys)
        throw RendererException("IrrlichtTexture::loadFromFile: "
                                "CEGUI::System object has not been created!");

    // load file to memory via resource provider
    RawDataContainer texFile;
    sys->getResourceProvider()->loadRawDataContainer(filename, texFile,
                                                     resourceGroup);

    Texture* res = sys->getImageCodec().load(texFile, this);

    // unload file data buffer
    sys->getResourceProvider()->unloadRawDataContainer(texFile);

    // throw exception if data was load loaded to texture.
    if (!res)
        throw RendererException("IrrlichtTexture::loadFromFile: " +
                                sys->getImageCodec().getIdentifierString()+
                                " failed to load image '" + filename + "'.");
}

//----------------------------------------------------------------------------//
void IrrlichtTexture::loadFromMemory(const void* buffer,
                                     const Size& buffer_size,
                                     PixelFormat pixel_format)
{
    using namespace irr;

    freeIrrlichtTexture();

    const Size tex_sz(d_owner.getAdjustedTextureSize(buffer_size));

    #if CEGUI_IRR_SDK_VERSION >= 16
        const irr::core::dimension2d<irr::u32> irr_sz(
            static_cast<irr::u32>(tex_sz.d_width),
            static_cast<irr::u32>(tex_sz.d_height));
    #else
        const irr::core::dimension2d<irr::s32> irr_sz(
            static_cast<irr::s32>(tex_sz.d_width),
            static_cast<irr::s32>(tex_sz.d_height));
    #endif

    d_texture = d_driver.addTexture(irr_sz, getUniqueName().c_str());

    d_size.d_width = static_cast<float>(d_texture->getSize().Width);
    d_size.d_height = static_cast<float>(d_texture->getSize().Height);
    d_dataSize = buffer_size;

    updateCachedScaleValues();

    // we now use ARGB all the time here, so throw if we get something else!
    if(video::ECF_A8R8G8B8 != d_texture->getColorFormat())
        throw RendererException("IrrlichtTexture::loadFromMemory: texture did "
                                "not have the correct format (ARGB)");

    const size_t pix_sz = (pixel_format == PF_RGB) ? 3 : 4;
    const char* src = static_cast<const char*>(buffer);
    char* dest = static_cast<char*>(d_texture->lock());

    // copy data into texture, swapping red & blue and creating alpha channel
    // values as needed.
    for (int j = 0; j < buffer_size.d_height; ++j)
    {
        for (int i = 0; i < buffer_size.d_width; ++i)
        {
            dest[i * 4 + 0] = src[i * pix_sz + 2];
            dest[i * 4 + 1] = src[i * pix_sz + 1];
            dest[i * 4 + 2] = src[i * pix_sz + 0];
            dest[i * 4 + 3] = (pix_sz == 3) ? 0xFF : src[i * pix_sz + 3];
        }

        src += static_cast<int>(buffer_size.d_width * pix_sz);
        dest += static_cast<int>(d_size.d_width * 4);
    }

    d_texture->unlock();
}

//----------------------------------------------------------------------------//
void IrrlichtTexture::saveToMemory(void* buffer)
{
    if (!d_texture)
        return;

    const size_t sz = static_cast<size_t>(d_size.d_width * d_size.d_height) * 4;
    memcpy(buffer, d_texture->lock(), sz);
    d_texture->unlock();
}

//----------------------------------------------------------------------------//
IrrlichtTexture::IrrlichtTexture(IrrlichtRenderer& owner,
                                 irr::video::IVideoDriver& driver) :
    d_driver(driver),
    d_texture(0),
    d_size(0, 0),
    d_dataSize(0, 0),
    d_texelScaling(0, 0),
    d_owner(owner)
{
}

//----------------------------------------------------------------------------//
IrrlichtTexture::IrrlichtTexture(IrrlichtRenderer& owner,
                                 irr::video::IVideoDriver& driver,
                                 const String& filename,
                                 const String& resourceGroup) :
    d_driver(driver),
    d_texture(0),
    d_owner(owner)

{
    loadFromFile(filename, resourceGroup);
}

//----------------------------------------------------------------------------//
IrrlichtTexture::IrrlichtTexture(IrrlichtRenderer& owner,
                                 irr::video::IVideoDriver& driver,
                                 const Size& size) :
    d_driver(driver),
    d_dataSize(size),
    d_owner(owner)

{
    const Size tex_sz(d_owner.getAdjustedTextureSize(size));

    #if CEGUI_IRR_SDK_VERSION >= 16
        const irr::core::dimension2d<irr::u32> irr_sz(
            static_cast<irr::u32>(tex_sz.d_width),
            static_cast<irr::u32>(tex_sz.d_height));
    #else
        const irr::core::dimension2d<irr::s32> irr_sz(
            static_cast<irr::s32>(tex_sz.d_width),
            static_cast<irr::s32>(tex_sz.d_height));
    #endif

    d_texture = d_driver.addTexture(irr_sz, getUniqueName().c_str(),
                                    irr::video::ECF_A8R8G8B8);

    d_size.d_width = static_cast<float>(d_texture->getSize().Width);
    d_size.d_height = static_cast<float>(d_texture->getSize().Height);

    updateCachedScaleValues();
}

//----------------------------------------------------------------------------//
IrrlichtTexture::~IrrlichtTexture()
{
    freeIrrlichtTexture();
}

//----------------------------------------------------------------------------//
void IrrlichtTexture::freeIrrlichtTexture()
{
    if (!d_texture)
        return;

    d_driver.removeTexture(d_texture);
    d_texture = 0;
}

//----------------------------------------------------------------------------//
std::string IrrlichtTexture::getUniqueName()
{
    char tmp[32];
    sprintf(tmp, "irr_tex_%d", d_textureNumber++);

    return std::string(tmp);
}

//----------------------------------------------------------------------------//
void IrrlichtTexture::setOriginalDataSize(const Size& sz)
{
    d_dataSize = sz;
    updateCachedScaleValues();
}

//----------------------------------------------------------------------------//
void IrrlichtTexture::updateCachedScaleValues()
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

} // End of  CEGUI namespace section
