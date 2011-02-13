/***********************************************************************
    filename:   CEGUINullTexture.cpp
    created:    Sat Jan 16 2010
    author:     Eugene Marcotte
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2010 Paul D Turner & The CEGUI Development Team
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
#include "CEGUINullTexture.h"
#include "CEGUIExceptions.h"
#include "CEGUIImageCodec.h"
#include "CEGUISystem.h"

// Start of CEGUI namespace section
namespace CEGUI
{
//----------------------------------------------------------------------------//
uint32 NullTexture::d_textureNumber = 0;

//----------------------------------------------------------------------------//
const Size& NullTexture::getSize() const
{
    return d_size;
}

//----------------------------------------------------------------------------//
const Size& NullTexture::getOriginalDataSize() const
{
    return d_dataSize;
}

//----------------------------------------------------------------------------//
const Vector2<>& NullTexture::getTexelScaling() const
{
    return d_texelScaling;
}

//----------------------------------------------------------------------------//
void NullTexture::loadFromFile(const String& filename,
                               const String& resourceGroup)
{
    // get and check existence of CEGUI::System object
    System* sys = System::getSingletonPtr();
    if (!sys)
        CEGUI_THROW(RendererException("NullTexture::loadFromFile: "
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
        CEGUI_THROW(RendererException("NullTexture::loadFromFile: " +
            sys->getImageCodec().getIdentifierString() +
            " failed to load image '" + filename + "'."));
}

//----------------------------------------------------------------------------//
void NullTexture::loadFromMemory(const void* buffer, const Size& buffer_size,
                                 PixelFormat pixel_format)
{
    const size_t pixel_size = pixel_format == PF_RGBA ? 4 : 3;
    const size_t byte_size = buffer_size.d_width * buffer_size.d_height *
                                pixel_size;

    d_size.d_width = buffer_size.d_width;
    d_size.d_height = buffer_size.d_height;
    d_dataSize = buffer_size;
}

//----------------------------------------------------------------------------//
void NullTexture::saveToMemory(void* /*buffer*/)
{
    // TODO: If we make loadFromMemory save a copy of the data, we could
    // implement this function too :)
}

//----------------------------------------------------------------------------//
void NullTexture::blitFromMemory(void* /*sourceData*/, const Rect& /*area*/)
{
    // do nothing
}

//----------------------------------------------------------------------------//
void NullTexture::blitToMemory(void* /*targetData*/)
{
    // do nothing
}

//----------------------------------------------------------------------------//
NullTexture::NullTexture() :
    d_size(0, 0),
    d_dataSize(0, 0),
    d_texelScaling(0, 0)
{
}

//----------------------------------------------------------------------------//
NullTexture::NullTexture(const String& filename, const String& resourceGroup) :
    d_size(0, 0),
    d_dataSize(0, 0),
    d_texelScaling(0, 0)
{
    loadFromFile(filename, resourceGroup);
}

//----------------------------------------------------------------------------//
NullTexture::NullTexture(const Size& sz) :
    d_size(0, 0),
    d_dataSize(0, 0),
    d_texelScaling(0, 0)
{
    d_size.d_width = sz.d_width;
    d_size.d_height = sz.d_height;
    d_dataSize = sz;
}

//----------------------------------------------------------------------------//
NullTexture::~NullTexture()
{
}

} // End of  CEGUI namespace section
