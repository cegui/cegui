/************************************************************************
	filename: 	renderer.cpp
	created:	15/3/2004
	author:		Paul D Turner
	
	purpose:	Main source file for Renderer class using DirectX 8.1
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
#include "renderers/directx81GUIRenderer/renderer.h"
#include "renderers/directx81GUIRenderer/texture.h"
#include "CEGUIExceptions.h"

#include <d3dx8.h>
#include <dxerr8.h>
#include <algorithm>


// Start of CEGUI namespace section
namespace CEGUI
{
/*************************************************************************
	Constants definitions
*************************************************************************/
const int			DirectX81Renderer::VERTEX_PER_QUAD			= 6;
const int			DirectX81Renderer::VERTEX_PER_TRIANGLE		= 3;
const int			DirectX81Renderer::VERTEXBUFFER_CAPACITY	= 4096;
const ulong			DirectX81Renderer::VERTEX_FVF				= (D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1);


/*************************************************************************
	Constructor
*************************************************************************/
DirectX81Renderer::DirectX81Renderer(LPDIRECT3DDEVICE8 device, uint max_quads) :
	d_device(device),
	d_quadBuffSize(max_quads),
	d_queueing(true)
{
	d_device->AddRef();

	d_currTexture	= NULL;
	d_buffer		= NULL;
	d_bufferPos		= 0;
	d_sorted		= true;		// no data in quad list, so consider it sorted.

	// Create a vertex buffer
	if (FAILED(d_device->CreateVertexBuffer((VERTEXBUFFER_CAPACITY * sizeof(QuadVertex)), D3DUSAGE_DYNAMIC|D3DUSAGE_WRITEONLY, VERTEX_FVF, D3DPOOL_DEFAULT, &d_buffer)))
	{
		// Ideally, this would have been a RendererException, but we can't use that until the System object is created
		// and that requires a Renderer passed to the constructor, so we throw this instead.
		throw std::exception("Creation of VertexBuffer for Renderer object failed");
	}

	// initialise renderer size
	D3DVIEWPORT8	vp;
	if (FAILED(device->GetViewport(&vp)))
	{
		throw std::exception("Unable to access required viewport information from Direct3DDevice8.");
	}

	d_display_area.d_left	= 0;
	d_display_area.d_top	= 0;
	d_display_area.d_right	= (float)vp.Width;
	d_display_area.d_bottom	= (float)vp.Height;

	// initialise quad buffer
	d_quadBuffPos = 0;
	d_quadBuff = new QuadInfo[max_quads + 1];	// NB: alloc 1 extra QuadInfo to simplify management if we try to overrun
	d_quadList = new QuadInfo*[max_quads];
}


/*************************************************************************
	Destructor
*************************************************************************/
DirectX81Renderer::~DirectX81Renderer(void)
{
	destroyAllTextures();
	d_device->Release();
}


/*************************************************************************
	add's a quad to the list to be rendered
*************************************************************************/
void DirectX81Renderer::addQuad(const Rect& dest_rect, float z, const Texture* tex, const Rect& texture_rect, const ColourRect& colours)
{
	// if not queueing, render directly (as in, right now!)
	if (!d_queueing)
	{
		renderQuadDirect(dest_rect, z, tex, texture_rect, colours);
	}
	else
	{
		if (d_quadBuffPos >= d_quadBuffSize)
		{
			return;
		}

		d_sorted = false;

		QuadInfo& quad = d_quadBuff[d_quadBuffPos];

		quad.position		= dest_rect;
		quad.z				= z;
		quad.texture		= ((DirectX81Texture*)tex)->getD3DTexture();
		quad.texPosition	= texture_rect;
		quad.colours		= colours;

		// offset destination to get correct texel to pixel mapping from Direct3D
		quad.position.offset(Point(-0.5f, -0.5f));

		d_quadList[d_quadBuffPos] = &quad;
		d_quadBuffPos++;
	}

}


/*************************************************************************
	perform final rendering for all queued renderable quads.
*************************************************************************/
void DirectX81Renderer::doRender(void)
{
	d_currTexture = NULL;

	sortQuads();
	initPerFrameStates();

	bool locked = false;
	QuadVertex*	buffmem;

	QuadInfo* quad;

	// iterate over each quad in the list
	for (int i = 0; i < d_quadBuffPos; ++i)
	{
		quad = d_quadList[i];

		// flush & set texture if needed
		if (d_currTexture != quad->texture)
		{
			if (locked)
			{
				d_buffer->Unlock();
				locked = false;
			}

			// render any remaining quads for current texture
			renderVBuffer();

			// set new texture
			d_device->SetTexture(0, quad->texture);
			d_currTexture = quad->texture;
		}

		if (!locked)
		{
			if (FAILED(d_buffer->Lock(0, 0, (BYTE**)&buffmem, D3DLOCK_DISCARD)))
			{
				return;
			}

			locked = true;
		}

		// setup Vertex 1...
		(&buffmem[0])->x	= quad->position.d_left;
		(&buffmem[0])->y	= quad->position.d_top;
		(&buffmem[0])->z	= quad->z;
		(&buffmem[0])->rhw	= 1.0f;
		(&buffmem[0])->diffuse = quad->colours.d_top_left;
		(&buffmem[0])->tu1	= quad->texPosition.d_left;
		(&buffmem[0])->tv1	= quad->texPosition.d_top;

		// setup Vertex 2...
		(&buffmem[1])->x	= quad->position.d_right;
		(&buffmem[1])->y	= quad->position.d_top;
		(&buffmem[1])->z	= quad->z;
		(&buffmem[1])->rhw	= 1.0f;
		(&buffmem[1])->diffuse = quad->colours.d_top_right;
		(&buffmem[1])->tu1	= quad->texPosition.d_right;
		(&buffmem[1])->tv1	= quad->texPosition.d_top;

		// setup Vertex 3...
		(&buffmem[2])->x	= quad->position.d_left;
		(&buffmem[2])->y	= quad->position.d_bottom;
		(&buffmem[2])->z	= quad->z;
		(&buffmem[2])->rhw	= 1.0f;
		(&buffmem[2])->diffuse = quad->colours.d_bottom_left;
		(&buffmem[2])->tu1	= quad->texPosition.d_left;
		(&buffmem[2])->tv1	= quad->texPosition.d_bottom;

		// setup Vertex 4...
		(&buffmem[3])->x	= quad->position.d_right;
		(&buffmem[3])->y	= quad->position.d_top;
		(&buffmem[3])->z	= quad->z;
		(&buffmem[3])->rhw	= 1.0f;
		(&buffmem[3])->diffuse = quad->colours.d_top_right;
		(&buffmem[3])->tu1	= quad->texPosition.d_right;
		(&buffmem[3])->tv1	= quad->texPosition.d_top;

		// setup Vertex 5...
		(&buffmem[4])->x	= quad->position.d_right;
		(&buffmem[4])->y	= quad->position.d_bottom;
		(&buffmem[4])->z	= quad->z;
		(&buffmem[4])->rhw	= 1.0f;
		(&buffmem[4])->diffuse = quad->colours.d_bottom_right;
		(&buffmem[4])->tu1	= quad->texPosition.d_right;
		(&buffmem[4])->tv1	= quad->texPosition.d_bottom;

		// setup Vertex 6...
		(&buffmem[5])->x	= quad->position.d_left;
		(&buffmem[5])->y	= quad->position.d_bottom;
		(&buffmem[5])->z	= quad->z;
		(&buffmem[5])->rhw	= 1.0f;
		(&buffmem[5])->diffuse = quad->colours.d_bottom_left;
		(&buffmem[5])->tu1	= quad->texPosition.d_left;
		(&buffmem[5])->tv1	= quad->texPosition.d_bottom;

		// update position within buffer for next time
		d_bufferPos += VERTEX_PER_QUAD;
		buffmem		+= VERTEX_PER_QUAD;

		// if there is not enough room in the buffer for another sprite, render what we have
		if (d_bufferPos >= (VERTEXBUFFER_CAPACITY - VERTEX_PER_QUAD))
		{
			if (locked)
			{
				d_buffer->Unlock();
				locked = false;
			}

			renderVBuffer();
		}

	}

	if (locked)
	{
		d_buffer->Unlock();
		locked = false;
	}

	renderVBuffer();
}


/*************************************************************************
	clear the queue
*************************************************************************/
void DirectX81Renderer::clearRenderList(void)
{
	d_sorted = true;
	d_quadBuffPos = 0;
}


/*************************************************************************
	create an empty texture
*************************************************************************/
Texture* DirectX81Renderer::createTexture(void)
{
	DirectX81Texture* tex = new DirectX81Texture(this);
	d_texturelist.push_back(tex);
	return tex;
}


/*************************************************************************
	Create a new Texture object and load a file into it.
*************************************************************************/
Texture* DirectX81Renderer::createTexture(const String& filename)
{
	DirectX81Texture* tex = (DirectX81Texture*)createTexture();
	tex->loadFromFile(filename);

	return tex;
}


/*************************************************************************
	Create a new texture with the given dimensions
*************************************************************************/
Texture* DirectX81Renderer::createTexture(float size)
{
	DirectX81Texture* tex = (DirectX81Texture*)createTexture();
	tex->setD3DTextureSize((uint)size);

	return tex;
}

/*************************************************************************
	Destroy a texture
*************************************************************************/
void DirectX81Renderer::destroyTexture(Texture* texture)
{
	if (texture != NULL)
	{
		DirectX81Texture* tex = (DirectX81Texture*)texture;
		d_texturelist.remove(tex);
		delete tex;
	}

}


/*************************************************************************
	destroy all textures still active
*************************************************************************/
void DirectX81Renderer::destroyAllTextures(void)
{
	while (!d_texturelist.empty())
	{
		destroyTexture(*(d_texturelist.begin()));
	}
}


/*************************************************************************
	setup states etc
*************************************************************************/
void DirectX81Renderer::initPerFrameStates(void)
{
	// set standard states
	d_device->SetTextureStageState(0, D3DTSS_COLOROP,   D3DTOP_MODULATE);
	d_device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	d_device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);

	// setup vertex stream
	d_device->SetStreamSource(0, d_buffer, sizeof(QuadVertex));
	d_device->SetVertexShader(VERTEX_FVF);
}


/*************************************************************************
	renders whatever is in the vertex buffer
*************************************************************************/
void DirectX81Renderer::renderVBuffer(void)
{
	// if bufferPos is 0 there is no data in the buffer and nothing to render
	if (d_bufferPos == 0)
	{
		return;
	}

	// render the sprites
	d_device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, (d_bufferPos / VERTEX_PER_TRIANGLE));

	// reset buffer position to 0...
	d_bufferPos = 0;
}


/*************************************************************************
	sort quads list according to texture
*************************************************************************/
void DirectX81Renderer::sortQuads(void)
{
	if (!d_sorted)
	{
		std::stable_sort(d_quadList, d_quadList + d_quadBuffPos, quadsorter());
	}

}


/*************************************************************************
	render a quad directly to the display
*************************************************************************/
void DirectX81Renderer::renderQuadDirect(const Rect& dest_rect, float z, const Texture* tex, const Rect& texture_rect, const ColourRect& colours)
{
	Rect final_rect(dest_rect);
	final_rect.offset(Point(-0.5f, -0.5f));

	QuadVertex*	buffmem;

	initPerFrameStates();
	d_device->SetTexture(0, ((DirectX81Texture*)tex)->getD3DTexture());

	if (SUCCEEDED(d_buffer->Lock(0, VERTEX_PER_QUAD * sizeof(QuadVertex), (BYTE**)&buffmem, D3DLOCK_DISCARD)))
	{
		// setup Vertex 1...
		(&buffmem[0])->x	= final_rect.d_left;
		(&buffmem[0])->y	= final_rect.d_top;
		(&buffmem[0])->z	= z;
		(&buffmem[0])->rhw	= 1.0f;
		(&buffmem[0])->diffuse = colours.d_top_left;
		(&buffmem[0])->tu1	= texture_rect.d_left;
		(&buffmem[0])->tv1	= texture_rect.d_top;

		// setup Vertex 2...
		(&buffmem[1])->x	= final_rect.d_right;
		(&buffmem[1])->y	= final_rect.d_top;
		(&buffmem[1])->z	= z;
		(&buffmem[1])->rhw	= 1.0f;
		(&buffmem[1])->diffuse = colours.d_top_right;
		(&buffmem[1])->tu1	= texture_rect.d_right;
		(&buffmem[1])->tv1	= texture_rect.d_top;

		// setup Vertex 3...
		(&buffmem[2])->x	= final_rect.d_left;
		(&buffmem[2])->y	= final_rect.d_bottom;
		(&buffmem[2])->z	= z;
		(&buffmem[2])->rhw	= 1.0f;
		(&buffmem[2])->diffuse = colours.d_bottom_left;
		(&buffmem[2])->tu1	= texture_rect.d_left;
		(&buffmem[2])->tv1	= texture_rect.d_bottom;

		// setup Vertex 4...
		(&buffmem[3])->x	= final_rect.d_right;
		(&buffmem[3])->y	= final_rect.d_top;
		(&buffmem[3])->z	= z;
		(&buffmem[3])->rhw	= 1.0f;
		(&buffmem[3])->diffuse = colours.d_top_right;
		(&buffmem[3])->tu1	= texture_rect.d_right;
		(&buffmem[3])->tv1	= texture_rect.d_top;

		// setup Vertex 5...
		(&buffmem[4])->x	= final_rect.d_right;
		(&buffmem[4])->y	= final_rect.d_bottom;
		(&buffmem[4])->z	= z;
		(&buffmem[4])->rhw	= 1.0f;
		(&buffmem[4])->diffuse = colours.d_bottom_right;
		(&buffmem[4])->tu1	= texture_rect.d_right;
		(&buffmem[4])->tv1	= texture_rect.d_bottom;

		// setup Vertex 6...
		(&buffmem[5])->x	= final_rect.d_left;
		(&buffmem[5])->y	= final_rect.d_bottom;
		(&buffmem[5])->z	= z;
		(&buffmem[5])->rhw	= 1.0f;
		(&buffmem[5])->diffuse = colours.d_bottom_left;
		(&buffmem[5])->tu1	= texture_rect.d_left;
		(&buffmem[5])->tv1	= texture_rect.d_bottom;

		d_buffer->Unlock();
		d_bufferPos = VERTEX_PER_QUAD;

		renderVBuffer();
	}

}

} // End of  CEGUI namespace section

