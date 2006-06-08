/***********************************************************************
	filename:	opengltexture.h
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
#ifndef _opengltexture_h_
#define _opengltexture_h_

#include "CEGUIBase.h"
#include "CEGUIRenderer.h"
#include "CEGUITexture.h"
#include "openglrenderer.h"

#include <list>


// Start of CEGUI namespace section
namespace CEGUI
{

/*!
\brief
	Texture class that is created by OpenGLRenderer objects
*/
class OPENGL_GUIRENDERER_API OpenGLTexture : public Texture
{
private:
    /*************************************************************************
	    Friends (to allow construction and destruction)
    *************************************************************************/
    friend	Texture* OpenGLRenderer::createTexture(void);
    friend	Texture* OpenGLRenderer::createTexture(const String& filename, const String& resourceGroup);
    friend	Texture* OpenGLRenderer::createTexture(float size);
    friend	void	 OpenGLRenderer::destroyTexture(Texture* texture);


    /*************************************************************************
		Construction & Destruction (by Renderer object only)
    *************************************************************************/
    OpenGLTexture(Renderer* owner);
    virtual ~OpenGLTexture(void);

public:
    /*!
    \brief
	    Returns the current pixel width of the texture

    \return
	    ushort value that is the current width of the texture in pixels
    */
    virtual	ushort	getWidth(void) const		{return d_width;}


    /*!
    \brief
	    Returns the current pixel height of the texture

    \return
	    ushort value that is the current height of the texture in pixels
    */
    virtual	ushort	getHeight(void) const		{return d_height;}


    /*!
    \brief
	    Loads the specified image file into the texture.  The texture is resized as required to hold the image.

    \param filename
	    The filename of the image file that is to be loaded into the texture

    \param resourceGroup
        Resource group identifier passed to the resource provider.

    \return
	    Nothing.
    */
    virtual void	loadFromFile(const String& filename, const String& resourceGroup);


    /*!
    \brief
	    Loads (copies) an image in memory into the texture.  The texture is resized as required to hold the image.

    \param buffPtr
	    Pointer to the buffer containing the image data

    \param buffWidth
	    Width of the buffer (in 0xAARRGGBB pixels)

    \param buffHeight
	    Height of the buffer (in 0xAARRGGBB pixels)

    \return
		Nothing.
    */
    virtual void	loadFromMemory(const void* buffPtr, uint buffWidth, uint buffHeight);


    /*!
    \brief
	    Return a pointer to the internal texture id

    \return
	    Texture id that is loaded
    */
    GLuint	getOGLTexid(void) const {return d_ogltexture;}


    /*!
    \brief
	    set the size of the internal texture.

    \param size
	    pixel size of the new internal texture.  This will be rounded up to a power of 2.

    \return
	    Nothing.
    */
    void	setOGLTextureSize(uint size);


    /************************************************************************
        Grab/restore
    *************************************************************************/
    /*!
    \brief
        Grab the texture to a local buffer.
        This will destroy the OpenGL texture, and restoreTexture must be called before using it again.
    */
    void grabTexture(void);


    /*!
    \brief
        Restore the texture from the locally buffered copy previously create by a call to grabTexture.
    */
    void restoreTexture(void);


private:
#ifndef USE_DEVIL_LIBRARY
// These defines are used to tell us about the type of TARGA file it is
#	define TGA_RGB		 2		// This tells us it's a normal RGB (really BGR) file
#	define TGA_A		 3		// This tells us it's a ALPHA file
#	define TGA_RLE		10		// This tells us that the targa is Run-Length Encoded (RLE)

	/*************************************************************************
		Implementation Struct
	*************************************************************************/
	// This is our image structure for our targa data
	struct tImageTGA
	{
		int channels;			// The channels in the image (3 = RGB : 4 = RGBA)
		int sizeX;				// The width of the image in pixels
		int sizeY;				// The height of the image in pixels
		unsigned char *data;	// The image pixel data
	};


	// flips data for tImageTGA 'img'
	static void flipImageTGA(tImageTGA* img);
    

	// Took this code from http://www.gametutorials.com still ne
    // tImageTGA *LoadTGA(const char *filename)
    //
    // This is our cool function that loads the targa (TGA) file, then returns it's data.  
    // This tutorial supports 16, 24 and 32 bit images, along with RLE compression.
    //
    //
    // Ben Humphrey (DigiBen)
    // Game Programmer
    // DigiBen@GameTutorials.com
    // Co-Web Host of www.GameTutorials.com
    //
	//
	// Modified by Paul D Turner to accept a raw data buffer & it's length
	// as input.
	//
    tImageTGA* LoadTGA(const unsigned char* buffer, size_t buffer_size);

#endif
	/*************************************************************************
		Implementation Data
	*************************************************************************/
    GLuint 		d_ogltexture;		//!< The 'real' texture.
    ushort		d_width;			//!< cached width of the texture
    ushort		d_height;			//!< cached height of the texture
    uint8*		d_grabBuffer;       //!< cached image data for restoring the texture
};

} // End of  CEGUI namespace section


#endif	// end of guard _opengltexture_h_
