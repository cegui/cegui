/***********************************************************************
	filename: 	texture.h
	created:	10/4/2004
	author:		Paul D Turner
	
	purpose:	Defines concrete texture class
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
#ifndef _texture_h_
#define _texture_h_

#include "CEGUIBase.h"
#include "CEGUIRenderer.h"
#include "CEGUITexture.h"
#include "renderer.h"
#include <d3d8.h>
#include <list>

// Start of CEGUI namespace section
namespace CEGUI
{

/*!
\brief
	Texture class that is created by DirectX81Renderer objects
*/
class DIRECTX81_GUIRENDERER_API DirectX81Texture : public Texture
{
private:
	/*************************************************************************
		Friends (to allow construction and destruction)
	*************************************************************************/
	friend	Texture* DirectX81Renderer::createTexture(void);
	friend	Texture* DirectX81Renderer::createTexture(const String& filename, const String& resourceGroup);
	friend	Texture* DirectX81Renderer::createTexture(float size);
	friend	void	 DirectX81Renderer::destroyTexture(Texture* texture);


	/*************************************************************************
		Construction & Destruction (by Renderer object only)
	*************************************************************************/
	DirectX81Texture(Renderer* owner);
	virtual ~DirectX81Texture(void);

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

    virtual	ushort getOriginalWidth(void) const { return d_orgWidth; }
    virtual	ushort getOriginalHeight(void) const { return d_orgHeight; }

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
		Return a pointer to the internal Direct3DTexture8 object

	\return
		Pointer to the IDirect3DTexture8 interface currently being used by this Texture object
	*/
	LPDIRECT3DTEXTURE8	getD3DTexture(void) const		{return d_d3dtexture;}


	// 
	/*!
	\brief
		set the size of the internal D3D texture.  Previous D3D texture is lost.

	\param size
		pixel size of the new internal texture.  This will be rounded up to a power of 2.

	\return
		Nothing.
	*/
	void	setD3DTextureSize(uint size);


	/*!
	\brief
		Direct3D support method that must be called prior to a Reset call on the
		Direct3DDevice; this is required so that the GUI renderer can release any
		unmanaged D3D resources as needed for the device reset to succeed.

	\note
		You do not need to call this on DirectX81Texture objects created via the
		DirectX81Renderer object; for such textures the method is called by the
		system when you call the DirectX81Renderer::preD3DReset method.
	*/
	virtual	void	preD3DReset(void);


	/*!
	\brief
		Direct3D support method that must be called after a Reset call on the
		Direct3DDevice; this is required so that the GUI renderer can rebuild any
		unmanaged D3D resources after the device has been reset.

	\note
		You do not need to call this on DirectX81Texture objects created via the
		DirectX81Renderer object; for such textures the method is called by the
		system when you call the DirectX81Renderer::postD3DReset method.
	*/
	virtual	void	postD3DReset(void);


private:
	/*************************************************************************
		Implementation Functions
	*************************************************************************/
	// safely free direc3d texture (can be called multiple times with no ill effect)
	void	freeD3DTexture(void);
    // obtain and fill-in the real texture dimensions.
    // d_orgWidth and d_orgHeight should be set before calling this, since
    // they are used as fall-back values if the query fails.
    void obtainActualTextureSize(void);

	/*************************************************************************
		Implementation Data
	*************************************************************************/
	LPDIRECT3DTEXTURE8		d_d3dtexture;		//!< The 'real' texture.
	String					d_filename;			//!< name of file used to create the texture, if any.
    String                  d_resourceGroup;    //!< resource provider group ID to use when loading file.
    bool					d_isMemoryTexture;	//!< true if the texture was created from memory (and not a file).

	ushort					d_width;			//!< cached width of the texture
	ushort					d_height;			//!< cached height of the texture
    ushort					d_orgWidth;			//!< width of the source material for the texture
    ushort					d_orgHeight;		//!< height of the source material for the texture
};

} // End of  CEGUI namespace section


#endif	// end of guard _texture_h_
