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

// remove Microsoft idiocy
#undef max
#undef min


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
	d_queueing(true)
{
	d_device->AddRef();

	d_currTexture	= NULL;
	d_buffer		= NULL;
	d_bufferPos		= 0;

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
		// release vertex buffer
		d_buffer->Release();
		throw std::exception("Unable to access required viewport information from Direct3DDevice8.");
	}

	d_display_area.d_left	= 0;
	d_display_area.d_top	= 0;
	d_display_area.d_right	= (float)vp.Width;
	d_display_area.d_bottom	= (float)vp.Height;

	// get the maximum available texture size.
	D3DCAPS8	devCaps;
	if (FAILED(device->GetDeviceCaps(&devCaps)))
	{
		// release vertex buffer
		d_buffer->Release();
		throw std::exception("Unable to retrieve device capabilities from Direct3DDevice8.");
	}

	// set max texture size the the smaller of max width and max height.
	d_maxTextureSize = std::min(devCaps.MaxTextureWidth, devCaps.MaxTextureHeight);
}


/*************************************************************************
	Destructor
*************************************************************************/
DirectX81Renderer::~DirectX81Renderer(void)
{
	d_buffer->Release();
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
		QuadInfo quad;

		quad.position		= dest_rect;
		quad.z				= z;
		quad.texture		= ((DirectX81Texture*)tex)->getD3DTexture();
		quad.texPosition	= texture_rect;
		quad.topLeftCol		= colours.d_bottom_left.getARGB();
		quad.topRightCol	= colours.d_bottom_right.getARGB();
		quad.bottomLeftCol	= colours.d_top_left.getARGB();
		quad.bottomRightCol	= colours.d_top_right.getARGB();

		// offset destination to get correct texel to pixel mapping from Direct3D
		quad.position.offset(Point(-0.5f, -0.5f));

		d_quadlist.insert(quad);
	}

}


/*************************************************************************
	perform final rendering for all queued renderable quads.
*************************************************************************/
void DirectX81Renderer::doRender(void)
{
	d_currTexture = NULL;

	initPerFrameStates();

	bool locked = false;
	QuadVertex*	buffmem;

	// iterate over each quad in the list
	for (QuadList::iterator i = d_quadlist.begin(); i != d_quadlist.end(); ++i)
	{
		const QuadInfo& quad = (*i);

		// flush & set texture if needed
		if (d_currTexture != quad.texture)
		{
			if (locked)
			{
				d_buffer->Unlock();
				locked = false;
			}

			// render any remaining quads for current texture
			renderVBuffer();

			// set new texture
			d_device->SetTexture(0, quad.texture);
			d_currTexture = quad.texture;
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
		buffmem->x = quad.position.d_left;
		buffmem->y = quad.position.d_top;
		buffmem->z = quad.z;
		buffmem->rhw = 1.0f;
		buffmem->diffuse = quad.topLeftCol;
		buffmem->tu1 = quad.texPosition.d_left;
		buffmem->tv1 = quad.texPosition.d_top;
		++buffmem;

		// setup Vertex 2...
		buffmem->x = quad.position.d_right;
		buffmem->y = quad.position.d_top;
		buffmem->z = quad.z;
		buffmem->rhw = 1.0f;
		buffmem->diffuse = quad.topRightCol;
		buffmem->tu1 = quad.texPosition.d_right;
		buffmem->tv1 = quad.texPosition.d_top;
		++buffmem;

		// setup Vertex 3...
		buffmem->x = quad.position.d_left;
		buffmem->y = quad.position.d_bottom;
		buffmem->z = quad.z;
		buffmem->rhw = 1.0f;
		buffmem->diffuse = quad.bottomLeftCol;
		buffmem->tu1 = quad.texPosition.d_left;
		buffmem->tv1 = quad.texPosition.d_bottom;
		++buffmem;

		// setup Vertex 4...
		buffmem->x = quad.position.d_right;
		buffmem->y = quad.position.d_top;
		buffmem->z = quad.z;
		buffmem->rhw = 1.0f;
		buffmem->diffuse = quad.topRightCol;
		buffmem->tu1 = quad.texPosition.d_right;
		buffmem->tv1 = quad.texPosition.d_top;
		++buffmem;

		// setup Vertex 5...
		buffmem->x = quad.position.d_right;
		buffmem->y = quad.position.d_bottom;
		buffmem->z = quad.z;
		buffmem->rhw = 1.0f;
		buffmem->diffuse = quad.bottomRightCol;
		buffmem->tu1 = quad.texPosition.d_right;
		buffmem->tv1 = quad.texPosition.d_bottom;
		++buffmem;

		// setup Vertex 6...
		buffmem->x = quad.position.d_left;
		buffmem->y = quad.position.d_bottom;
		buffmem->z = quad.z;
		buffmem->rhw = 1.0f;
		buffmem->diffuse = quad.bottomLeftCol;
		buffmem->tu1 = quad.texPosition.d_left;
		buffmem->tv1 = quad.texPosition.d_bottom;
		++buffmem;

		// update buffer level
		d_bufferPos += VERTEX_PER_QUAD;

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
	d_quadlist.clear();
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
	// setup vertex stream
	d_device->SetStreamSource(0, d_buffer, sizeof(QuadVertex));
	d_device->SetVertexShader(VERTEX_FVF);

	// set device states
	d_device->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
	d_device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	d_device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	d_device->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	d_device->SetRenderState(D3DRS_FOGENABLE, FALSE);


	// setup texture addressing settings
	d_device->SetTextureStageState(0, D3DTSS_ADDRESSU, D3DTADDRESS_CLAMP);
	d_device->SetTextureStageState(0, D3DTSS_ADDRESSV, D3DTADDRESS_CLAMP);

	// setup colour calculations
	d_device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	d_device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	d_device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);

	// setup alpha calculations
	d_device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	d_device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
	d_device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);

	// setup filtering
	d_device->SetTextureStageState(0, D3DTSS_MINFILTER, D3DTEXF_LINEAR);
	d_device->SetTextureStageState(0, D3DTSS_MAGFILTER, D3DTEXF_LINEAR);

	// setup scene alpha blending
	d_device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	d_device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	d_device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
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
}


/*************************************************************************
	render a quad directly to the display
*************************************************************************/
void DirectX81Renderer::renderQuadDirect(const Rect& dest_rect, float z, const Texture* tex, const Rect& texture_rect, const ColourRect& colours)
{
	// ensure offset destination to ensure proper texel to pixel mapping from D3D.
	Rect final_rect(dest_rect);
	final_rect.offset(Point(-0.5f, -0.5f));

	QuadVertex*	buffmem;

	initPerFrameStates();
	d_device->SetTexture(0, ((DirectX81Texture*)tex)->getD3DTexture());

	if (SUCCEEDED(d_buffer->Lock(0, VERTEX_PER_QUAD * sizeof(QuadVertex), (BYTE**)&buffmem, D3DLOCK_DISCARD)))
	{
		// setup Vertex 1...
		buffmem->x = final_rect.d_left;
		buffmem->y = final_rect.d_top;
		buffmem->z = z;
		buffmem->rhw = 1.0f;
		buffmem->diffuse = colours.d_top_left.getARGB();
		buffmem->tu1 = texture_rect.d_left;
		buffmem->tv1 = texture_rect.d_top;
		++buffmem;

		// setup Vertex 2...
		buffmem->x = final_rect.d_right;
		buffmem->y = final_rect.d_top;
		buffmem->z = z;
		buffmem->rhw = 1.0f;
		buffmem->diffuse = colours.d_top_right.getARGB();
		buffmem->tu1 = texture_rect.d_right;
		buffmem->tv1 = texture_rect.d_top;
		++buffmem;

		// setup Vertex 3...
		buffmem->x = final_rect.d_left;
		buffmem->y = final_rect.d_bottom;
		buffmem->z = z;
		buffmem->rhw = 1.0f;
		buffmem->diffuse = colours.d_bottom_left.getARGB();
		buffmem->tu1 = texture_rect.d_left;
		buffmem->tv1 = texture_rect.d_bottom;
		++buffmem;

		// setup Vertex 4...
		buffmem->x = final_rect.d_right;
		buffmem->y = final_rect.d_top;
		buffmem->z = z;
		buffmem->rhw = 1.0f;
		buffmem->diffuse = colours.d_top_right.getARGB();
		buffmem->tu1 = texture_rect.d_right;
		buffmem->tv1 = texture_rect.d_top;
		++buffmem;

		// setup Vertex 5...
		buffmem->x = final_rect.d_right;
		buffmem->y = final_rect.d_bottom;
		buffmem->z = z;
		buffmem->rhw = 1.0f;
		buffmem->diffuse = colours.d_bottom_right.getARGB();
		buffmem->tu1 = texture_rect.d_right;
		buffmem->tv1 = texture_rect.d_bottom;
		++buffmem;

		// setup Vertex 6...
		buffmem->x = final_rect.d_left;
		buffmem->y = final_rect.d_bottom;
		buffmem->z = z;
		buffmem->rhw = 1.0f;
		buffmem->diffuse = colours.d_bottom_left.getARGB();
		buffmem->tu1 = texture_rect.d_left;
		buffmem->tv1 = texture_rect.d_bottom;

		d_buffer->Unlock();
		d_bufferPos = VERTEX_PER_QUAD;

		renderVBuffer();
	}

}

} // End of  CEGUI namespace section

