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

    virtual ushort getOriginalWidth(void) const { return d_orgWidth; }
    virtual ushort getOriginalHeight(void) const { return d_orgHeight; }

    virtual float getXScale(void) const { return d_xScale; } 
    virtual float getYScale(void) const { return d_yScale; } 

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
		Width of the buffer (in pixels as specified by \a pixelFormat )

	\param buffHeight
		Height of the buffer (in pixels as specified by \a pixelFormat )

    \param pixelFormat
        PixelFormat value describing the format contained in \a buffPtr

	\return
		Nothing.
	*/
    virtual void	loadFromMemory(const void* buffPtr, uint buffWidth, uint buffHeight, PixelFormat pixelFormat);


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
    //! updates cached scale value used to map pixels to texture co-ords.
    void updateCachedScaleValues();
    
    //! returns next power of 2 size if \a size is not power of 2
    uint getSizeNextPOT(uint size) const;

	/*************************************************************************
		Implementation Data
	*************************************************************************/
    GLuint 		d_ogltexture;		//!< The 'real' texture.
    ushort		d_width;			//!< cached width of the texture
    ushort		d_height;			//!< cached height of the texture
    uint8*		d_grabBuffer;       //!< cached image data for restoring the texture
    //! original pixel width of data loaded into texture
    ushort d_orgWidth;
    //! original pixel height of data loaded into texture
    ushort d_orgHeight;
    //! cached value for x scale
    float d_xScale;
    //! cahced value foy y scale
    float d_yScale;
};

} // End of  CEGUI namespace section


#endif	// end of guard _opengltexture_h_
