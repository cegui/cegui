/***********************************************************************
    created:    Wed May 5 2010
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2011 Paul D Turner & The CEGUI Development Team
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
#include "CEGUI/RendererModules/Direct3D11/Renderer.h"
#include "CEGUI/RendererModules/Direct3D11/GeometryBuffer.h"
#include "CEGUI/RendererModules/Direct3D11/TextureTarget.h"
#include "CEGUI/RendererModules/Direct3D11/ViewportTarget.h"
#include "CEGUI/RendererModules/Direct3D11/Texture.h"
#include "CEGUI/Exceptions.h"
#include "CEGUI/System.h"
#include "CEGUI/DefaultResourceProvider.h"
#include "CEGUI/Logger.h"
#include <algorithm>

#include "d3dx11effect.h"

#include "shader.txt"

// Start of CEGUI namespace section
namespace CEGUI
{
//----------------------------------------------------------------------------//
String Direct3D11Renderer::d_rendererID(
"CEGUI::Direct3D11Renderer - Official Direct3D 11 based 2nd generation renderer"
" module.");

//----------------------------------------------------------------------------//
Direct3D11Renderer& Direct3D11Renderer::bootstrapSystem(
                                                   ID3D11Device* device,
                                                   ID3D11DeviceContext* context,
                                                   const int abi)
{
    System::performVersionTest(CEGUI_VERSION_ABI, abi, CEGUI_FUNCTION_NAME);

    if (System::getSingletonPtr())
        CEGUI_THROW(InvalidRequestException(
            "CEGUI::System object is already initialised."));

    Direct3D11Renderer& renderer(create(device, context));
    DefaultResourceProvider* rp = new CEGUI::DefaultResourceProvider();
    System::create(renderer, rp);

    return renderer;
}

//----------------------------------------------------------------------------//
void Direct3D11Renderer::destroySystem()
{
    System* sys;
    if (!(sys = System::getSingletonPtr()))
        CEGUI_THROW(InvalidRequestException(
            "CEGUI::System object is not created or was already destroyed."));

    Direct3D11Renderer* renderer =
        static_cast<Direct3D11Renderer*>(sys->getRenderer());
    DefaultResourceProvider* rp =
        static_cast<DefaultResourceProvider*>(sys->getResourceProvider());

    System::destroy();
    delete rp;
    destroy(*renderer);
}

//----------------------------------------------------------------------------//
Direct3D11Renderer& Direct3D11Renderer::create(ID3D11Device* device,
                                               ID3D11DeviceContext* context,
                                               const int abi)
{
    System::performVersionTest(CEGUI_VERSION_ABI, abi, CEGUI_FUNCTION_NAME);

    return *new Direct3D11Renderer(device,context);
}

//----------------------------------------------------------------------------//
void Direct3D11Renderer::destroy(Direct3D11Renderer& renderer)
{
    delete &renderer;
}

//----------------------------------------------------------------------------//
RenderTarget& Direct3D11Renderer::getDefaultRenderTarget()
{
    return *d_defaultTarget;
}

//----------------------------------------------------------------------------//
GeometryBuffer& Direct3D11Renderer::createGeometryBuffer()
{
    Direct3D11GeometryBuffer* b = new Direct3D11GeometryBuffer(*this);
    d_geometryBuffers.push_back(b);
    return *b;
}

//----------------------------------------------------------------------------//
void Direct3D11Renderer::destroyGeometryBuffer(const GeometryBuffer& buffer)
{
    GeometryBufferList::iterator i = std::find(d_geometryBuffers.begin(),
                                               d_geometryBuffers.end(),
                                               &buffer);

    if (d_geometryBuffers.end() != i)
    {
        d_geometryBuffers.erase(i);
        delete &buffer;
    }
}

//----------------------------------------------------------------------------//
void Direct3D11Renderer::destroyAllGeometryBuffers()
{
    while (!d_geometryBuffers.empty())
        destroyGeometryBuffer(**d_geometryBuffers.begin());
}

//----------------------------------------------------------------------------//
TextureTarget* Direct3D11Renderer::createTextureTarget()
{
    TextureTarget* t = new Direct3D11TextureTarget(*this);
    d_textureTargets.push_back(t);
    return t;
}

//----------------------------------------------------------------------------//
void Direct3D11Renderer::destroyTextureTarget(TextureTarget* target)
{
    TextureTargetList::iterator i = std::find(d_textureTargets.begin(),
                                              d_textureTargets.end(),
                                              target);

    if (d_textureTargets.end() != i)
    {
        d_textureTargets.erase(i);
        delete target;
    }
}

//----------------------------------------------------------------------------//
void Direct3D11Renderer::destroyAllTextureTargets()
{
    while (!d_textureTargets.empty())
        destroyTextureTarget(*d_textureTargets.begin());
}

//----------------------------------------------------------------------------//
Texture& Direct3D11Renderer::createTexture(const String& name)
{
    throwIfNameExists(name);

    Direct3D11Texture* tex = new Direct3D11Texture(d_device, name);
    d_textures[name] = tex;

    logTextureCreation(name);

    return *tex;
}

//----------------------------------------------------------------------------//
Texture& Direct3D11Renderer::createTexture(const String& name,
                                           const String& filename,
                                           const String& resourceGroup)
{
    throwIfNameExists(name);

    Direct3D11Texture* tex = new Direct3D11Texture(d_device, name, filename,
                                                   resourceGroup);
    d_textures[name] = tex;

    logTextureCreation(name);

    return *tex;
}

//----------------------------------------------------------------------------//
Texture& Direct3D11Renderer::createTexture(const String& name,
                                           const Sizef& size)
{
    throwIfNameExists(name);

    Direct3D11Texture* tex = new Direct3D11Texture(d_device, name, size);
    d_textures[name] = tex;

    logTextureCreation(name);

    return *tex;
}

//----------------------------------------------------------------------------//
void Direct3D11Renderer::throwIfNameExists(const String& name) const
{
    if (d_textures.find(name) != d_textures.end())
        CEGUI_THROW(AlreadyExistsException(
            "[Direct3D11Renderer] Texture already exists: " + name));
}

//----------------------------------------------------------------------------//
void Direct3D11Renderer::logTextureCreation(const String& name)
{
    Logger* logger = Logger::getSingletonPtr();
    if (logger)
        logger->logEvent("[Direct3D11Renderer] Created texture: " + name);
}

//----------------------------------------------------------------------------//
void Direct3D11Renderer::destroyTexture(Texture& texture)
{
    destroyTexture(texture.getName());
}

//----------------------------------------------------------------------------//
void Direct3D11Renderer::destroyTexture(const String& name)
{
    TextureMap::iterator i = d_textures.find(name);

    if (d_textures.end() != i)
    {
        logTextureDestruction(name);
        delete i->second;
        d_textures.erase(i);
    }
}

//----------------------------------------------------------------------------//
void Direct3D11Renderer::logTextureDestruction(const String& name)
{
    Logger* logger = Logger::getSingletonPtr();
    if (logger)
        logger->logEvent("[Direct3D11Renderer] Destroyed texture: " + name);
}

//----------------------------------------------------------------------------//
void Direct3D11Renderer::destroyAllTextures()
{
    while (!d_textures.empty())
        destroyTexture(d_textures.begin()->first);
}

//----------------------------------------------------------------------------//
Texture& Direct3D11Renderer::getTexture(const String& name) const
{
    TextureMap::const_iterator i = d_textures.find(name);
    
    if (i == d_textures.end())
        CEGUI_THROW(UnknownObjectException("Texture does not exist: " + name));

    return *i->second;
}

//----------------------------------------------------------------------------//
bool Direct3D11Renderer::isTextureDefined(const String& name) const
{
    return d_textures.find(name) != d_textures.end();
}

//----------------------------------------------------------------------------//
void Direct3D11Renderer::beginRendering()
{
    d_device.d_context->IASetInputLayout(d_inputLayout);
    d_device.d_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

//----------------------------------------------------------------------------//
void Direct3D11Renderer::endRendering()
{
}

//----------------------------------------------------------------------------//
void Direct3D11Renderer::setDisplaySize(const Sizef& sz)
{
    if (sz != d_displaySize)
    {
        d_displaySize = sz;

        // FIXME: This is probably not the right thing to do in all cases.
        Rectf area(d_defaultTarget->getArea());
        area.setSize(sz);
        d_defaultTarget->setArea(area);
    }

}

//----------------------------------------------------------------------------//
const Sizef& Direct3D11Renderer::getDisplaySize() const
{
    return d_displaySize;
}

//----------------------------------------------------------------------------//
const Vector2f& Direct3D11Renderer::getDisplayDPI() const
{
    return d_displayDPI;
}

//----------------------------------------------------------------------------//
uint Direct3D11Renderer::getMaxTextureSize() const
{
    return 8192;//DIRECTX11 may support even 16384, but most users will use it as feauture level 10, so keep it for now
}

//----------------------------------------------------------------------------//
const String& Direct3D11Renderer::getIdentifierString() const
{
    return d_rendererID;
}

//----------------------------------------------------------------------------//
Direct3D11Renderer::Direct3D11Renderer(ID3D11Device* device,ID3D11DeviceContext *context) :
    
    d_displayDPI(96, 96),
    d_defaultTarget(0),
    d_effect(0),
    d_normalClippedTechnique(0),
    d_normalUnclippedTechnique(0),
    d_premultipliedClippedTechnique(0),
    d_premultipliedUnclippedTechnique(0),
    d_inputLayout(0),
    d_boundTextureVariable(0),
    d_worldMatrixVariable(0),
    d_projectionMatrixVariable(0)
{

	if(!device || !context) 
	{
		std::string msg("In Order To Use Direct3D11 Module, You Must Provide Both Device And Context");
		CEGUI_THROW(RendererException(msg));
	}

	//ToDo
	//For Perfomance Reasons this little struct can be replaced with pointer
	d_device.d_device=device;
	d_device.d_context=context;

	d_displaySize=getViewportSize();

    // create the main effect from the shader source.
    ID3D10Blob* errors = 0;

	DWORD DefaultOptions=NULL;//D3D10_SHADER_PACK_MATRIX_ROW_MAJOR|D3D10_SHADER_PARTIAL_PRECISION|D3D10_SHADER_SKIP_VALIDATION;

	ID3D10Blob* ShaderBlob=NULL;//first we compile shader, then create effect from it

	if (FAILED(D3DX11CompileFromMemory(shaderSource,sizeof(shaderSource),
		"shaderSource",NULL,NULL,NULL,"fx_5_0",
		DefaultOptions,NULL,NULL,&ShaderBlob,&errors,NULL)))
	{
		std::string msg(static_cast<const char*>(errors->GetBufferPointer()),
			errors->GetBufferSize());
		errors->Release();
		CEGUI_THROW(RendererException(msg));
	}

	if (FAILED(D3DX11CreateEffectFromMemory(ShaderBlob->GetBufferPointer(), ShaderBlob->GetBufferSize(),0, 
		d_device.d_device, &d_effect) ))
	{
		CEGUI_THROW(RendererException("failed to create effect!"));
	}

	if(ShaderBlob) 
		ShaderBlob->Release();

    // extract the rendering techniques
    d_normalClippedTechnique =
            d_effect->GetTechniqueByName("BM_NORMAL_Clipped_Rendering");
    d_normalUnclippedTechnique =
            d_effect->GetTechniqueByName("BM_NORMAL_Unclipped_Rendering");
    d_premultipliedClippedTechnique =
            d_effect->GetTechniqueByName("BM_RTT_PREMULTIPLIED_Clipped_Rendering");
    d_premultipliedClippedTechnique =
            d_effect->GetTechniqueByName("BM_RTT_PREMULTIPLIED_Unclipped_Rendering");

    // Get the variables from the shader we need to be able to access
    d_boundTextureVariable =
            d_effect->GetVariableByName("BoundTexture")->AsShaderResource();
    d_worldMatrixVariable =
            d_effect->GetVariableByName("WorldMatrix")->AsMatrix();
    d_projectionMatrixVariable =
            d_effect->GetVariableByName("ProjectionMatrix")->AsMatrix();

    // Create the input layout
    const D3D11_INPUT_ELEMENT_DESC vertex_layout[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "COLOR",    0, DXGI_FORMAT_R8G8B8A8_UNORM,  0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,	  0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };

    const UINT element_count = sizeof(vertex_layout) / sizeof(vertex_layout[0]);

    D3DX11_PASS_DESC pass_desc;
    if (FAILED(d_normalClippedTechnique->GetPassByIndex(0)->GetDesc(&pass_desc)))
        CEGUI_THROW(RendererException(
            "failed to obtain technique description for pass 0."));

    if (FAILED(d_device.d_device->CreateInputLayout(vertex_layout, element_count,
                                            pass_desc.pIAInputSignature,
                                            pass_desc.IAInputSignatureSize,
                                            &d_inputLayout)))
    {
        CEGUI_THROW(RendererException(
            "failed to create D3D 11 input layout."));
    }

    d_defaultTarget = new Direct3D11ViewportTarget(*this);
}

//----------------------------------------------------------------------------//
Direct3D11Renderer::~Direct3D11Renderer()
{
    destroyAllTextureTargets();
    destroyAllTextures();
    destroyAllGeometryBuffers();

    delete d_defaultTarget;

    if (d_effect)
        d_effect->Release();

    if (d_inputLayout)
        d_inputLayout->Release();

}

//----------------------------------------------------------------------------//
Sizef Direct3D11Renderer::getViewportSize()
{
    D3D11_VIEWPORT vp;
    UINT vp_count = 1;

    d_device.d_context->RSGetViewports(&vp_count, &vp);

    if (vp_count != 1)
        CEGUI_THROW(RendererException(
            "Unable to access required view port information from "
            "ID3D11Device."));
    else
        return Sizef(static_cast<float>(vp.Width),
                      static_cast<float>(vp.Height));
}

//----------------------------------------------------------------------------//
IDevice11& Direct3D11Renderer::getDirect3DDevice()
{
	return d_device;
}

//----------------------------------------------------------------------------//
void Direct3D11Renderer::bindTechniquePass(const BlendMode mode,
                                           const bool clipped)
{
    if (mode == BM_RTT_PREMULTIPLIED)
        if (clipped)
            d_premultipliedClippedTechnique->GetPassByIndex(0)->Apply(0, d_device.d_context);
        else
            d_premultipliedUnclippedTechnique->GetPassByIndex(0)->Apply(0, d_device.d_context);
    else if (clipped)
        d_normalClippedTechnique->GetPassByIndex(0)->Apply(0, d_device.d_context);
    else
        d_normalUnclippedTechnique->GetPassByIndex(0)->Apply(0, d_device.d_context);
}

//----------------------------------------------------------------------------//
void Direct3D11Renderer::setCurrentTextureShaderResource(
    ID3D11ShaderResourceView* srv)
{
    d_boundTextureVariable->SetResource(srv);
}

//----------------------------------------------------------------------------//
void Direct3D11Renderer::setProjectionMatrix(D3DXMATRIX& matrix)
{
    d_projectionMatrixVariable->SetMatrix(reinterpret_cast<float*>(&matrix));
}
//----------------------------------------------------------------------------//
void Direct3D11Renderer::setWorldMatrix(D3DXMATRIX& matrix)
{
    d_worldMatrixVariable->SetMatrix(reinterpret_cast<float*>(&matrix));
}

//----------------------------------------------------------------------------//

} // End of  CEGUI namespace section
