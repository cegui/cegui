/************************************************************************
	filename: 	ogretexture.cpp
	created:	11/5/2004
	author:		Paul D Turner
	
	purpose:	Implementation of Texture using Ogre engine
*************************************************************************/
/*************************************************************************
    Crazy Eddie's GUI System (http://crayzedsgui.sourceforge.net)
    Copyright (C)2004 Paul D Turner (crayzed@users.sourceforge.net)

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
#include "renderers/OgreGUIRenderer/ogretexture.h"
#include "renderers/OgreGUIRenderer/ogrerenderer.h"
#include "CEGUIExceptions.h"
#include <string>
#include <Ogre.h>

// Start of CEGUI namespace section
namespace CEGUI
{
/*************************************************************************
	Static data definition / initialisation
*************************************************************************/
ulong OgreTexture::d_texturenumber		= 0;


/*************************************************************************
	Constructor
*************************************************************************/
OgreTexture::OgreTexture(Renderer* owner) :
	Texture(owner)
{
	d_ogre_texture = NULL;
	d_isLinked = false;
}


/*************************************************************************
	Destructor
*************************************************************************/
OgreTexture::~OgreTexture(void)
{
	freeOgreTexture();
}


/*************************************************************************
	Loads the specified image file into the texture.  The texture is
	resized as required to hold the image.	
*************************************************************************/
void OgreTexture::loadFromFile(const String& filename)
{
	using namespace Ogre;

	// unload old ogre texture
	freeOgreTexture();

	// create / load a new ogre texture from the specified image
	try
	{
		TextureManager& textureManager = TextureManager::getSingleton();

		// see if texture already exists
		Ogre::Texture* ogreTexture = (Ogre::Texture*)textureManager.getByName(filename.c_str());

		if (ogreTexture)
		{
			// texture already exists, so create a 'linked' texture (ensures texture is not destroyed twice)
			d_ogre_texture = ogreTexture;
			d_isLinked = true;
		}
		// texture does not already exist, so load it in
		else
		{
			d_ogre_texture = TextureManager::getSingleton().load(filename.c_str(), TEX_TYPE_2D, 1, 1.0f, 1);
			d_isLinked = false;
		}

	}
	catch(Ogre::Exception e)
	{
		throw RendererException((utf8*)"Failed to create Texture object from file '" + filename + "'. Additional Information:\n" + e.getFullDescription().c_str());
	}

	// if we got a pointer cache some details
	if (d_ogre_texture != NULL)
	{
		d_width		= d_ogre_texture->getWidth();
		d_height	= d_ogre_texture->getHeight();
	}
	// no texture from image so throw.
	else
	{
		throw RendererException((utf8*)"Failed to create Texture object from file '" + filename + "'.  Ogre returned a NULL pointer.");
	}

}


/*************************************************************************
	Loads (copies) an image in memory into the texture.  The texture is
	resized as required to hold the image.	
*************************************************************************/
void OgreTexture::loadFromMemory(const void* buffPtr, uint buffWidth, uint buffHeight)
{
	using namespace Ogre;

	// get rid of old texture
	freeOgreTexture();

	// wrap input buffer with an Ogre DataChunk
	ulong bytesize = ((buffWidth * sizeof(ulong)) * buffHeight);
	const DataChunk odc(const_cast<void*>(buffPtr), bytesize);

	// try to create a Ogre::Texture from the input data
	d_ogre_texture = TextureManager::getSingleton().loadRawData(getUniqueName(), odc, buffWidth, buffHeight, PF_A8R8G8B8, TEX_TYPE_2D, 1, 1.0f, 1);

	// if we got a pointer cache some details
	if (d_ogre_texture != NULL)
	{
		d_width		= d_ogre_texture->getWidth();
		d_height	= d_ogre_texture->getHeight();
	}
	// no texture from memory so throw.
	else
	{
		throw RendererException((utf8*)"Failed to create Texture object from memory:  Ogre returned a NULL Ogre::Texture pointer.");
	}

}


/*************************************************************************
	set the size of the internal Ogre texture.  Previous Ogre texture
	is lost.	
*************************************************************************/
void OgreTexture::setOgreTextureSize(uint size)
{
	using namespace Ogre;

	// unload any current Ogre::Texture
	freeOgreTexture();

	// Try to create an empty texture of the given size
	d_ogre_texture = TextureManager::getSingleton().createManual(getUniqueName(), TEX_TYPE_2D, size, size, 1, PF_A8R8G8B8, TU_DEFAULT);

	// if we got a pointer cache some details
	if (d_ogre_texture != NULL)
	{
		d_width		= d_ogre_texture->getWidth();
		d_height	= d_ogre_texture->getHeight();
	}
	// no texture so throw.
	else
	{
		throw RendererException((utf8*)"Failed to create texture of specified size: Ogre::Texture creation failed.");
	}

}


/*************************************************************************
	safely free Ogre::Texture texture (can be called multiple times with
	no ill effect)
*************************************************************************/
void OgreTexture::freeOgreTexture(void)
{
	if (d_ogre_texture != NULL)
	{
		Ogre::TextureManager::getSingleton().unload(d_ogre_texture->getName());
		d_ogre_texture->destroy();
		d_ogre_texture = NULL;
	}
}


/*************************************************************************
	return a Ogre::string that contains a unique name.	
*************************************************************************/
Ogre::String OgreTexture::getUniqueName(void)
{
	Ogre::String str;

#ifdef CEGUI_USEOLDOGRESTRING
	str << "_cegui_ogre_" << d_texturenumber;
#else
	Ogre::StringUtil::StrStreamType strstream;
	strstream << "_cegui_ogre_" << d_texturenumber;
	str = strstream.str();
#endif

	++d_texturenumber;

	return str;
}


/*************************************************************************
	Set the internal Ogre::Texture object.
*************************************************************************/
void OgreTexture::setOgreTexture(Ogre::Texture& texture)
{
	freeOgreTexture();

	d_ogre_texture = &texture;
	d_width	 = d_ogre_texture->getWidth();
	d_height = d_ogre_texture->getHeight();
	d_isLinked = true;
}


} // End of  CEGUI namespace section

