/************************************************************************
	filename: 	renderer.h
	created:	15/3/2004
	author:		Paul D Turner
	
	purpose:	Interface for DirectX 8.1 Renderer class
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
/*************************************************************************
	This file contains code that is specific to Win32 and DirectX
*************************************************************************/
#ifndef _DirectX81GUIRenderer_h_
#define _DirectX81GUIRenderer_h_

#ifdef DIRECTX81_GUIRENDERER_EXPORTS
#define DIRECTX81_GUIRENDERER_API __declspec(dllexport)
#else
#define DIRECTX81_GUIRENDERER_API __declspec(dllimport)
#endif

#include "CEGUIBase.h"
#include "CEGUIRenderer.h"
#include "CEGUITexture.h"
#include <d3d8.h>
#include <list>

// Start of CEGUI namespace section
namespace CEGUI
{
/*************************************************************************
	Forward refs
*************************************************************************/
class DirectX81Renderer;
class DirectX81Texture;


/*!
\brief
	Texture class that is created by DirectX81Renderer objects
*/
class DIRECTX81_GUIRENDERER_API DirectX81Texture : public Texture
{
private:
	/*************************************************************************
		Construction & Destruction (by Renderer object only)
	*************************************************************************/
	friend class DirectX81Renderer;

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


	/*!
	\brief
		Loads the specified image file into the texture.  The texture is resized as required to hold the image.

	\param filename
		The filename of the image file that is to be loaded into the texture

	\return
		Nothing.

	\exception
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

	\exception
	*/
	virtual void	loadFromMemory(const void* buffPtr, uint buffWidth, uint buffHeight);


	/*!
	\brief
		Return a pointer to the internal Direct3DTexture8 object

	\return
		Pointer to the IDirect3DTexture8 interface currently being used by this Texture object
	*/
	LPDIRECT3DTEXTURE8	getD3DTexture(void) const		{return d_d3dtexture;}


private:
	/*************************************************************************
		Implementation Functions
	*************************************************************************/
	// safely free direc3d texture (can be called multiple times with no ill effect)
	void	freeD3DTexture(void);

	/*************************************************************************
		Implementation Data
	*************************************************************************/
	LPDIRECT3DTEXTURE8		d_d3dtexture;		//!< The 'real' texture.

	ushort					d_width;			//!< cached width of the texture
	ushort					d_height;			//!< cached height of the texture
};


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

	
/*!
\brief
	Renderer class to interface with Microsoft DirectX 8.1
*/
class DIRECTX81_GUIRENDERER_API DirectX81Renderer : public Renderer
{
public:
	/*!
	\brief
		Constructor for Direct3D 8.1 Renderer object

	\param device
		Pointer to the IDirect3DDevice8 interface object that will be used for all rendering

	\param max_quads
		Maximum number of quads that the Renderer will be able to render per frame.
	*/
	DirectX81Renderer(LPDIRECT3DDEVICE8 device, uint max_quads);

	/*!
	\brief
		Destructor for DirectX81Renderer objects
	*/
	virtual ~DirectX81Renderer(void);

	// add's a quad to the list to be rendered
	virtual	void	addQuad(const Rect& dest_rect, float z, const Texture* tex, const Rect& texture_rect, const ColourRect& colours);

	// perform final rendering for all queued renderable quads.
	virtual	void	doRender(void);

	// clear the queue
	virtual	void	clearRenderList(void);

	// create an empty texture
	virtual	Texture*	createTexture(void);

	// create a texture and load it with the specified file.
	virtual	Texture*	createTexture(const String& filename);

	// create a texture and set it to the specified size
	virtual	Texture*	createTexture(float width, float height);

	// destroy the given texture
	virtual	void		destroyTexture(Texture* texture);

	// destroy all textures still active
	virtual void		destroyAllTextures(void);

	// return ptr to device
	LPDIRECT3DDEVICE8	getDevice(void) const		{return d_device;}


private:
	/************************************************************************
		Implementation Constants
	************************************************************************/
	const static int			VERTEX_PER_SPRITE;							//!< number of vertices per sprite
	const static int			VERTEX_PER_TRIANGLE;						//!< number of vertices for a triangle
	const static int			VERTEXBUFFER_CAPACITY;						//!< capacity of the allocated vertex buffer
	const static ulong			VERTEX_FVF;									//!< FVF specifier constant

	/*************************************************************************
	    Implementation Structs & classes
	*************************************************************************/
	/*!
	\brief
		FVF structure used for all vertices.
	*/
	struct QuadVertex {
		FLOAT x, y, z, rhw;		//!< The transformed position for the vertex.
		DWORD diffuse;			//!< colour of the vertex
		float tu1, tv1;			//!< texture coordinates
	};

	/*!
	\brief
		structure holding details about a quad to be drawn
	*/
	struct QuadInfo
	{
		LPDIRECT3DTEXTURE8	texture;
		Rect				position;
		float				z;
		Rect				texPosition;
		ColourRect			colours;
	};


	// this is a function object that is used by the sorting algorithm to
	// sort the quads.  It would be possible to define comparison operators
	// the the QuadInfo structure and not have this - the reason this was
	// not done, is because the system could be extended to have >1 quad
	// list each sorted by a different key (by texture for non-alpha stuff, etc).
	struct quadsorter
		: public std::binary_function<QuadInfo*, QuadInfo*, bool>
	{
		bool operator()(const QuadInfo* _Left, const QuadInfo* _Right) const
		{return (_Left->z > _Right->z);}
	};

	/*************************************************************************
	    Implementation Methods
	*************************************************************************/
	// renders whatever is in the vertex buffer
	void	renderVBuffer(void);

	// sort quads list according to texture
	void	sortQuads(void);

	/*************************************************************************
	    Implementation Data
	*************************************************************************/
	QuadInfo**			d_quadList;
	QuadInfo*			d_quadBuff;
	int					d_quadBuffPos;
	int					d_quadBuffSize;

	LPDIRECT3DDEVICE8		d_device;			//!< Base Direct3DDevice8 interface that we use for rendering
	LPDIRECT3DTEXTURE8		d_currTexture;		//!< currently set texture;
	LPDIRECT3DVERTEXBUFFER8	d_buffer;			//!< vertex buffer to queue sprite rendering
	int						d_bufferPos;		//!< index into buffer where next vertex should be put.
	bool					d_sorted;			//!< true when data in quad list is sorted.

	std::list<DirectX81Texture*>	d_texturelist;		//!< List used to track textures.
};

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////


} // End of  CEGUI namespace section


#endif	// end of guard _DirectX81GUIRenderer_h_
