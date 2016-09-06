/***********************************************************************
    created:    Wed Mar 25 2009
    author:     Paul D Turner <paul@cegui.org.uk>
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
#include "CEGUI/RendererModules/Ogre/ImageCodec.h"
#include "CEGUI/RendererModules/Ogre/Texture.h"
#include "CEGUI/Exceptions.h"
#include <Ogre.h>

// Start of CEGUI namespace section
namespace CEGUI
{
//----------------------------------------------------------------------------//
OgreImageCodec::OgreImageCodec() :
    ImageCodec("OgreImageCodec - Integrated ImageCodec using the Ogre engine.")
{
}

//----------------------------------------------------------------------------//
void OgreImageCodec::setImageFileDataType(const String& type)
{
    d_dataTypeID = type;
}

//----------------------------------------------------------------------------//
const String& OgreImageCodec::getImageFileDataType() const
{
    return d_dataTypeID;
}

//----------------------------------------------------------------------------//
Texture* OgreImageCodec::load(const RawDataContainer& data, Texture* result)
{
    using namespace Ogre;

    // wrap the buffer of the RawDataContainer with an Ogre::MemoryDataStream.
    DataStreamPtr stream(
        OGRE_NEW MemoryDataStream(
            const_cast<void*>(static_cast<const void*>(data.getDataPtr())),
            data.getSize(), false));

    // load the image
    Ogre::Image image;
    image.load(stream, d_dataTypeID.c_str());

    const PixelFormat ogre_pf = image.getFormat();
    const Texture::PixelFormat cegui_pf =
        OgreTexture::fromOgrePixelFormat(ogre_pf);

    // discover the pixel format and number of pixel components
    int components;
    bool rbswap;
    switch (ogre_pf)
    {
        case PF_R8G8B8:
            rbswap = true;
            components = 3;
            break;

        case PF_A8R8G8B8:
            rbswap = true;
            components = 4;
            break;

        default:
            rbswap = false;
            break;
    }

    // do the old switcharoo on R and B if needed
    if (rbswap)
    {
        uchar* dat = image.getData();
        for (uint j = 0; j < image.getHeight(); ++j)
        {
            for (uint i = 0; i < image.getWidth(); ++i)
            {
                uchar tmp = dat[i * components + 0];
                dat[i * components + 0] = dat[i * components + 2];
                dat[i * components + 2] = tmp;
            }

            dat += image.getRowSpan();
        }
    }

    // load the resulting image into the texture
    result->loadFromMemory(image.getData(),
                           Sizef(image.getWidth(),
                                  image.getHeight()),
                           cegui_pf);

    return result;
}

//----------------------------------------------------------------------------//

} // End of  CEGUI namespace section
