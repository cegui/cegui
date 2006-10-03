/***********************************************************************
	filename:	opengltexture.cpp
	created:	9/4/2004
	author:		Mark Strom
				mwstrom@gmail.com

	purpose:	Interface to Texture implemented via Opengl
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
#include "opengltexture.h"
#include "CEGUIExceptions.h"
#include "CEGUISystem.h"
#include "CEGUIImageCodec.h"
// Start of CEGUI namespace section
namespace CEGUI
{

/*************************************************************************
	Constructor
*************************************************************************/
OpenGLTexture::OpenGLTexture(Renderer* owner) :
	Texture(owner),
	d_grabBuffer(0),
    d_xScale(1.0f),
    d_yScale(1.0f)
{
	// generate a OGL texture that we will use.
	glGenTextures(1, &d_ogltexture);

	// set some parameters for this texture.
	glBindTexture(GL_TEXTURE_2D, d_ogltexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, 0x812F);	// GL_CLAMP_TO_EDGE
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, 0x812F);	// GL_CLAMP_TO_EDGE
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
}

/*************************************************************************
	Destructor
*************************************************************************/
OpenGLTexture::~OpenGLTexture(void)
{
    // if the grabbuffer is not empty then free it
    if (d_grabBuffer)
    {
        delete [] d_grabBuffer;
    }
    // otherwise delete OGL texture associated with this object.
    else
    {
	    glDeleteTextures(1, &d_ogltexture);
    }
}


/*************************************************************************
	Load texture from file.  Texture is made to be same size as image in
	file.
*************************************************************************/
void OpenGLTexture::loadFromFile(const String& filename, const String& resourceGroup)
{
    // Note from PDT:
    // There is somewhat tight coupling here between OpenGLTexture and the
    // ImageCodec classes - we have intimate knowledge of how they are
    // implemented and that knowledge is relied upon in an unhealthy way; this
    // should be addressed at some stage.

    OpenGLRenderer* renderer =  static_cast<OpenGLRenderer*>(getRenderer());
	glBindTexture(GL_TEXTURE_2D, d_ogltexture);
	// load file to memory via resource provider
	RawDataContainer texFile;
	System::getSingleton().getResourceProvider()->loadRawDataContainer(filename, texFile, resourceGroup);
	Texture* res = renderer->getImageCodec().load(texFile, this);
	// unload file data buffer
	System::getSingleton().getResourceProvider()->unloadRawDataContainer(texFile);
	if (res == 0)
    {
        // It's an error 
		throw RendererException("OpenGLTexture::loadFromFile - " + 
                                renderer->getImageCodec().getIdentifierString() + 
                                " failed to load image '" + filename + "'.");
    }
}


/*************************************************************************
	Load texture from raw memory.	
*************************************************************************/
void OpenGLTexture::loadFromMemory(const void* buffPtr, uint buffWidth, uint buffHeight, Texture::PixelFormat pixFormat)
{
	GLint comps;
	GLenum format;
	switch (pixFormat)
	{
	    case PF_RGB:
	        comps = 3;
	        format = GL_RGB;
	        break;
	    case PF_RGBA:
	        comps = 4;
	        format = GL_RGBA;
	        break;
	};

    // At least for now, we will enforce POT and square texture sizes, this way
    // we do not need to test for, and have adaptations for, missing extensions.
    setOGLTextureSize(ceguimax(buffWidth, buffHeight));
    // store sizes of original data we are loading
    d_orgWidth = buffWidth;
    d_orgHeight = buffHeight;
    // update scale values
    updateCachedScaleValues();
    // do the real work of getting the data into the texture
    glBindTexture(GL_TEXTURE_2D, d_ogltexture);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, buffWidth, buffHeight, format, GL_UNSIGNED_BYTE, buffPtr);
}


/*************************************************************************
	allocate atexture >= 'size'.  Previous texture is lost
*************************************************************************/
void OpenGLTexture::setOGLTextureSize(uint size)
{
    size = getSizeNextPOT(size);

	// make sure size is within boundaries
	int maxSize;
	glGetIntegerv(GL_MAX_TEXTURE_SIZE, (GLint*)&maxSize);
	if (size > (uint)maxSize)
	{
	    throw RendererException("OpenGLTexture::setOGLTextureSize - size too big");
	}

	// allocate temp buffer for texture
	uchar* buff = new uchar[size * size * 4];

	// load empty buffer to texture
	glBindTexture(GL_TEXTURE_2D, d_ogltexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size, size, 0, GL_RGBA ,GL_UNSIGNED_BYTE, buff);

	// delete buffer
	delete[] buff;

	d_orgWidth = d_orgHeight = d_height = d_width = static_cast<ushort>(size);
    updateCachedScaleValues();
}

/************************************************************************
    Grabs this texture's image data into a buffer for later restoration
*************************************************************************/
void OpenGLTexture::grabTexture(void)
{
    // bind the texture we want to grab
    glBindTexture(GL_TEXTURE_2D, d_ogltexture);
    // allocate the buffer for storing the image data
    d_grabBuffer = new uint8[4*d_width*d_height];
    glGetTexImage(GL_TEXTURE_2D,0,GL_RGBA,GL_UNSIGNED_BYTE,d_grabBuffer);
    // delete the texture
    glDeleteTextures(1, &d_ogltexture);
}


/************************************************************************
    Restores this texture from the previously grabbed image data
*************************************************************************/
void OpenGLTexture::restoreTexture(void)
{
    // recreate texture
    // generate a OGL texture that we will use.
	glGenTextures(1, &d_ogltexture);

	// set some parameters for this texture.
	glBindTexture(GL_TEXTURE_2D, d_ogltexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, 0x812F);	// GL_CLAMP_TO_EDGE
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, 0x812F);	// GL_CLAMP_TO_EDGE
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, d_width, d_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, d_grabBuffer);
    
    // free the grabbuffer
    delete [] d_grabBuffer;
    d_grabBuffer = 0;
}


/************************************************************************
    Update stored scale values for pixel to texture co-ord mapping
*************************************************************************/
void OpenGLTexture::updateCachedScaleValues()
{
    //
    // calculate what to use for x scale
    //
    ushort orgW = getOriginalWidth();
    ushort texW = getWidth();

    // if texture and original data width are the same, scale is based
    // on the original size.
    // if texture is wider (and source data was not stretched), scale
    // is based on the size of the resulting texture.
    d_xScale = 1.0f / ((orgW == texW) ?
        static_cast<float>(orgW) :
        static_cast<float>(texW));

    //
    // calculate what to use for y scale
    //
    ushort orgH = getOriginalHeight();
    ushort texH = getHeight();

    // if texture and original data height are the same, scale is based
    // on the original size.
    // if texture is taller (and source data was not stretched), scale
    // is based on the size of the resulting texture.
    d_yScale = 1.0f / ((orgH == texH) ?
        static_cast<float>(orgH) :
        static_cast<float>(texH));
}

/************************************************************************
    Use to ensure power of 2 texture sizes
*************************************************************************/
uint OpenGLTexture::getSizeNextPOT(uint size) const
{
    // if not power of 2
    if ((size & (size - 1)) || !size)
    {
        int log = 0;

        // get integer log of 'size' to base 2
        while (size >>= 1)
            ++log;

        // use log to calculate value to use as size.
        size = (2 << log);
    }

    return size;
}

} // End of  CEGUI namespace section
