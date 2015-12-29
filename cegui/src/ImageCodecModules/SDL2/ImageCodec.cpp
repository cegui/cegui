/************************************************************************
	created:	04/07/2014
	author:		Luca Ebach <bitbucket@lucebac.net>
                        with code by John Norman
	
	purpose:	This codec provides SDL2 based image loading 
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2015 Paul D Turner & The CEGUI Development Team
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

#include "CEGUI/ImageCodecModules/SDL2/ImageCodec.h"

#include "CEGUI/DataContainer.h"
#include "CEGUI/Exceptions.h"
#include "CEGUI/Texture.h"
#include "CEGUI/Singleton.h"
#include "CEGUI/Logger.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <cstring>

namespace CEGUI
{

SDL2ImageCodec::SDL2ImageCodec()
    : ImageCodec("CEGUI::SDL2ImageCodec - Official SDL2 based image codec")
{    
    IMG_Init(~0);
    
    d_supportedFormat = "BMP GIF JPEG LBM PCX PNG PNM TGA TIFF WEBP XCF XPM XV";
}

SDL2ImageCodec::~SDL2ImageCodec()
{
    IMG_Quit();
}

Texture* SDL2ImageCodec::load(const RawDataContainer& data, Texture* result)
{
    SDL_RWops* ops = SDL_RWFromConstMem(static_cast<const void*>(data.getDataPtr()), data.getSize());
    
    if (ops)
    {
        Texture::PixelFormat pixel_fmt;
                
        SDL_Surface* img = IMG_Load_RW(ops, 0);
        
        if (!img)
            img = IMG_LoadTGA_RW(ops);
        
        if (!img)
        {
            Logger::getSingletonPtr()->logEvent("Cannot load image! SDL2_image returned an error in IMG_Load_RW", Errors);
            return 0;
        }
            
        switch (img->format->BitsPerPixel)
        {
            case 24:
                pixel_fmt = Texture::PF_RGB;
                break;
                
            case 32:
                pixel_fmt = Texture::PF_RGBA;
                break;
                
            default:
                Logger::getSingletonPtr()->logEvent("Unsupported pixel format in SDL2 image codec", Errors);
                return 0;
        }
        
        Sizef size;
        size.d_width  = static_cast<float>(img->w);
        size.d_height = static_cast<float>(img->h);
        
        SDL_LockSurface(img);
        result->loadFromMemory(img->pixels, size, pixel_fmt);
        SDL_UnlockSurface(img);
        
        SDL_FreeRW(ops);
        SDL_FreeSurface(img);
    }
    else
    {
        CEGUI_THROW(FileIOException("Could not load image from memory."));
    }
    
    return result;
}

}