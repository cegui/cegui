/***********************************************************************
	filename: 	renderer.cpp
	created:	15/3/2004
	author:		Paul D Turner
	
	purpose:	Main source file for Renderer class using DirectX 8.1
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
 **************************************************************************/
#include "renderer.h"
#include "texture.h"
#include "CEGUIExceptions.h"
#include "CEGUISystem.h"

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
DirectX81Renderer::DirectX81Renderer(LPDIRECT3DDEVICE8 device, uint max_quads)
{
	d_canGetVPSize = true;
	d_device = device;

	Size size;

	try
	{
		size = getViewportSize();
	}
	catch (std::exception&)
	{
		// we'll allow things to continue here, and assume that the user will
		// be calling DirectX81Renderer::setDisplaySize afterwards.
		size.d_height = size.d_width = 0.0f;
	}

	constructor_impl(device, size);
}


/*************************************************************************
	Constructor
*************************************************************************/
DirectX81Renderer::DirectX81Renderer(LPDIRECT3DDEVICE8 device, const Size& sz)
{
	d_canGetVPSize = true;
	constructor_impl(device, sz);
}


/*************************************************************************
	Method to do common work for the constructor
*************************************************************************/
void DirectX81Renderer::constructor_impl(LPDIRECT3DDEVICE8 device, const Size& display_size)
{
	d_device        = device;
	d_queueing      = true;
	d_currTexture   = 0;
	d_buffer        = 0;
	d_bufferPos     = 0;

	// initialise renderer display area
	d_display_area.d_left = 0;
	d_display_area.d_top  = 0;
	d_display_area.setSize(display_size);

	// Create a vertex buffer
	if (FAILED(d_device->CreateVertexBuffer((VERTEXBUFFER_CAPACITY * sizeof(QuadVertex)), D3DUSAGE_DYNAMIC|D3DUSAGE_WRITEONLY, VERTEX_FVF, D3DPOOL_DEFAULT, &d_buffer)))
	{
		// Ideally, this would have been a RendererException, but we can't use that until the System object is created
		// and that requires a Renderer passed to the constructor, so we throw this instead.
		throw std::exception("Creation of VertexBuffer for Renderer object failed");
	}

	// get the maximum available texture size.
	D3DCAPS8	devCaps;
	if (FAILED(device->GetDeviceCaps(&devCaps)))
	{
		// release vertex buffer
		d_buffer->Release();
		throw std::exception("Unable to retrieve device capabilities from Direct3DDevice8.");
	}

	// set max texture size the the smaller of max width and max height.
	d_maxTextureSize = ceguimin(devCaps.MaxTextureWidth, devCaps.MaxTextureHeight);

	d_device->AddRef();

    // set ID string
    d_identifierString = "CEGUI::DirectX81Renderer - Official Direct3D 8.1 based renderer module for CEGUI";
}


/*************************************************************************
	Destructor
*************************************************************************/
DirectX81Renderer::~DirectX81Renderer(void)
{
	if (d_buffer)
	{
		d_buffer->Release();
	}
	
	destroyAllTextures();
	
	if (d_device)
	{
		d_device->Release();
	}
}


/*************************************************************************
	add's a quad to the list to be rendered
*************************************************************************/
void DirectX81Renderer::addQuad(const Rect& dest_rect, float z, const Texture* tex, const Rect& texture_rect, const ColourRect& colours, QuadSplitMode quad_split_mode)
{
	// if not queueing, render directly (as in, right now!)
	if (!d_queueing)
	{
		renderQuadDirect(dest_rect, z, tex, texture_rect, colours, quad_split_mode);
	}
	else
	{
		QuadInfo quad;

		quad.position		= dest_rect;
		quad.z				= z;
		quad.texture		= ((DirectX81Texture*)tex)->getD3DTexture();
		quad.texPosition	= texture_rect;
		quad.topLeftCol		= colours.d_top_left.getARGB();
		quad.topRightCol	= colours.d_top_right.getARGB();
		quad.bottomLeftCol	= colours.d_bottom_left.getARGB();
		quad.bottomRightCol	= colours.d_bottom_right.getARGB();

		// offset destination to get correct texel to pixel mapping from Direct3D
		quad.position.offset(Point(-0.5f, -0.5f));

        // set quad split mode
        quad.splitMode = quad_split_mode;

		d_quadlist.insert(quad);
	}

}


/*************************************************************************
	perform final rendering for all queued renderable quads.
*************************************************************************/
void DirectX81Renderer::doRender(void)
{
	d_currTexture = 0;

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

        // top-left to bottom-right diagonal
        if (quad.splitMode == TopLeftToBottomRight)
        {
            buffmem->x = quad.position.d_right;
            buffmem->y = quad.position.d_bottom;
            buffmem->z = quad.z;
            buffmem->rhw = 1.0f;
            buffmem->diffuse = quad.bottomRightCol;
            buffmem->tu1 = quad.texPosition.d_right;
            buffmem->tv1 = quad.texPosition.d_bottom;
        }
        // bottom-left to top-right diagonal
        else
        {
            buffmem->x = quad.position.d_right;
            buffmem->y = quad.position.d_top;
            buffmem->z = quad.z;
            buffmem->rhw = 1.0f;
            buffmem->diffuse = quad.topRightCol;
            buffmem->tu1 = quad.texPosition.d_right;
            buffmem->tv1 = quad.texPosition.d_top;
        }
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

        // top-left to bottom-right diagonal
        if (quad.splitMode == TopLeftToBottomRight)
        {
            buffmem->x = quad.position.d_left;
            buffmem->y = quad.position.d_top;
            buffmem->z = quad.z;
            buffmem->rhw = 1.0f;
            buffmem->diffuse = quad.topLeftCol;
            buffmem->tu1 = quad.texPosition.d_left;
            buffmem->tv1 = quad.texPosition.d_top;
        }
        // bottom-left to top-right diagonal
        else
        {
            buffmem->x = quad.position.d_left;
            buffmem->y = quad.position.d_bottom;
            buffmem->z = quad.z;
            buffmem->rhw = 1.0f;
            buffmem->diffuse = quad.bottomLeftCol;
            buffmem->tu1 = quad.texPosition.d_left;
            buffmem->tv1 = quad.texPosition.d_bottom;
        }
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
Texture* DirectX81Renderer::createTexture(const String& filename, const String& resourceGroup)
{
    DirectX81Texture* tex = new DirectX81Texture(this);
    try
    {
        tex->loadFromFile(filename, resourceGroup);
    }
    catch (RendererException&)
    {
        delete tex;
        throw;
    }
    d_texturelist.push_back(tex);
    return tex;
}


/*************************************************************************
	Create a new texture with the given dimensions
*************************************************************************/
Texture* DirectX81Renderer::createTexture(float size)
{
    DirectX81Texture* tex = new DirectX81Texture(this);
    try
    {
        tex->setD3DTextureSize((uint)size);
    }
    catch (RendererException&)
    {
        delete tex;
        throw;
    }
    d_texturelist.push_back(tex);
    return tex;
}

/*************************************************************************
	Destroy a texture
*************************************************************************/
void DirectX81Renderer::destroyTexture(Texture* texture)
{
	if (texture)
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
    d_device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);


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

    // disable texture stages we do not need.
    d_device->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);

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
void DirectX81Renderer::renderQuadDirect(const Rect& dest_rect, float z, const Texture* tex, const Rect& texture_rect, const ColourRect& colours, QuadSplitMode quad_split_mode)
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

        // top-left to bottom-right diagonal
        if (quad_split_mode == TopLeftToBottomRight)
        {
            buffmem->x = final_rect.d_right;
            buffmem->y = final_rect.d_bottom;
            buffmem->z = z;
            buffmem->rhw = 1.0f;
            buffmem->diffuse = colours.d_bottom_right.getARGB();
            buffmem->tu1 = texture_rect.d_right;
            buffmem->tv1 = texture_rect.d_bottom;
        }
        // bottom-left to top-right diagonal
        else
        {
            buffmem->x = final_rect.d_right;
            buffmem->y = final_rect.d_top;
            buffmem->z = z;
            buffmem->rhw = 1.0f;
            buffmem->diffuse = colours.d_top_right.getARGB();
            buffmem->tu1 = texture_rect.d_right;
            buffmem->tv1 = texture_rect.d_top;
        }
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

        // top-left to bottom-right diagonal
        if (quad_split_mode == TopLeftToBottomRight)
        {
            buffmem->x = final_rect.d_left;
            buffmem->y = final_rect.d_top;
            buffmem->z = z;
            buffmem->rhw = 1.0f;
            buffmem->diffuse = colours.d_top_left.getARGB();
            buffmem->tu1 = texture_rect.d_left;
            buffmem->tv1 = texture_rect.d_top;
        }
        // bottom-left to top-right diagonal
        else
        {
            buffmem->x = final_rect.d_left;
            buffmem->y = final_rect.d_bottom;
            buffmem->z = z;
            buffmem->rhw = 1.0f;
            buffmem->diffuse = colours.d_bottom_left.getARGB();
            buffmem->tu1 = texture_rect.d_left;
            buffmem->tv1 = texture_rect.d_bottom;
        }

		d_buffer->Unlock();
		d_bufferPos = VERTEX_PER_QUAD;

		renderVBuffer();
	}

}


/*************************************************************************
	Direct3D support method that must be called prior to a Reset call
	on the Direct3DDevice.
*************************************************************************/
void DirectX81Renderer::preD3DReset(void)
{
	// release the buffer prior to the reset call (will be re-created later)
	if (FAILED(d_buffer->Release()))
	{
		throw RendererException("DirectX81Renderer::preD3DReset - Failed to release the VertexBuffer used by the DirectX81Renderer object.");
	}

	d_buffer = 0;

	// perform pre-reset operations on all textures
	std::list<DirectX81Texture*>::iterator ctex = d_texturelist.begin();
	std::list<DirectX81Texture*>::iterator endtex = d_texturelist.end();

	for (; ctex != endtex; ++ctex)
	{
		(*ctex)->preD3DReset();
	}

}


/*************************************************************************
	Direct3D support method that must be called after a Reset call on the
	Direct3DDevice.
*************************************************************************/
void DirectX81Renderer::postD3DReset(void)
{
	// Recreate a vertex buffer
	if (FAILED(d_device->CreateVertexBuffer((VERTEXBUFFER_CAPACITY * sizeof(QuadVertex)), D3DUSAGE_DYNAMIC|D3DUSAGE_WRITEONLY, VERTEX_FVF, D3DPOOL_DEFAULT, &d_buffer)))
	{
		throw RendererException("DirectX81Renderer::preD3DReset - Failed to create the VertexBuffer for use by the DirectX81Renderer object.");
	}

	// perform post-reset operations on all textures
	std::list<DirectX81Texture*>::iterator ctex = d_texturelist.begin();
	std::list<DirectX81Texture*>::iterator endtex = d_texturelist.end();

	for (; ctex != endtex; ++ctex)
	{
		(*ctex)->postD3DReset();
	}

	// update size of display (if we can)
	if (d_canGetVPSize)
	{
		try
		{
			setDisplaySize(getViewportSize());
		}
		catch (std::exception&)
		{
			// Do nothing here, DirectX81Renderer::getViewportSize has set a flag so we never try to do this again!
			// The user must call DirectX81Renderer::setDisplaySize to ensure correct re-sizing of the view.
		}
	}

	// Now we've come back, we MUST ensure a full redraw is done since the
	// textures in the stored quads will have been invalidated.
	System::getSingleton().signalRedraw();
}


/*************************************************************************
	return size of device view port (if possible)	
*************************************************************************/
Size DirectX81Renderer::getViewportSize(void)
{
	// initialise renderer size
	D3DVIEWPORT8	vp;

	if (FAILED(d_device->GetViewport(&vp)))
	{
		d_canGetVPSize = false;
		throw std::exception("Unable to access required view port information from Direct3DDevice8.");
	}
	else
	{
		return Size((float)vp.Width, (float)vp.Height);
	}

}


/*************************************************************************
	Set the size of the display in pixels.	
*************************************************************************/
void DirectX81Renderer::setDisplaySize(const Size& sz)
{
	if (d_display_area.getSize() != sz)
	{
		d_display_area.setSize(sz);

		EventArgs args;
		fireEvent(EventDisplaySizeChanged, args, EventNamespace);
	}

}

} // End of  CEGUI namespace section

