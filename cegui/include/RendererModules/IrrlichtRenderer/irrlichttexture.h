/***********************************************************************
	filename: 	irrlichttexture.h
	created:	20/7/2004
	author:		Thomas Suter
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
#ifndef IRRLICHTTEXTURE_H_INCLUDED
#define IRRLICHTTEXTURE_H_INCLUDED

#include "IrrlichtRendererDef.h"

#include "CEGUIBase.h"
#include "CEGUIRenderer.h"
#include "CEGUITexture.h"

#include <irrlicht.h>

namespace CEGUI
{
	class IRRLICHT_GUIRENDERER_API IrrlichtTexture : public Texture
	{
	public:

		// constructor
		IrrlichtTexture(Renderer* r, irr::IrrlichtDevice* device);
        // constructor with a predefined size.
        IrrlichtTexture(Renderer* r, irr::IrrlichtDevice* dr, float size);

		// destructor
		virtual ~IrrlichtTexture();
		
		// returns the irrlich texture
		irr::video::ITexture* getTexture();

		// set the irrlicht texture
		void setTexture(irr::video::ITexture* texture);


		/*!
		\brief
		Returns the current pixel width of the texture

		\return
		ushort value that is the current width of the texture in pixels
		*/
		virtual	ushort	getWidth(void) const;

		/*!
		\brief
		Returns the original pixel width of the texture

		\return
		ushort value that is the original width of the texture data
        in pixels
		*/
		virtual	ushort	getOriginalWidth(void) const;

        virtual float getXScale(void) const;

		/*!
		\brief
		Returns the current pixel height of the texture

		\return
		ushort value that is the current height of the texture in pixels
		*/
		virtual	ushort	getHeight(void) const;

		/*!
		\brief
		Returns the original pixel height of the texture

		\return
		ushort value that is the original height of the texture data
        in pixels
		*/
		virtual	ushort	getOriginalHeight(void) const;

        virtual float getYScale(void) const;

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

	private:

		// counter for unique texture names
		static int iTextureNumber;

		// generate a unique name
		static irr::core::stringc getUniqueName(void);
		
		// remove the texture from irrlicht textures
		void freeTexture();

        // updates cached scale value used to map pixels to texture co-ords.
        void updateCachedScaleValues();

		// the current texture for rendering
		irr::video::ITexture* tex;

		// the irrlicht video driver 
		irr::video::IVideoDriver* driver;

		// the irrlicht device
		irr::IrrlichtDevice* device;

        // cached value for x scale
        float xScale;

        // cahced value foy y scale
        float yScale;
	};
}
#endif
