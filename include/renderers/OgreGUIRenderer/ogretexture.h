/************************************************************************
	filename: 	ogretexture.h
	created:	11/5/2004
	author:		Paul D Turner
	
	purpose:	Interface to Texture implemented via Ogre engine
*************************************************************************/
/*************************************************************************
    Crazy Eddie's GUI System (http://www.cegui.org.uk)
    Copyright (C)2004 - 2005 Paul D Turner (paul@cegui.org.uk)

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*************************************************************************/
#ifndef _ogretexture_h_
#define _ogretexture_h_

#include "CEGUIBase.h"
#include "CEGUIRenderer.h"
#include "CEGUITexture.h"
#include "ogrerenderer.h"

// Start of CEGUI namespace section
namespace CEGUI
{
/*!
\brief
	Texture class that is created by OgreRenderer objects
*/
class OGRE_GUIRENDERER_API OgreTexture : public Texture
{
private:
	/*************************************************************************
		Friends (to allow construction and destruction)
	*************************************************************************/
	friend	Texture* OgreRenderer::createTexture(void);
	friend	Texture* OgreRenderer::createTexture(const String& filename);
	friend	Texture* OgreRenderer::createTexture(float size);
	friend	void	 OgreRenderer::destroyTexture(Texture* texture);


	/*************************************************************************
		Construction & Destruction (by Renderer object only)
	*************************************************************************/
	OgreTexture(Renderer* owner);
	virtual ~OgreTexture(void);

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

	\return
		Nothing.
	*/
	virtual void	loadFromFile(const String& filename);


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
		Return a pointer to the internal Ogre::Texture object

	\return
		Pointer to the Ogre::Texture object currently being used by this Texture object
	*/
	Ogre::Texture*	getOgreTexture(void) const		{return d_ogre_texture;}


	/*!
	\brief
		set the size of the internal Ogre texture.  Previous Ogre texture is lost.

	\param size
		pixel size of the new internal texture.  This will be rounded up to a power of 2.

	\return
		Nothing.
	*/
	void	setOgreTextureSize(uint size);


	/*!
	\brief
		Set the internal Ogre::Texture object.

	\param texture
		Reference to an Ogre::Texture object that is to be used by this Texture object.

	\return
		Nothing.
	*/
	void	setOgreTexture(Ogre::Texture& texture);


private:
	/*************************************************************************
		Implementation Functions
	*************************************************************************/
	// safely free Ogre::Texture texture (can be called multiple times with no ill effect)
	void	freeOgreTexture(void);

	// return a Ogre::string that contains a unique name.
	Ogre::String	getUniqueName(void);


	/*************************************************************************
		Implementation Data
	*************************************************************************/
	static	ulong			d_texturenumber;	//!< Counter used to provide unique texture names.

	Ogre::Texture*			d_ogre_texture;		//!< The 'real' texture.

	ushort					d_width;			//!< cached width of the texture
	ushort					d_height;			//!< cached height of the texture

	bool	d_isLinked;		//!< True if we are linked to a texture we did not actually create.
};


} // End of  CEGUI namespace section


#endif	// end of guard _ogretexture_h_
