/***********************************************************************
    created:    Sat Jan 16 2010
    author:     Eugene Marcotte
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
#include "CEGUI/RendererModules/Null/Texture.h"
#include "CEGUI/Exceptions.h"
#include "CEGUI/ImageCodec.h"
#include "CEGUI/System.h"

// Start of CEGUI namespace section
namespace CEGUI
{
//----------------------------------------------------------------------------//
uint32 NullTexture::d_textureNumber = 0;

//----------------------------------------------------------------------------//
const String& NullTexture::getName() const
{
    return d_name;
}

//----------------------------------------------------------------------------//
const Sizef& NullTexture::getSize() const
{
    return d_size;
}

//----------------------------------------------------------------------------//
const Sizef& NullTexture::getOriginalDataSize() const
{
    return d_dataSize;
}

//----------------------------------------------------------------------------//
const Vector2f& NullTexture::getTexelScaling() const
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
void NullTexture::loadFromMemory(const void* /*buffer*/,
                                 const Sizef& buffer_size,
                                 PixelFormat)
{
    d_size = d_dataSize = buffer_size;
}

//----------------------------------------------------------------------------//
void NullTexture::blitFromMemory(const void* /*sourceData*/, const Rectf& /*area*/)
{
    // do nothing
}

//----------------------------------------------------------------------------//
void NullTexture::blitToMemory(void* /*targetData*/)
{
    // do nothing
}

//----------------------------------------------------------------------------//
NullTexture::NullTexture(const String& name) :
    d_size(0, 0),
    d_dataSize(0, 0),
    d_texelScaling(0, 0),
    d_name(name)
{
}

//----------------------------------------------------------------------------//
NullTexture::NullTexture(const String& name, const String& filename,
                         const String& resourceGroup) :
    d_size(0, 0),
    d_dataSize(0, 0),
    d_texelScaling(0, 0),
    d_name(name)
{
    loadFromFile(filename, resourceGroup);
}

//----------------------------------------------------------------------------//
NullTexture::NullTexture(const String& name, const Sizef& sz) :
    d_size(0, 0),
    d_dataSize(0, 0),
    d_texelScaling(0, 0),
    d_name(name)
{
    d_size.d_width = sz.d_width;
    d_size.d_height = sz.d_height;
    d_dataSize = sz;
}

//----------------------------------------------------------------------------//
NullTexture::~NullTexture()
{
}

//----------------------------------------------------------------------------//
bool NullTexture::isPixelFormatSupported(const PixelFormat) const
{
    return true;
}

//----------------------------------------------------------------------------//

} // End of  CEGUI namespace section
