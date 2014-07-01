/***********************************************************************
    created:    Tue Oct 20 2009
    author:     Andrew Shevchenko
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
#include "CEGUI/Exceptions.h"
#include "CEGUI/Logger.h"
#include "CEGUI/Size.h"

#include "PVRTGlobal.h"
#include "PVRTTexture.h"
#include "PVRTDecompress.h"

#include "CEGUI/ImageCodecModules/PVR/ImageCodec.h"

// Start of CEGUI namespace section
namespace CEGUI
{
//----------------------------------------------------------------------------//
PVRImageCodec::PVRImageCodec() : ImageCodec("PVRImageCodec")
{
    d_supportedFormat = "pvr";
}

//----------------------------------------------------------------------------//
PVRImageCodec::~PVRImageCodec()
{
}

//----------------------------------------------------------------------------//
Texture* PVRImageCodec::load(const RawDataContainer& data, Texture* result)
{
    const PVR_Texture_Header* pvr_header =
        reinterpret_cast<const PVR_Texture_Header*>(data.getDataPtr());

    // convert header to big endian if required
    PVR_Texture_Header converted_pvr_header;
    if (!PVRTIsLittleEndian())
    {
        converted_pvr_header = *pvr_header;

        PVRTuint32* data = reinterpret_cast<PVRTuint32*>(&converted_pvr_header);
        for (uint i = 0; i < sizeof(PVR_Texture_Header) / sizeof(PVRTuint32); i++)
            PVRTByteSwap(reinterpret_cast<PVRTuint8*>(data++), sizeof(PVRTuint32));

        pvr_header = &converted_pvr_header;
    }

    // perform checks for old PVR psPVRHeader
    if (pvr_header->dwHeaderSize != sizeof(PVR_Texture_Header))
    {
        // Unknow or old format
        Logger::getSingletonPtr()->logEvent(
            "PVRImageCodec::load - unknown or old texture format.  "
            "Use PVRTexTool to save in appropriate format.", Errors);
        return 0;
    }

    if (pvr_header->dwpfFlags & PVRTEX_CUBEMAP)
    {
        //Cube maps are not supported
        Logger::getSingletonPtr()->logEvent(
            "PVRImageCodec::load - cubemap textures unsupported.", Errors);
        return 0;
    }

    /* Only accept untwiddled data UNLESS texture format is PVRTC */
    if (((pvr_header->dwpfFlags & PVRTEX_TWIDDLE) == PVRTEX_TWIDDLE) &&
        ((pvr_header->dwpfFlags & PVRTEX_PIXELTYPE) != OGL_PVRTC2) &&
        ((pvr_header->dwpfFlags & PVRTEX_PIXELTYPE) != OGL_PVRTC4))
    {
        // We need to load untwiddled textures -- hw will twiddle for us.
        Logger::getSingletonPtr()->logEvent(
            "PVRImageCodec::load - Texture should be untwiddled", Errors);
        return 0;
    }

    CEGUI::Texture::PixelFormat cefmt;
    bool is_compressed_format = false;

    switch (pvr_header->dwpfFlags & PVRTEX_PIXELTYPE)
    {
    case OGL_RGBA_4444:
        cefmt = CEGUI::Texture::PF_RGBA_4444;
        break;

    case OGL_RGBA_5551:
        Logger::getSingletonPtr()->logEvent("PVRImageCodec::load - "
            "pixel format RGBA_5551 not supported.", Errors);
        return 0;

    case OGL_RGB_565:
        cefmt = CEGUI::Texture::PF_RGB_565;
        break;

    case OGL_RGB_555:
        Logger::getSingletonPtr()->logEvent("PVRImageCodec::load - "
            "pixel format RGB_555 not supported.", Errors);
        return 0;

    case OGL_I_8:
        Logger::getSingletonPtr()->logEvent("PVRImageCodec::load - "
            "pixel format I_8 not supported.", Errors);
        return 0;

    case OGL_AI_88:
        Logger::getSingletonPtr()->logEvent("PVRImageCodec::load - "
            "pixel format AI_88 not supported.", Errors);
        return 0;

    case OGL_BGRA_8888:
        Logger::getSingletonPtr()->logEvent("PVRImageCodec::load - "
            "pixel format BGRA8888 not supported.", Errors);
        return 0;

    case OGL_RGBA_8888:
        cefmt = CEGUI::Texture::PF_RGBA;
        break;

    case OGL_RGB_888:
        cefmt = CEGUI::Texture::PF_RGB;
        break;

    case MGLPT_PVRTC2:
    case OGL_PVRTC2:
        cefmt = CEGUI::Texture::PF_PVRTC2;
        is_compressed_format = true;
        break;

    case MGLPT_PVRTC4:
    case OGL_PVRTC4:
        cefmt = CEGUI::Texture::PF_PVRTC4;
        is_compressed_format = true;
        break;

    default:
        Logger::getSingletonPtr()->logEvent("PVRImageCodec::load - "
            "wrong pvr pixel format.", Errors);
        return 0;
    }

    const void* texture_data =
        reinterpret_cast<const void*>(data.getDataPtr() + pvr_header->dwHeaderSize);
    const uint size_x = pvr_header->dwWidth;
    const uint size_y = pvr_header->dwHeight;

    if (is_compressed_format)
    {
        if (result->isPixelFormatSupported(cefmt))
        {
            result->loadFromMemory(texture_data,
                                   Sizef(static_cast<float>(size_x),
                                         static_cast<float>(size_y)),
                                   cefmt);
        }
        else
        {
            // Convert PVRTC to 32-bit RGBA
            PVRTuint8* decompressed_texture = new PVRTuint8[size_x * size_y * 4];

            int bit_mode = (pvr_header->dwpfFlags & PVRTEX_PIXELTYPE) == OGL_PVRTC2;
            PVRTDecompressPVRTC(texture_data, bit_mode, size_x, size_y,
                                decompressed_texture);

            cefmt = CEGUI::Texture::PF_RGBA; // Redefine to uncompressed format
            result->loadFromMemory(decompressed_texture,
                                   Sizef(static_cast<float>(size_x),
                                         static_cast<float>(size_y)),
                                   cefmt);
            delete[] decompressed_texture;
        }
    }
    else
    {
        // Perform raw data load from textureData
        result->loadFromMemory(texture_data,
                               Sizef(static_cast<float>(size_x),
                                     static_cast<float>(size_y)),
                               cefmt);
    }


    return result;
}

//----------------------------------------------------------------------------//

} // End of CEGUI namespace section
