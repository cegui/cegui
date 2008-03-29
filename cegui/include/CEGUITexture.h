/***********************************************************************
	filename: 	CEGUITexture.h
	created:	21/2/2004
	author:		Paul D Turner
	
	purpose:	Defines abstract interface for texture objects.  Texture
				objects are created & destroyed by the Renderer.
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
#ifndef _CEGUITexture_h_
#define _CEGUITexture_h_

#include "CEGUIBase.h"
#include "CEGUIString.h"

// Start of CEGUI namespace section
namespace CEGUI
{
/*!
\brief
	Abstract base class specifying the required interface for Texture objects.

	Texture objects are created via the Renderer.  The actual inner workings of any Texture object
	are dependant upon the Renderer (and underlying API) in use.  This base class defines the minimal
	set of functions that is required for the rest of the system to work.  Texture objects are only
	created through the Renderer object's texture creation functions.
*/
class CEGUIEXPORT Texture
{
public:
    /*!
    \brief
        Enum containing the list of supported pixel formats that can be passed to loadFromMemory
    */
    enum PixelFormat
    {
        //! Each pixel is 3 bytes. RGB in that order.
        PF_RGB,
        //! Each pixel is 4 bytes. RGBA in that order.
        PF_RGBA
    };

	/*************************************************************************
		Abstract Interface
	*************************************************************************/
	/*!
	\brief
		Returns the current pixel width of the texture

	\return
		ushort value that is the current width of the texture in pixels
	*/
	virtual	ushort	getWidth(void) const = 0;

    /*!
    \brief
        Returns the original pixel width of the data loaded into the texture.

    \return
        ushort value that is the original width, in pixels, of the data last
        loaded into the texture.

    \note 
        for compatibility reason this method is optional the auto scale 
        issue mantis ticket # 0000045 is not fixed for renderer that do 
        not handle this. 
    */
    virtual ushort getOriginalWidth(void) const { return getWidth(); }

    /*! 
    \brief 
        Returns the current scale used for the width of the texture 

    \return 
        float value that denotes the horizontal scaling required to
        accurately map pixel positions to texture co-ords.
    */
    virtual float getXScale(void) const { return 1.0f / static_cast<float>(getOriginalWidth()); } 

	/*!
	\brief
		Returns the current pixel height of the texture

	\return
		ushort value that is the current height of the texture in pixels
	*/
	virtual	ushort	getHeight(void) const = 0;

    /*!
    \brief
        Returns the original pixel height of the data loaded into the texture.

    \return
        ushort value that is the original height, in pixels, of the data last
        loaded into the texture.

    \note for compatibility reason this method is optional the auto scale 
    issue mantis ticket # 0000045 is not fixed for renderer that do 
    not handle this. 
    */
    virtual ushort getOriginalHeight(void) const { return getHeight(); }

    /*! 
    \brief 
        Returns the current scale used for the height of the texture 

    \return 
        float value that denotes the vertical scaling required to
        accurately map pixel positions to texture co-ords.
    */
    virtual float getYScale(void) const { return 1.0f / static_cast<float>(getOriginalHeight()); }

	/*!
	\brief
		Loads the specified image file into the texture.  The texture is resized as required to hold the image.

	\param filename
		The filename of the image file that is to be loaded into the texture

    \param resourceGroup
        Resource group identifier to be passed to the resource provider when loading the image file.

	\return
		Nothing.
	*/
	virtual void	loadFromFile(const String& filename, const String& resourceGroup) = 0;


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
	virtual void	loadFromMemory(const void* buffPtr, uint buffWidth, uint buffHeight, PixelFormat pixelFormat) = 0;


	/*!
	\brief
		Return a pointer to the Renderer object that created and owns this Texture

	\return
		Pointer to the Renderer object that owns the Texture
	*/
	Renderer*	getRenderer(void) const			{return d_owner;}


protected:
	/*************************************************************************
		Construction and Destruction
	*************************************************************************/
	/*!
	\brief
		Constructor for Texture base class.  This is never called by client code.
	*/
	Texture(Renderer* owner) : d_owner(owner) {}

	/*!
	\brief
		Destructor for Texture base class.  This is never called by client code.
	*/
	virtual ~Texture(void) {}

private:
	Renderer* d_owner;		//<! Renderer object that created and owns this texture
};

} // End of  CEGUI namespace section

#endif	// end of guard _CEGUITexture_h_
