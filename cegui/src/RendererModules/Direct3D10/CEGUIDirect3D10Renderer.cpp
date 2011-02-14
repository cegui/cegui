/***********************************************************************
    filename:   CEGUIDirect3D10Renderer.cpp
    created:    Sat Mar 7 2009
    author:     Paul D Turner (parts based on code by Rajko Stojadinovic)
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2010 Paul D Turner & The CEGUI Development Team
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
#define NOMINMAX
#include "CEGUIDirect3D10Renderer.h"
#include "CEGUIDirect3D10GeometryBuffer.h"
#include "CEGUIDirect3D10TextureTarget.h"
#include "CEGUIDirect3D10ViewportTarget.h"
#include "CEGUIDirect3D10Texture.h"
#include "CEGUIRenderingRoot.h"
#include "CEGUIExceptions.h"
#include "CEGUISystem.h"
#include "CEGUIDefaultResourceProvider.h"
#include <algorithm>

#include "CEGUIDirect3D10RendererShader.txt"

// Start of CEGUI namespace section
namespace CEGUI
{
//----------------------------------------------------------------------------//
String Direct3D10Renderer::d_rendererID(
"CEGUI::Direct3D10Renderer - Official Direct3D 10 based 2nd generation renderer"
" module.");

//----------------------------------------------------------------------------//
Direct3D10Renderer& Direct3D10Renderer::bootstrapSystem(ID3D10Device* device)
{
    if (System::getSingletonPtr())
        CEGUI_THROW(InvalidRequestException(
            "Direct3D10Renderer::bootstrapSystem: CEGUI::System object is "
            "already initialised."));

    Direct3D10Renderer& renderer(create(device));
    DefaultResourceProvider* rp = new CEGUI::DefaultResourceProvider();
    System::create(renderer, rp);

    return renderer;
}

//----------------------------------------------------------------------------//
void Direct3D10Renderer::destroySystem()
{
    System* sys;
    if (!(sys = System::getSingletonPtr()))
        CEGUI_THROW(InvalidRequestException(
            "Direct3D10Renderer::destroySystem: CEGUI::System object is not "
            "created or was already destroyed."));

    Direct3D10Renderer* renderer =
        static_cast<Direct3D10Renderer*>(sys->getRenderer());
    DefaultResourceProvider* rp =
        static_cast<DefaultResourceProvider*>(sys->getResourceProvider());

    System::destroy();
    delete rp;
    destroy(*renderer);
}

//----------------------------------------------------------------------------//
Direct3D10Renderer& Direct3D10Renderer::create(ID3D10Device* device)
{
    return *new Direct3D10Renderer(device);
}

//----------------------------------------------------------------------------//
void Direct3D10Renderer::destroy(Direct3D10Renderer& renderer)
{
    delete &renderer;
}

//----------------------------------------------------------------------------//
RenderingRoot& Direct3D10Renderer::getDefaultRenderingRoot()
{
    return *d_defaultRoot;
}

//----------------------------------------------------------------------------//
GeometryBuffer& Direct3D10Renderer::createGeometryBuffer()
{
    Direct3D10GeometryBuffer* b = new Direct3D10GeometryBuffer(*this);
    d_geometryBuffers.push_back(b);
    return *b;
}

//----------------------------------------------------------------------------//
void Direct3D10Renderer::destroyGeometryBuffer(const GeometryBuffer& buffer)
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
void Direct3D10Renderer::destroyAllGeometryBuffers()
{
    while (!d_geometryBuffers.empty())
        destroyGeometryBuffer(**d_geometryBuffers.begin());
}

//----------------------------------------------------------------------------//
TextureTarget* Direct3D10Renderer::createTextureTarget()
{
    TextureTarget* t = new Direct3D10TextureTarget(*this);
    d_textureTargets.push_back(t);
    return t;
}

//----------------------------------------------------------------------------//
void Direct3D10Renderer::destroyTextureTarget(TextureTarget* target)
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
void Direct3D10Renderer::destroyAllTextureTargets()
{
    while (!d_textureTargets.empty())
        destroyTextureTarget(*d_textureTargets.begin());
}

//----------------------------------------------------------------------------//
Texture& Direct3D10Renderer::createTexture()
{
    Direct3D10Texture* tex = new Direct3D10Texture(*d_device);
    d_textures.push_back(tex);
    return *tex;
}

//----------------------------------------------------------------------------//
Texture& Direct3D10Renderer::createTexture(const String& filename,
                                           const String& resourceGroup)
{
    Direct3D10Texture* tex = new Direct3D10Texture(*d_device, filename,
                                                   resourceGroup);
    d_textures.push_back(tex);
    return *tex;
}

//----------------------------------------------------------------------------//
Texture& Direct3D10Renderer::createTexture(const Size<>& size)
{
    Direct3D10Texture* tex = new Direct3D10Texture(*d_device, size);
    d_textures.push_back(tex);
    return *tex;
}

//----------------------------------------------------------------------------//
void Direct3D10Renderer::destroyTexture(Texture& texture)
{
    TextureList::iterator i = std::find(d_textures.begin(),
                                        d_textures.end(),
                                        &texture);

    if (d_textures.end() != i)
    {
        d_textures.erase(i);
        delete &static_cast<Direct3D10Texture&>(texture);
    }
}

//----------------------------------------------------------------------------//
void Direct3D10Renderer::destroyAllTextures()
{
    while (!d_textures.empty())
        destroyTexture(**d_textures.begin());
}

//----------------------------------------------------------------------------//
void Direct3D10Renderer::beginRendering()
{
    d_device->IASetInputLayout(d_inputLayout);
    d_device->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

//----------------------------------------------------------------------------//
void Direct3D10Renderer::endRendering()
{
}

//----------------------------------------------------------------------------//
void Direct3D10Renderer::setDisplaySize(const Size<>& sz)
{
    if (sz != d_displaySize)
    {
        d_displaySize = sz;

        // FIXME: This is probably not the right thing to do in all cases.
        Rect area(d_defaultTarget->getArea());
        area.setSize(sz);
        d_defaultTarget->setArea(area);
    }

}

//----------------------------------------------------------------------------//
const Size<>& Direct3D10Renderer::getDisplaySize() const
{
    return d_displaySize;
}

//----------------------------------------------------------------------------//
const Vector2<>& Direct3D10Renderer::getDisplayDPI() const
{
    return d_displayDPI;
}

//----------------------------------------------------------------------------//
uint Direct3D10Renderer::getMaxTextureSize() const
{
    return 8192;
}

//----------------------------------------------------------------------------//
const String& Direct3D10Renderer::getIdentifierString() const
{
    return d_rendererID;
}

//----------------------------------------------------------------------------//
Direct3D10Renderer::Direct3D10Renderer(ID3D10Device* device) :
    d_device(device),
    d_displaySize(getViewportSize()),
    d_displayDPI(96, 96),
    d_defaultTarget(0),
    d_defaultRoot(0),
    d_effect(0),
    d_normalTechnique(0),
    d_premultipliedTechnique(0),
    d_inputLayout(0),
    d_boundTextureVariable(0),
    d_worldMatrixVariable(0),
    d_projectionMatrixVariable(0)
{
    // create the main effect from the shader source.
    ID3D10Blob* errors = 0;
    if (FAILED(D3DX10CreateEffectFromMemory(shaderSource, sizeof(shaderSource),
                                            0, 0, 0, "fx_4_0", 0, 0, d_device,
                                            0, 0, &d_effect, &errors, 0)))
    {
        std::string msg(static_cast<const char*>(errors->GetBufferPointer()),
                        errors->GetBufferSize());
        errors->Release();
        CEGUI_THROW(RendererException(msg));
    }

    // extract the rendering techniques
    d_normalTechnique =
            d_effect->GetTechniqueByName("BM_NORMAL_Rendering");
    d_premultipliedTechnique =
            d_effect->GetTechniqueByName("BM_RTT_PREMULTIPLIED_Rendering");

    // Get the variables from the shader we need to be able to access
    d_boundTextureVariable =
            d_effect->GetVariableByName("BoundTexture")->AsShaderResource();
    d_worldMatrixVariable =
            d_effect->GetVariableByName("WorldMatrix")->AsMatrix();
    d_projectionMatrixVariable =
            d_effect->GetVariableByName("ProjectionMatrix")->AsMatrix();

    // Create the input layout
    const D3D10_INPUT_ELEMENT_DESC vertex_layout[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  0, D3D10_INPUT_PER_VERTEX_DATA, 0 },
        { "COLOR",    0, DXGI_FORMAT_R8G8B8A8_UNORM,  0, 12, D3D10_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,	  0, 16, D3D10_INPUT_PER_VERTEX_DATA, 0 },
    };

    const UINT element_count = sizeof(vertex_layout) / sizeof(vertex_layout[0]);

    D3D10_PASS_DESC pass_desc;
    if (FAILED(d_normalTechnique->GetPassByIndex(0)->GetDesc(&pass_desc)))
        CEGUI_THROW(RendererException(
            "Direct3D10Renderer: failed to obtain technique "
            "description for pass 0."));

    if (FAILED(d_device->CreateInputLayout(vertex_layout, element_count,
                                            pass_desc.pIAInputSignature,
                                            pass_desc.IAInputSignatureSize,
                                            &d_inputLayout)))
    {
        CEGUI_THROW(RendererException(
            "Direct3D10Renderer: failed to create D3D 10 input layout."));
    }

    d_defaultTarget = new Direct3D10ViewportTarget(*this);
    d_defaultRoot = new RenderingRoot(*d_defaultTarget);
}

//----------------------------------------------------------------------------//
Direct3D10Renderer::~Direct3D10Renderer()
{
    destroyAllTextureTargets();
    destroyAllTextures();
    destroyAllGeometryBuffers();

    delete d_defaultRoot;
    delete d_defaultTarget;

    if (d_effect)
        d_effect->Release();

    if (d_inputLayout)
        d_inputLayout->Release();

}

//----------------------------------------------------------------------------//
Size<> Direct3D10Renderer::getViewportSize()
{
    D3D10_VIEWPORT vp;
    UINT vp_count = 1;

    d_device->RSGetViewports(&vp_count, &vp);

    if (vp_count != 1)
        CEGUI_THROW(RendererException(
            "Direct3D10Renderer::getViewportSize: Unable "
            "to access required view port information from IDirect3DDevice10."));
    else
        return Size<>(static_cast<float>(vp.Width),
                      static_cast<float>(vp.Height));
}

//----------------------------------------------------------------------------//
ID3D10Device& Direct3D10Renderer::getDirect3DDevice() const
{
    return *d_device;
}

//----------------------------------------------------------------------------//
void Direct3D10Renderer::bindTechniquePass(const BlendMode mode)
{
    if (mode == BM_RTT_PREMULTIPLIED)
        d_premultipliedTechnique->GetPassByIndex(0)->Apply(0);
    else
        d_normalTechnique->GetPassByIndex(0)->Apply(0);
}

//----------------------------------------------------------------------------//
void Direct3D10Renderer::setCurrentTextureShaderResource(
    ID3D10ShaderResourceView* srv)
{
    d_boundTextureVariable->SetResource(srv);
}

//----------------------------------------------------------------------------//
void Direct3D10Renderer::setProjectionMatrix(D3DXMATRIX& matrix)
{
    d_projectionMatrixVariable->SetMatrix(reinterpret_cast<float*>(&matrix));
}
//----------------------------------------------------------------------------//
void Direct3D10Renderer::setWorldMatrix(D3DXMATRIX& matrix)
{
    d_worldMatrixVariable->SetMatrix(reinterpret_cast<float*>(&matrix));
}

//----------------------------------------------------------------------------//

} // End of  CEGUI namespace section
