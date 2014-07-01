/***********************************************************************
    created:    Tue Mar 3 2009
    author:     Paul D Turner (based on original code by Thomas Suter)
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
#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "CEGUI/RendererModules/Irrlicht/Texture.h"
#include "CEGUI/System.h"
#include "CEGUI/Exceptions.h"
#include "CEGUI/ImageCodec.h"
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
        d_size = d_dataSize = Sizef(
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
const String& IrrlichtTexture::getName() const
{
    return d_name;
}

//----------------------------------------------------------------------------//
const Sizef& IrrlichtTexture::getSize() const
{
    return d_size;
}

//----------------------------------------------------------------------------//
const Sizef& IrrlichtTexture::getOriginalDataSize() const
{
    return d_dataSize;
}

//----------------------------------------------------------------------------//
const Vector2f& IrrlichtTexture::getTexelScaling() const
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
        CEGUI_THROW(RendererException(
            "CEGUI::System object has not been created!"));

    // load file to memory via resource provider
    RawDataContainer texFile;
    sys->getResourceProvider()->loadRawDataContainer(filename, texFile,
                                                     resourceGroup);

    Texture* res = sys->getImageCodec().load(texFile, this);

    // unload file data buffer
    sys->getResourceProvider()->unloadRawDataContainer(texFile);

    // throw exception if data was load loaded to texture.
    if (!res)
        CEGUI_THROW(RendererException(
            sys->getImageCodec().getIdentifierString() +
            " failed to load image '" + filename + "'."));
}

//----------------------------------------------------------------------------//
void IrrlichtTexture::loadFromMemory(const void* buffer,
                                     const Sizef& buffer_size,
                                     PixelFormat pixel_format)
{
    using namespace irr;

    if (!isPixelFormatSupported(pixel_format))
        CEGUI_THROW(InvalidRequestException(
            "Data was supplied in an unsupported pixel format."));

    freeIrrlichtTexture();
    createIrrlichtTexture(buffer_size);

    d_size.d_width = static_cast<float>(d_texture->getSize().Width);
    d_size.d_height = static_cast<float>(d_texture->getSize().Height);
    d_dataSize = buffer_size;

    updateCachedScaleValues();

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
void IrrlichtTexture::blitFromMemory(const void* sourceData, const Rectf& area)
{
    if (!d_texture)
        return;

    const size_t pitch = d_texture->getPitch();
    const uint32* src = static_cast<const uint32*>(sourceData);
    uint32* dst = static_cast<uint32*>(d_texture->lock());

    if (!dst)
        CEGUI_THROW(RendererException(
            "[IrrlichtRenderer] ITexture::lock failed."));

    dst += static_cast<size_t>(area.top()) * (pitch / 4) +
        static_cast<size_t>(area.left());

    const Sizef sz(area.getSize());

    for (int j = 0; j < sz.d_height; ++j)
    {
        for (int i = 0; i < sz.d_width; ++i)
        {
            dst[i] = src[i];
        }

        src += static_cast<int>(sz.d_width);
        dst += pitch / 4;
    }
    
    d_texture->unlock();
}

//----------------------------------------------------------------------------//
void IrrlichtTexture::blitToMemory(void* targetData)
{
    if (!d_texture)
        return;

#if (IRRLICHT_VERSION_MAJOR <= 1) && (IRRLICHT_VERSION_MINOR < 8)
    const void* src = d_texture->lock(true);
#else
    const void* src = d_texture->lock(irr::video::ETLM_WRITE_ONLY);
#endif
    if (!src)
        CEGUI_THROW(RendererException(
            "[IrrlichtRenderer] ITexture::lock failed."));

    memcpy(targetData, src,
           static_cast<size_t>(d_size.d_width * d_size.d_height) * 4);

    d_texture->unlock();
}

//----------------------------------------------------------------------------//
IrrlichtTexture::IrrlichtTexture(IrrlichtRenderer& owner,
                                 irr::video::IVideoDriver& driver,
                                 const String& name) :
    d_driver(driver),
    d_texture(0),
    d_size(0, 0),
    d_dataSize(0, 0),
    d_texelScaling(0, 0),
    d_owner(owner),
    d_name(name)
{
}

//----------------------------------------------------------------------------//
IrrlichtTexture::IrrlichtTexture(IrrlichtRenderer& owner,
                                 irr::video::IVideoDriver& driver,
                                 const String& name,
                                 const String& filename,
                                 const String& resourceGroup) :
    d_driver(driver),
    d_texture(0),
    d_owner(owner),
    d_name(name)
{
    loadFromFile(filename, resourceGroup);
}

//----------------------------------------------------------------------------//
IrrlichtTexture::IrrlichtTexture(IrrlichtRenderer& owner,
                                 irr::video::IVideoDriver& driver,
                                 const String& name,
                                 const Sizef& size) :
    d_driver(driver),
    d_dataSize(size),
    d_owner(owner),
    d_name(name)

{
    createIrrlichtTexture(size);

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
void IrrlichtTexture::createIrrlichtTexture(const Sizef& sz)
{
    using namespace irr;

    const Sizef tex_sz(d_owner.getAdjustedTextureSize(sz));

    #if CEGUI_IRR_SDK_VERSION >= 16
        const core::dimension2d<u32> irr_sz(
            static_cast<u32>(tex_sz.d_width),
            static_cast<u32>(tex_sz.d_height));
    #else
        const core::dimension2d<s32> irr_sz(
            static_cast<s32>(tex_sz.d_width),
            static_cast<s32>(tex_sz.d_height));
    #endif

    // save texture creation state
    video::E_TEXTURE_CREATION_FLAG fmtflg;

    if (d_driver.getTextureCreationFlag(video::ETCF_ALWAYS_32_BIT))
        fmtflg = video::ETCF_ALWAYS_32_BIT;
    else if (d_driver.getTextureCreationFlag(video::ETCF_OPTIMIZED_FOR_QUALITY))
        fmtflg = video::ETCF_OPTIMIZED_FOR_QUALITY;
    else if (d_driver.getTextureCreationFlag(video::ETCF_OPTIMIZED_FOR_SPEED))
        fmtflg = video::ETCF_OPTIMIZED_FOR_SPEED;
    else
        fmtflg = video::ETCF_ALWAYS_16_BIT;

    const bool tfmips =
        d_driver.getTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS);
    const bool tfalpha =
        d_driver.getTextureCreationFlag(video::ETCF_NO_ALPHA_CHANNEL);
    const bool tfnpot =
        d_driver.getTextureCreationFlag(video::ETCF_ALLOW_NON_POWER_2);

    // explicitly set all states to what we want
    d_driver.setTextureCreationFlag(video::ETCF_ALWAYS_32_BIT, true);
    d_driver.setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS, false);
    d_driver.setTextureCreationFlag(video::ETCF_NO_ALPHA_CHANNEL, false);
    d_driver.setTextureCreationFlag(video::ETCF_ALLOW_NON_POWER_2, true);

    d_texture = d_driver.addTexture(irr_sz, getUniqueName().c_str(),
                                    irr::video::ECF_A8R8G8B8);

    // restore previous texture creation state
    d_driver.setTextureCreationFlag(fmtflg, true);
    d_driver.setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS, tfmips);
    d_driver.setTextureCreationFlag(video::ETCF_NO_ALPHA_CHANNEL, tfalpha);
    d_driver.setTextureCreationFlag(video::ETCF_ALLOW_NON_POWER_2, tfnpot);

    // we use ARGB all the time for now, so throw if we gut something else!
    if(video::ECF_A8R8G8B8 != d_texture->getColorFormat())
        CEGUI_THROW(RendererException(
            "texture did not have the correct format (ARGB)"));
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
void IrrlichtTexture::setOriginalDataSize(const Sizef& sz)
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
bool IrrlichtTexture::isPixelFormatSupported(const PixelFormat fmt) const
{
    return fmt == PF_RGBA || fmt == PF_RGB;
}

//----------------------------------------------------------------------------//

} // End of  CEGUI namespace section
