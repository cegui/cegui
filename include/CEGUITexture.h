/************************************************************************
	filename: 	CEGUITexture.h
	created:	21/2/2004
	author:		Paul D Turner
	
	purpose:	Defines abstract interface for texture objects.  Texture
				objects are created & destroyed by the Renderer.
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
class CEGUIBASE_API Texture
{
public:
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
		Returns the current pixel height of the texture

	\return
		ushort value that is the current height of the texture in pixels
	*/
	virtual	ushort	getHeight(void) const = 0;


	/*!
	\brief
		Loads the specified image file into the texture.  The texture is resized as required to hold the image.

	\param filename
		The filename of the image file that is to be loaded into the texture

	\return
		Nothing.

	\exception
	*/
	virtual void	loadFromFile(const String& filename) = 0;


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

	\exception
	*/
	virtual void	loadFromMemory(const void* buffPtr, uint buffWidth, uint buffHeight) = 0;


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

public:		// luabind compatibility
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
