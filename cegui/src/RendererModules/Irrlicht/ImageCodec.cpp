/***********************************************************************
    created:    Tue Aug 18 2009
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
#include "CEGUI/RendererModules/Irrlicht/ImageCodec.h"
#include "CEGUI/Exceptions.h"
#include "CEGUI/RendererModules/Irrlicht/MemoryFile.h"
#include "CEGUI/Size.h"

// Start of CEGUI namespace section
namespace CEGUI
{
//----------------------------------------------------------------------------//
IrrlichtImageCodec::IrrlichtImageCodec(irr::video::IVideoDriver& driver) :
    ImageCodec("IrrlichtImageCodec - "
               "Integrated ImageCodec using the Irrlicht engine."),
    d_driver(driver)
{
}

//----------------------------------------------------------------------------//
Texture* IrrlichtImageCodec::load(const RawDataContainer& data, Texture* result)
{
    using namespace irr;

    // wrap data in a IrrlichtMemoryFile so Irrlicht can try to 'load' it.
    IrrlichtMemoryFile imf("IrrlichtImageCodec::load",
                           data.getDataPtr(),
                           data.getSize());

    // get irrlicht to parse the file data
    video::IImage* image = d_driver.createImageFromFile(&imf);

    if (!image)
        CEGUI_THROW(FileIOException(
            "Irrlicht failed to create irr::video::IImage from file data."));

    // get format of image
    Texture::PixelFormat format;
    int components;
    switch (image->getColorFormat())
    {
        case video::ECF_A8R8G8B8:
            format = Texture::PF_RGBA;
            components = 4;
            break;

        case video::ECF_R8G8B8:
            format = Texture::PF_RGB;
            components = 3;
            break;

        default:
            image->drop();
            CEGUI_THROW(FileIOException(
                "File data was of an unsupported format."));
    }

    const core::dimension2d<s32> sz(image->getDimension());
    uchar* dat = static_cast<uchar*>(image->lock());
    const uchar* const image_data = dat;

    // ONLY for RGBA, switch R and B components
    // (we should probably check the R and B masks and decide based on those)
    if (format == Texture::PF_RGBA)
    {
        for (s32 j = 0; j < sz.Height; ++j)
        {
            for (s32 i = 0; i < sz.Width; ++i)
            {
                const uchar tmp = dat[i * components + 0];
                dat[i * components + 0] = dat[i * components + 2];
                dat[i * components + 2] = tmp;
            }

            dat += image->getPitch();
        }
    }

    // load the resulting image into the texture
    CEGUI_TRY
    {
        result->loadFromMemory(image_data, Sizef(static_cast<float>(sz.Width),
                                                  static_cast<float>(sz.Height)),
                               format);
    }
    CEGUI_CATCH (...)
    {
        // cleanup when there's an exception
        image->unlock();
        image->drop();
        CEGUI_RETHROW;
    }

    // cleanup.
    image->unlock();
    image->drop();

    return result;
}

//----------------------------------------------------------------------------//

} // End of  CEGUI namespace section
