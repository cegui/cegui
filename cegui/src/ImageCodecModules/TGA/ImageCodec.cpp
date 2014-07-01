/***********************************************************************
	created:	03/06/2006
	author:		Olivier Delannoy 
	
	purpose:	This codec provide TGA image loading, it's the 
                        default codec 
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2006 Paul D Turner & The CEGUI Development Team
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
#include "CEGUI/ImageCodecModules/TGA/ImageCodec.h"
#include "CEGUI/Logger.h"
#include "CEGUI/Size.h"
#	define TGA_RGB		 2		// This tells us it's a normal RGB (really BGR) file
#	define TGA_A		 3		// This tells us it's a ALPHA file
#	define TGA_RLE		10		// This tells us that the targa is Run-Length Encoded (RLE)
namespace CEGUI 
{

TGAImageCodec::TGAImageCodec()
    : ImageCodec("TGAImageCodec - Official TGA image codec")
{
    d_supportedFormat = "tga";
}
TGAImageCodec::~TGAImageCodec()
{
}

Texture* TGAImageCodec::load(const RawDataContainer& data, Texture* result)
{
    Logger::getSingleton().logEvent("TGAImageCodec::load()", Informative);
    ImageTGA* img = loadTGA(data.getDataPtr(), data.getSize());
    if (img == 0)
    {
        return 0;
    }
    else 
    {
        Texture::PixelFormat fmt = (img->channels == 3) ? Texture::PF_RGB : Texture::PF_RGBA;
        result->loadFromMemory(img->data, Sizef(static_cast<float>(img->sizeX), static_cast<float>(img->sizeY)), fmt);
        if (img->data)
        {
            delete[] img->data; 
        }
        // Free the image structure
        delete img;
    }
    return result;
}

/*************************************************************************
	flips data vertically for tImageTGA 'img'	
*************************************************************************/
void TGAImageCodec::flipVertImageTGA(ImageTGA* img)
{
    
	int pitch = img->sizeX * img->channels;
    
	// flip the image bits...
	for (int line = 0; line < img->sizeY / 2; ++line)
	{
		int srcOffset = (line * pitch);
		int dstOffest = ((img->sizeY - line - 1) * pitch);

		for (int colBit = 0; colBit < pitch; ++colBit)
		{
			uchar tmp = img->data[dstOffest + colBit];
			img->data[dstOffest + colBit] = img->data[srcOffset + colBit];
			img->data[srcOffset + colBit] = tmp;
		}

	}
}

//----------------------------------------------------------------------------//
void TGAImageCodec::flipHorzImageTGA(ImageTGA* img)
{
	const int pitch = img->sizeX * img->channels;
    
	for (int line = 0; line < img->sizeY; ++line)
	{
		int srcOffset = (line * pitch);
		int dstOffest = srcOffset + pitch - img->channels;

		for (int pix = 0; pix < img->sizeX / 2; ++pix)
		{
            for (int c = 0; c < img->channels; ++c)
            {
                const uchar tmp = img->data[dstOffest + c];
                img->data[dstOffest + c] = img->data[srcOffset + c];
                img->data[srcOffset + c] = tmp;
            }

            srcOffset += img->channels;
            dstOffest -= img->channels;
		}
	}
}

//----------------------------------------------------------------------------//
/*************************************************************************
   convert a 24 bits Image to a 32 bit one
   This function is a temporary hack used to convert all image to 
   32 bits per pixel. This function is going to be uneeded as soon as 
   we introduce a new parameter to loadFromMemory 
**************************************************************************/
void TGAImageCodec::convertRGBToRGBA(ImageTGA* img)
{
    unsigned char* data = new unsigned char[img->sizeX * img->sizeY * 4];
    int buffer = img->sizeX * img->sizeY * img->channels;
    int newI = 0;
    
    for(int i = 0 ; i < buffer ; )
    {
        data[newI++] = img->data[i++];
        data[newI++] = img->data[i++];
        data[newI++] = img->data[i++];
        data[newI++] = 0xff;
    }
    img->channels = 4;
    delete [] img->data;
    img->data = data;
}
///////////////////////////////// LOAD TGA \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This loads the TGA file and returns it's data in a tImageTGA struct
/////
/////   Modified by Paul D Turner to accept a raw data buffer & it's length
/////   as input.
/////
///////////////////////////////// LOAD TGA \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
 TGAImageCodec::ImageTGA* TGAImageCodec::loadTGA(const unsigned char* buffer, size_t /*buffer_size*/)
{
    typedef unsigned char byte;
    
    ImageTGA *pImageData = 0;		// This stores our important image data
	short width = 0, height = 0;	// The dimensions of the image
	byte length = 0;				// The length in bytes to the pixels
	byte imageType = 0;	     		// The image type (RLE, RGB, Alpha...)

	byte bits = 0;	    			// The bits per pixel for the image (16, 24, 32)
	int channels = 0;				// The channels of the image (3 = RGA : 4 = RGBA)
	int stride = 0;					// The stride (channels * width)
	int i = 0;						// A counter

    // This function loads in a TARGA (.TGA) file and returns its data to be
	// used as a texture or what have you.  This currently loads in a 16, 24
	// and 32-bit targa file, along with RLE compressed files.  Eventually you
	// will want to do more error checking to make it more robust.  This is
	// also a perfect start to go into a modular class for an engine.
	// Basically, how it works is, you read in the header information, then
	// move your file pointer to the pixel data.  Before reading in the pixel
	// data, we check to see the if it's an RLE compressed image.  This is because
	// we will handle it different.  If it isn't compressed, then we need another
	// check to see if we need to convert it from 16-bit to 24 bit.  24-bit and
	// 32-bit textures are very similar, so there's no need to do anything special.
	// We do, however, read in an extra bit for each color.

    	// Allocate the structure that will hold our eventual image data (must free it!)
	pImageData = new ImageTGA;

	// Read in the length in bytes from the header to the pixel data
	memcpy(&length, buffer, sizeof(byte));
	buffer += sizeof(byte);

	// Jump over one byte
	++buffer;

	// Read in the imageType (RLE, RGB, etc...)
	memcpy(&imageType, buffer, sizeof(byte));
	buffer += sizeof(byte);

	// Skip past general information we don't care about
	buffer += 9;

	// Read the width, height and bits per pixel (16, 24 or 32)
	memcpy(&width, buffer, sizeof(short));
	buffer += sizeof(short);
	memcpy(&height, buffer, sizeof(short));
	buffer += sizeof(short);
#ifdef __BIG_ENDIAN__
    width = ((width & 0xFF00)>>8) | ((width & 0x00FF)<<8);
    height = ((height & 0xFF00)>>8) | ((height & 0x00FF)<<8);
#endif
	memcpy(&bits, buffer, sizeof(byte));
	buffer += sizeof(byte);

    // see where and what we might need to flip
    const bool flip_horz = (buffer[0] & 16) == 16;
    const bool flip_vert = !((buffer[0] & 32) == 32);

	// Now we move the file pointer to the pixel data
	buffer += length + 1;

	// Check if the image is RLE compressed or not
	if(imageType != TGA_RLE)
	{
		// Check if the image is a 24 or 32-bit image
		if(bits == 24 || bits == 32)
		{
			// Calculate the channels (3 or 4) - (use bits >> 3 for more speed).
			// Next, we calculate the stride and allocate enough memory for the pixels.
			channels = bits / 8;
			stride = channels * width;
			pImageData->data = new unsigned char[stride * height];

			// Load in all the pixel data line by line
			for(int y = 0; y < height; y++)
			{
				// Store a pointer to the current line of pixels
				unsigned char *pLine = &(pImageData->data[stride * y]);

				// Read in the current line of pixels
				memcpy(pLine, buffer, stride);
				buffer += stride;

				// Go through all of the pixels and swap the B and R values since TGA
				// files are stored as BGR instead of RGB (or use GL_BGR_EXT verses GL_RGB)
				for(i = 0; i < stride; i += channels)
				{
					unsigned char temp     = pLine[i];
					pLine[i]     = pLine[i + 2];
					pLine[i + 2] = temp;
				}
			}
		}
		// Check if the image is a 16 bit image (RGB stored in 1 unsigned short)
		else if(bits == 16)
		{
			unsigned short pixels = 0;
			unsigned char r=0, g=0, b=0;

			// Since we convert 16-bit images to 24 bit, we hardcode the channels to 3.
			// We then calculate the stride and allocate memory for the pixels.
			channels = 3;
			stride = channels * width;
			pImageData->data = new unsigned char[stride * height];

			// Load in all the pixel data pixel by pixel
			for(int i = 0; i < width*height; i++)
			{
				// Read in the current pixel
				memcpy(&pixels, buffer, sizeof(unsigned short));
				buffer += sizeof(unsigned short);

				// To convert a 16-bit pixel into an R, G, B, we need to
				// do some masking and such to isolate each color value.
				// 0x1f = 11111 in binary, so since 5 bits are reserved in
				// each unsigned short for the R, G and B, we bit shift and mask
				// to find each value.  We then bit shift up by 3 to get the full color.
				b = static_cast<unsigned char>((pixels & 0x1f) << 3);
				g = static_cast<unsigned char>(((pixels >> 5) & 0x1f) << 3);
				r = static_cast<unsigned char>(((pixels >> 10) & 0x1f) << 3);

				// This essentially assigns the color to our array and swaps the
				// B and R values at the same time.
				pImageData->data[i * 3 + 0] = r;
				pImageData->data[i * 3 + 1] = g;
				pImageData->data[i * 3 + 2] = b;
			}
		}	
		// Else return a NULL for a bad or unsupported pixel format
		else
        {
            delete pImageData;
			return 0;
        }
	}
	// Else, it must be Run-Length Encoded (RLE)
	else
	{
		// First, let me explain real quickly what RLE is.  
		// For further information, check out Paul Bourke's intro article at: 
		// http://astronomy.swin.edu.au/~pbourke/dataformats/rle/
		// 
		// Anyway, we know that RLE is a basic type compression.  It takes
		// colors that are next to each other and then shrinks that info down
		// into the color and a integer that tells how much of that color is used.
		// For instance:
		// aaaaabbcccccccc would turn into a5b2c8
		// Well, that's fine and dandy and all, but how is it down with RGB colors?
		// Simple, you read in an color count (rleID), and if that number is less than 128,
		// it does NOT have any optimization for those colors, so we just read the next
		// pixels normally.  Say, the color count was 28, we read in 28 colors like normal.
		// If the color count is over 128, that means that the next color is optimized and
		// we want to read in the same pixel color for a count of (colorCount - 127).
		// It's 127 because we add 1 to the color count, as you'll notice in the code.

		// Create some variables to hold the rleID, current colors read, channels, & stride.
		unsigned char rleID = 0;
		int colorsRead = 0;
		channels = bits / 8;
		stride = channels * width;

		// Next we want to allocate the memory for the pixels and create an array,
		// depending on the channel count, to read in for each pixel.
		pImageData->data = new unsigned char[stride * height];
		byte *pColors = new byte [channels];

		// Load in all the pixel data
		while(i < width*height)
		{
			// Read in the current color count + 1
			memcpy(&rleID, buffer, sizeof(unsigned char));
			buffer += sizeof(unsigned char);

			// Check if we don't have an encoded string of colors
			if(rleID < 128)
			{
				// Increase the count by 1
				rleID++;

				// Go through and read all the unique colors found
				while(rleID)
				{
					// Read in the current color
					memcpy(pColors, buffer, sizeof(byte) * channels);
					buffer += sizeof(byte) * channels;

					// Store the current pixel in our image array
					pImageData->data[colorsRead + 0] = pColors[2];
					pImageData->data[colorsRead + 1] = pColors[1];
					pImageData->data[colorsRead + 2] = pColors[0];

					// If we have a 4 channel 32-bit image, assign one more for the alpha
					if(bits == 32)
						pImageData->data[colorsRead + 3] = pColors[3];

					// Increase the current pixels read, decrease the amount
					// of pixels left, and increase the starting index for the next pixel.
					i++;
					rleID--;
					colorsRead += channels;
				}
			}
			// Else, let's read in a string of the same character
			else
			{
				// Minus the 128 ID + 1 (127) to get the color count that needs to be read
				rleID -= 127;

				// Read in the current color, which is the same for a while
				memcpy(pColors, buffer, sizeof(byte) * channels);
				buffer += sizeof(byte) * channels;

				// Go and read as many pixels as are the same
				while(rleID)
				{
					// Assign the current pixel to the current index in our pixel array
					pImageData->data[colorsRead + 0] = pColors[2];
					pImageData->data[colorsRead + 1] = pColors[1];
					pImageData->data[colorsRead + 2] = pColors[0];

					// If we have a 4 channel 32-bit image, assign one more for the alpha
					if(bits == 32)
						pImageData->data[colorsRead + 3] = pColors[3];

					// Increase the current pixels read, decrease the amount
					// of pixels left, and increase the starting index for the next pixel.
					i++;
					rleID--;
					colorsRead += channels;
				}

			}

		}
		// Free up pColors
		delete[] pColors;
	}
	// Fill in our tImageTGA structure to pass back
	pImageData->channels = channels;
	pImageData->sizeX    = width;
	pImageData->sizeY    = height;

    // now we've read the data, do required flips so that origin is top-left
    if (flip_vert)
        flipVertImageTGA(pImageData);

    if (flip_horz)
        flipHorzImageTGA(pImageData);

	// Return the TGA data (remember, you must free this data after you are done)
	return pImageData;
}


}

