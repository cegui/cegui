/***********************************************************************
	filename: 	d3d10renderer.cpp
	created:	17/7/2004
	author:		Paul D Turner with D3D 9 Updates by Magnus Österlind
				D3D10 Port by Rajko Stojadinovic
	
	purpose:	Main source file for Renderer class using DirectX 10.0
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2008 Paul D Turner & The CEGUI Development Team
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
#include "d3d10renderer.h"
#include "d3d10texture.h"
#include "CEGUIExceptions.h"
#include "CEGUISystem.h"

#include <d3dx10.h>
#include <dxerr.h>
#include <algorithm>
#undef min

// Start of CEGUI namespace section
namespace CEGUI
{
//----------------------------------------------------------------------------//
const int DirectX10Renderer::VERTEX_PER_QUAD       = 6;
const int DirectX10Renderer::VERTEX_PER_TRIANGLE   = 3;
const int DirectX10Renderer::VERTEXBUFFER_CAPACITY = 4096;

//----------------------------------------------------------------------------//
const char Shader[] =
"struct VSSceneIn\n\
{    float3 pos          : POSITION;\n\
float4 color        : COLOR;\n\
float2 tex          : TEX0;\n\
};\n\
//viewport params\n\
float    g_viewportHeight;\n\
float    g_viewportWidth;\n\
struct PSSceneIn \n\
{    float4 pos : SV_Position;               //position\n\
float4 color : COLOR0;                  //color\n\
float2 tex : TEXTURE0;                  //texture coordinate\n\
};\n\
DepthStencilState DisableDepth\n\
{    DepthEnable = FALSE;\n\
DepthWriteMask = ZERO;\n\
DepthFunc = ALWAYS;\n\
};\n\
BlendState EnableBlending\n\
{    AlphaToCoverageEnable = FALSE;\n\
BlendEnable[0] = TRUE;\n\
SrcBlend = SRC_ALPHA;\n\
DestBlend = INV_SRC_ALPHA;\n\
};\n\
//// VS for rendering in screen space//\n\
PSSceneIn VSMain(VSSceneIn input)\n\
{    PSSceneIn output = (PSSceneIn)0.0;\n\
//output our final position\n\
output.pos.x = (input.pos.x / (g_viewportWidth/2.0)) -1;\n\
output.pos.y = -(input.pos.y / (g_viewportHeight/2.0)) +1;\n\
output.pos.z = input.pos.z;\n\
output.pos.w = 1;\n\
//propogate texture coordinate\n\
output.tex = input.tex;\n\
output.color.rgba = input.color.bgra;\n\
return output;\n\
}\n\
Texture2D DiffuseTex;\n\
SamplerState LinearSampler\n\
{    Filter = MIN_MAG_MIP_LINEAR;\n\
AddressU = Clamp;\n\
AddressV = Clamp;\n\
};\n\
//// PS for rendering//\n\
float4 PSMain(PSSceneIn input) : SV_Target\n\
{\n\
	float4 color =  DiffuseTex.Sample( LinearSampler, input.tex ) * input.color;\n\
	return color;\n\
}\n\
RasterizerState rasterstate\n\
{DepthClipEnable = false;\n\
FillMode = Solid;};\n\
//// Technique//\n\
technique10 Render{    \n\
	pass P0    {        \n\
		SetVertexShader( CompileShader( vs_4_0, VSMain() ) );\n\
		SetGeometryShader( NULL );\n\
		SetPixelShader( CompileShader( ps_4_0, PSMain() ) );\n\
		SetDepthStencilState(DisableDepth, 0 );\n\
		SetBlendState( EnableBlending, float4( 0.0f, 0.0f, 0.0f, 0.0f ), 0xFFFFFFFF );\n\
		SetRasterizerState(rasterstate);    \n\
	}\n\
}\n";

//----------------------------------------------------------------------------//
DirectX10Renderer::DirectX10Renderer(LPDIRECT3DDEVICE device, uint max_quads)
{
	d_device = device;
	Size size(getViewportSize());

	constructor_impl(device, size);
}

//----------------------------------------------------------------------------//
void DirectX10Renderer::constructor_impl(LPDIRECT3DDEVICE device,
    const Size& display_size)
{
	d_device        = device;
	d_queueing      = true;
	d_currTexture   = 0;
	d_currSRView    = 0;
	d_buffer        = 0;
	d_bufferPos     = 0;

	// initialise renderer display area
	d_display_area.d_left	= 0;
	d_display_area.d_top	= 0;
	d_display_area.setSize(display_size);

	if (FAILED(CreateVertexBuffer()))
	{
		// Ideally, this would have been a RendererException, but we can't use
        // that until the System object is created and that requires a Renderer
        // passed to the constructor, so we throw this instead.
		throw std::exception("Creation of VertexBuffer for Renderer object failed");
	}


	// set max texture size the the smaller of max width and max height.
	d_maxTextureSize = 4096;

	d_device->AddRef();

    // set ID string
    d_identifierString = "CEGUI::DirectX10Renderer - Official Direct3D 10 "
                         "based renderer module for CEGUI";

	ID3D10Blob* Errors = 0;
	HRESULT hrz = D3DX10CreateEffectFromMemory(Shader, sizeof(Shader), 0, 0, 0,
                                               "fx_4_0", 0, 0, d_device, 0, 0,
                                               &d_Effect, &Errors, 0);
	std::string error;

    if (FAILED(hrz))
	{
		error = std::string((char*)Errors->GetBufferPointer(),Errors->GetBufferSize());
		throw std::exception(error.c_str());
	}

	d_Technique = d_Effect->GetTechniqueByName("Render");
	d_DiffuseTexVariable = d_Effect->GetVariableByName("DiffuseTex")->AsShaderResource();
	d_WindowHeight = d_Effect->GetVariableByName("g_viewportHeight")->AsScalar();
	d_WindowWidth = d_Effect->GetVariableByName("g_viewportWidth")->AsScalar();

	const D3D10_INPUT_ELEMENT_DESC VertexLayout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  0, D3D10_INPUT_PER_VERTEX_DATA, 0 }, 
		{ "COLOR",    0, DXGI_FORMAT_R8G8B8A8_UNORM,  0, 12, D3D10_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEX",	  0, DXGI_FORMAT_R32G32_FLOAT,	  0, 16, D3D10_INPUT_PER_VERTEX_DATA, 0 },		
	};

	UINT numElements = sizeof(VertexLayout)/sizeof(VertexLayout[0]);

	// Create the input layout
	D3D10_PASS_DESC PassDesc;
	d_Technique->GetPassByIndex( 0 )->GetDesc( &PassDesc );
	hrz = d_device->CreateInputLayout(VertexLayout, numElements,
                                      PassDesc.pIAInputSignature,
                                      PassDesc.IAInputSignatureSize,
                                      &d_InputLayout );
	if (FAILED( hrz ))
	{
		throw std::exception("Failed to create Input Layout");
	}

	d_device->IASetInputLayout( d_InputLayout );
}

//----------------------------------------------------------------------------//
DirectX10Renderer::~DirectX10Renderer(void)
{
	if (d_buffer)
		d_buffer->Release();

    if (d_Effect)
		d_Effect->Release();

    if (d_InputLayout)
		d_InputLayout->Release();

    destroyAllTextures();

	if (d_device)
		d_device->Release();
}

//----------------------------------------------------------------------------//
void DirectX10Renderer::addQuad(const Rect& dest_rect, float z,
    const Texture* tex, const Rect& texture_rect, const ColourRect& colours,
    QuadSplitMode quad_split_mode)
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
		quad.texture		= ((DirectX10Texture*)tex)->getD3DTexture();
		quad.resview		= ((DirectX10Texture*)tex)->getD3DResourceView();
		quad.texPosition	= texture_rect;
		quad.topLeftCol		= colours.d_top_left.getARGB();
		quad.topRightCol	= colours.d_top_right.getARGB();
		quad.bottomLeftCol	= colours.d_bottom_left.getARGB();
		quad.bottomRightCol	= colours.d_bottom_right.getARGB();

        // set quad split mode
        quad.splitMode = quad_split_mode;

		d_quadlist.insert(quad);
	}
}

//----------------------------------------------------------------------------//
void DirectX10Renderer::doRender(void)
{
	d_currTexture = 0;
	d_currSRView = 0;

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
				d_buffer->Unmap();
				locked = false;
			}

			// render any remaining quads for current texture
			renderVBuffer(d_currSRView);

			d_currTexture = quad.texture;
			d_currSRView = quad.resview;
		}

		if (!locked)
		{
			if (FAILED(d_buffer->Map(D3D10_MAP_WRITE_DISCARD, 0, (void**)&buffmem)))
				return;

			locked = true;
		}

		// setup Vertex 1...
		buffmem->x = quad.position.d_left;
		buffmem->y = quad.position.d_top;
		buffmem->z = quad.z;
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
            buffmem->diffuse = quad.topRightCol;
            buffmem->tu1 = quad.texPosition.d_right;
            buffmem->tv1 = quad.texPosition.d_top;
        }
        ++buffmem;

		// setup Vertex 3...
		buffmem->x = quad.position.d_left;
		buffmem->y = quad.position.d_bottom;
		buffmem->z = quad.z;
		buffmem->diffuse = quad.bottomLeftCol;
		buffmem->tu1 = quad.texPosition.d_left;
		buffmem->tv1 = quad.texPosition.d_bottom;
		++buffmem;

		// setup Vertex 4...
		buffmem->x = quad.position.d_right;
		buffmem->y = quad.position.d_top;
		buffmem->z = quad.z;
		buffmem->diffuse = quad.topRightCol;
		buffmem->tu1 = quad.texPosition.d_right;
		buffmem->tv1 = quad.texPosition.d_top;
		++buffmem;

		// setup Vertex 5...
		buffmem->x = quad.position.d_right;
		buffmem->y = quad.position.d_bottom;
		buffmem->z = quad.z;
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
            buffmem->diffuse = quad.bottomLeftCol;
            buffmem->tu1 = quad.texPosition.d_left;
            buffmem->tv1 = quad.texPosition.d_bottom;
        }
        ++buffmem;

		// update buffer level
		d_bufferPos += VERTEX_PER_QUAD;

		// if there is not enough room in the buffer for another sprite,
        // render what we have so far.
		if (d_bufferPos >= (VERTEXBUFFER_CAPACITY - VERTEX_PER_QUAD))
		{
			if (locked)
			{
				d_buffer->Unmap();
				locked = false;
			}

			renderVBuffer(d_currSRView);
		}
	}

	if (locked)
	{
		d_buffer->Unmap();
		locked = false;
	}

	renderVBuffer(d_currSRView);
}

//----------------------------------------------------------------------------//
void DirectX10Renderer::clearRenderList(void)
{
	d_quadlist.clear();
}

//----------------------------------------------------------------------------//
Texture* DirectX10Renderer::createTexture(void)
{
	DirectX10Texture* tex = new DirectX10Texture(this);
	d_texturelist.push_back(tex);
	return tex;
}

//----------------------------------------------------------------------------//
Texture* DirectX10Renderer::createTexture(const String& filename,
    const String& resourceGroup)
{
	DirectX10Texture* tex = new DirectX10Texture(this);
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

//----------------------------------------------------------------------------//
Texture* DirectX10Renderer::createTexture(float size)
{
	DirectX10Texture* tex = new DirectX10Texture(this);
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

//----------------------------------------------------------------------------//
void DirectX10Renderer::destroyTexture(Texture* texture)
{
	if (texture)
	{
		DirectX10Texture* tex = (DirectX10Texture*)texture;
		d_texturelist.remove(tex);
		delete tex;
	}
}

//----------------------------------------------------------------------------//
void DirectX10Renderer::destroyAllTextures(void)
{
	while (!d_texturelist.empty())
		destroyTexture(*(d_texturelist.begin()));
}

//----------------------------------------------------------------------------//
void DirectX10Renderer::renderVBuffer(ID3D10ShaderResourceView *tex)
{
	// if bufferPos is 0 there is no data in the buffer and nothing to render
	if (d_bufferPos == 0)
		return;

	d_device->IASetInputLayout(d_InputLayout);

	// Set IA parameters
	UINT strides[1] = { sizeof( QuadVertex ) };
	UINT offsets[1] = {0};
	ID3D10Buffer* pBuffers[1] = { d_buffer };

	d_device->IASetVertexBuffers( 0, 1, pBuffers, strides, offsets );
	d_device->IASetPrimitiveTopology( D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

	d_WindowHeight->SetFloat(getHeight());
	d_WindowWidth->SetFloat(getWidth());

	D3D10_TECHNIQUE_DESC techDesc;
	d_Technique->GetDesc( &techDesc );

	for( UINT p = 0; p < techDesc.Passes; ++p )
	{
		d_DiffuseTexVariable->SetResource( tex );

		d_Technique->GetPassByIndex( p )->Apply(0);
		d_device->Draw(d_bufferPos,0);
	}

	// reset buffer position to 0...
	d_bufferPos = 0;
}

//----------------------------------------------------------------------------//
void DirectX10Renderer::renderQuadDirect(const Rect& dest_rect, float z,
    const Texture* tex, const Rect& texture_rect, const ColourRect& colours,
    QuadSplitMode quad_split_mode)
{
	// ensure offset destination to ensure proper texel to pixel mapping from D3D.
	Rect final_rect(dest_rect);

	QuadVertex*	buffmem;

	if (SUCCEEDED(d_buffer->Map(D3D10_MAP_WRITE_DISCARD,0, (void**)&buffmem)))
	{
		// setup Vertex 1...
		buffmem->x = final_rect.d_left;
		buffmem->y = final_rect.d_top;
		buffmem->z = z;
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
            buffmem->diffuse = colours.d_top_right.getARGB();
            buffmem->tu1 = texture_rect.d_right;
            buffmem->tv1 = texture_rect.d_top;
        }
        ++buffmem;

		// setup Vertex 3...
		buffmem->x = final_rect.d_left;
		buffmem->y = final_rect.d_bottom;
		buffmem->z = z;
		buffmem->diffuse = colours.d_bottom_left.getARGB();
		buffmem->tu1 = texture_rect.d_left;
		buffmem->tv1 = texture_rect.d_bottom;
		++buffmem;

		// setup Vertex 4...
		buffmem->x = final_rect.d_right;
		buffmem->y = final_rect.d_top;
		buffmem->z = z;
		buffmem->diffuse = colours.d_top_right.getARGB();
		buffmem->tu1 = texture_rect.d_right;
		buffmem->tv1 = texture_rect.d_top;
		++buffmem;

		// setup Vertex 5...
		buffmem->x = final_rect.d_right;
		buffmem->y = final_rect.d_bottom;
		buffmem->z = z;
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
            buffmem->diffuse = colours.d_bottom_left.getARGB();
            buffmem->tu1 = texture_rect.d_left;
            buffmem->tv1 = texture_rect.d_bottom;
        }

		d_buffer->Unmap();
		d_bufferPos = VERTEX_PER_QUAD;

		renderVBuffer(((DirectX10Texture*)tex)->getD3DResourceView());
	}

}

//----------------------------------------------------------------------------//
Size DirectX10Renderer::getViewportSize(void)
{
	// initialise renderer size
	D3D10_VIEWPORT viewports[1];
	UINT numviewports = 1;

	d_device->RSGetViewports(&numviewports,viewports);
	return Size((float)viewports[0].Width, (float)viewports[0].Height);
}

//----------------------------------------------------------------------------//
void DirectX10Renderer::setDisplaySize(const Size& sz)
{
	if (d_display_area.getSize() != sz)
	{
		d_display_area.setSize(sz);

		EventArgs args;
		fireEvent(EventDisplaySizeChanged, args, EventNamespace);
	}
}

//----------------------------------------------------------------------------//
HRESULT DirectX10Renderer::CreateVertexBuffer()
{
    D3D10_BUFFER_DESC BufferDescription;

    BufferDescription.Usage          = D3D10_USAGE_DYNAMIC;
    BufferDescription.ByteWidth      = (VERTEXBUFFER_CAPACITY * sizeof(QuadVertex));
    BufferDescription.BindFlags      = D3D10_BIND_VERTEX_BUFFER;
    BufferDescription.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;
    BufferDescription.MiscFlags      = 0;	 

    return d_device->CreateBuffer(&BufferDescription,0,&d_buffer);
}

//----------------------------------------------------------------------------//

} // End of  CEGUI namespace section

