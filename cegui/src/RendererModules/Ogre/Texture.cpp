/***********************************************************************
    filename:   CEGUIOgreTexture.cpp
    created:    Tue Feb 17 2009
    author:     Paul D Turner
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
#include "CEGUI/RendererModules/Ogre/Texture.h"
#include "CEGUI/Exceptions.h"
#include "CEGUI/System.h"
#include "CEGUI/RendererModules/Ogre/ImageCodec.h"
#include <OgreTextureManager.h>
#include <OgreHardwarePixelBuffer.h>

// Start of CEGUI namespace section
namespace CEGUI
{
//----------------------------------------------------------------------------//
// Internal function that reverses all bytes in a buffer
void _byteSwap(unsigned char* b, int n)
{
    register int i = 0;
    register int j = n-1;
    while (i < j)
        std::swap(b[i++], b[j--]);
}
#define byteSwap(x) _byteSwap((unsigned char*) &x,sizeof(x))

//----------------------------------------------------------------------------//
uint32 OgreTexture::d_textureNumber = 0;

//----------------------------------------------------------------------------//
const String& OgreTexture::getName() const
{
    return d_name;
}

//----------------------------------------------------------------------------//
const Sizef& OgreTexture::getSize() const
{
    return d_size;
}

//----------------------------------------------------------------------------//
const Sizef& OgreTexture::getOriginalDataSize() const
{
    return d_dataSize;
}

//----------------------------------------------------------------------------//
const Vector2f& OgreTexture::getTexelScaling() const
{
    return d_texelScaling;
}

//----------------------------------------------------------------------------//
void OgreTexture::loadFromFile(const String& filename,
                               const String& resourceGroup)
{
    // get and check existence of CEGUI::System object
    System* sys = System::getSingletonPtr();
    if (!sys)
        CEGUI_THROW(RendererException("OgreTexture::loadFromFile: "
            "CEGUI::System object has not been created!"));

    // load file to memory via resource provider
    RawDataContainer texFile;
    sys->getResourceProvider()->loadRawDataContainer(filename, texFile,
                                                     resourceGroup);

    ImageCodec& ic(sys->getImageCodec());

    // if we're using the integrated Ogre codec, set the file-type hint string
    if (ic.getIdentifierString().substr(0, 14)  == "OgreImageCodec")
    {
        String type;
        String::size_type i = filename.find_last_of(".");
        if (i != String::npos && filename.length() - i > 1)
            type = filename.substr(i+1);
        static_cast<OgreImageCodec&>(ic).setImageFileDataType(type);
    }

    Texture* res = sys->getImageCodec().load(texFile, this);

    // unload file data buffer
    sys->getResourceProvider()->unloadRawDataContainer(texFile);

    // throw exception if data was load loaded to texture.
    if (!res)
        CEGUI_THROW(RendererException("OgreTexture::loadFromFile: " +
            sys->getImageCodec().getIdentifierString() +
            " failed to load image '" + filename + "'."));
}

//----------------------------------------------------------------------------//
void OgreTexture::loadFromMemory(const void* buffer, const Sizef& buffer_size,
                                 PixelFormat pixel_format)
{
    using namespace Ogre;

    if (!isPixelFormatSupported(pixel_format))
        CEGUI_THROW(InvalidRequestException("OgreTexture::loadFromMemory: Data "
            "was supplied in an unsupported pixel format."));

    // get rid of old texture
    freeOgreTexture();

    // wrap input buffer with an Ogre data stream
    const size_t pixel_size = pixel_format == PF_RGBA ? 4 : 3;
    const size_t byte_size = buffer_size.d_width * buffer_size.d_height *
                                pixel_size;

#if OGRE_ENDIAN == OGRE_ENDIAN_BIG
    // FIXME: I think this leaks memory, though need to check!
    unsigned char* swapped_buffer = new unsigned char[byte_size];
    memcpy(swapped_buffer, buffer, byte_size);

    for (size_t i = 0; i < byte_size; i += pixel_size)
        _byteSwap(&swapped_buffer[i], pixel_size);

    DataStreamPtr odc(new MemoryDataStream(static_cast<void*>(swapped_buffer),
                                           byte_size, false));
#else
    DataStreamPtr odc(new MemoryDataStream(const_cast<void*>(buffer),
                                           byte_size, false));
#endif

    // get pixel type for the target texture.
    Ogre::PixelFormat target_fmt =
        (pixel_format == PF_RGBA) ? Ogre::PF_A8B8G8R8 : Ogre::PF_B8G8R8;

    // try to create a Ogre::Texture from the input data
    d_texture = TextureManager::getSingleton().loadRawData(
        getUniqueName(), "General", odc,
        buffer_size.d_width, buffer_size.d_height,
        target_fmt, TEX_TYPE_2D, 0, 1.0f);

    // throw exception if no texture was able to be created
    if (d_texture.isNull())
        CEGUI_THROW(RendererException("OgreTexture::loadFromMemory: Failed to "
            "create Texture object from memory."));

    d_size.d_width = d_texture->getWidth();
    d_size.d_height = d_texture->getHeight();
    d_dataSize = buffer_size;
    updateCachedScaleValues();
}

//----------------------------------------------------------------------------//
void OgreTexture::blitFromMemory(void* sourceData, const Rectf& area)
{
    if (d_texture.isNull()) // TODO: exception?
        return;

    Ogre::PixelBox pb(area.getWidth(), area.getHeight(),
                      1, Ogre::PF_A8R8G8B8, sourceData);
    Ogre::Image::Box box(area.left(), area.top(), area.right(), area.bottom());
    d_texture->getBuffer()->blitFromMemory(pb, box);
}

//----------------------------------------------------------------------------//
void OgreTexture::blitToMemory(void* targetData)
{
    if (d_texture.isNull()) // TODO: exception?
        return;

    Ogre::PixelBox pb(d_size.d_width, d_size.d_height,
                      1, Ogre::PF_A8R8G8B8, targetData);
    d_texture->getBuffer()->blitToMemory(pb);
}

//----------------------------------------------------------------------------//
OgreTexture::OgreTexture(const String& name) :
    d_isLinked(false),
    d_size(0, 0),
    d_dataSize(0, 0),
    d_texelScaling(0, 0),
    d_name(name)
{
}

//----------------------------------------------------------------------------//
OgreTexture::OgreTexture(const String& name, const String& filename,
                         const String& resourceGroup) :
    d_isLinked(false),
    d_size(0, 0),
    d_dataSize(0, 0),
    d_texelScaling(0, 0),
    d_name(name)
{
    loadFromFile(filename, resourceGroup);
}

//----------------------------------------------------------------------------//
OgreTexture::OgreTexture(const String& name, const Sizef& sz) :
    d_isLinked(false),
    d_size(0, 0),
    d_dataSize(0, 0),
    d_texelScaling(0, 0),
    d_name(name)
{
    using namespace Ogre;

    // try to create a Ogre::Texture with given dimensions
    d_texture = TextureManager::getSingleton().createManual(
                                   getUniqueName(), "General", TEX_TYPE_2D,
                                   sz.d_width, sz.d_height, 0,
                                   Ogre::PF_A8B8G8R8);
    
    // throw exception if no texture was able to be created
    if (d_texture.isNull())
        CEGUI_THROW(RendererException("OgreTexture: Failed to create Texture "
            "object with spcecified size."));
    
    d_size.d_width = d_texture->getWidth();
    d_size.d_height = d_texture->getHeight();
    d_dataSize = sz;
    updateCachedScaleValues();
}

//----------------------------------------------------------------------------//
OgreTexture::OgreTexture(const String& name, Ogre::TexturePtr& tex,
                         bool take_ownership) :
    d_isLinked(false),
    d_size(0, 0),
    d_dataSize(0, 0),
    d_texelScaling(0, 0),
    d_name(name)
{
    setOgreTexture(tex, take_ownership);
}

//----------------------------------------------------------------------------//
OgreTexture::~OgreTexture()
{
    freeOgreTexture();
}

//----------------------------------------------------------------------------//
void OgreTexture::freeOgreTexture()
{
    if (!d_texture.isNull() && !d_isLinked)
        Ogre::TextureManager::getSingleton().remove(d_texture->getHandle());

    d_texture.setNull();
}

//----------------------------------------------------------------------------//
Ogre::String OgreTexture::getUniqueName()
{
    Ogre::StringUtil::StrStreamType strstream;
    strstream << "_cegui_ogre_" << d_textureNumber++;

    return strstream.str();
}

//----------------------------------------------------------------------------//
void OgreTexture::updateCachedScaleValues()
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
void OgreTexture::setOgreTexture(Ogre::TexturePtr texture, bool take_ownership)
{
    freeOgreTexture();

    d_texture = texture;
    d_isLinked = !take_ownership;

    if (!d_texture.isNull())
    {
        d_size.d_width = d_texture->getWidth();
        d_size.d_height= d_texture->getHeight();
        d_dataSize = d_size;
    }
    else
        d_size = d_dataSize = Sizef(0, 0);

    updateCachedScaleValues();
}

//----------------------------------------------------------------------------//
Ogre::TexturePtr OgreTexture::getOgreTexture() const
{
    return d_texture;
}

//----------------------------------------------------------------------------//
bool OgreTexture::isPixelFormatSupported(const PixelFormat fmt) const
{
    return fmt == PF_RGBA || fmt == PF_RGB;
}

//----------------------------------------------------------------------------//

} // End of  CEGUI namespace section
