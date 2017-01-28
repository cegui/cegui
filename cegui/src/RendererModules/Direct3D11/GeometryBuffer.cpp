/***********************************************************************
    created:    Sun, 6th April 2014
    author:     Lukas E Meindl
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2014 Paul D Turner & The CEGUI Development Team
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
#include "CEGUI/RendererModules/Direct3D11/GeometryBuffer.h"
#include "CEGUI/RendererModules/Direct3D11/Texture.h"
#include "CEGUI/RendererModules/Direct3D11/ShaderWrapper.h"
#include "CEGUI/ShaderParameterBindings.h"
#include "CEGUI/RenderEffect.h"
#include "CEGUI/Vertex.h"
#include "CEGUI/Exceptions.h"

#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtc/type_ptr.hpp"

// Start of CEGUI namespace section
namespace CEGUI
{
//----------------------------------------------------------------------------//
Direct3D11GeometryBuffer::Direct3D11GeometryBuffer(Direct3D11Renderer& owner, CEGUI::RefCounted<RenderMaterial> renderMaterial)
    : GeometryBuffer(renderMaterial)
    , d_owner(owner)
    , d_device(d_owner.getDirect3DDevice())
    , d_deviceContext(d_owner.getDirect3DDeviceContext())
    , d_vertexBuffer(0)
    , d_bufferSize(0)
    , d_inputLayout(0)
{
}

//----------------------------------------------------------------------------//
Direct3D11GeometryBuffer::~Direct3D11GeometryBuffer()
{
    cleanupVertexBuffer();

    if (d_inputLayout)
        d_inputLayout->Release();
}

//----------------------------------------------------------------------------//
void Direct3D11GeometryBuffer::draw() const
{
    if(d_vertexData.empty())
        return;

    // setup clip region
    if(d_clippingActive)
        setScissorRects();

    // Update the model view projection matrix
    updateMatrix();
 
    CEGUI::ShaderParameterBindings* shaderParameterBindings = (*d_renderMaterial).getShaderParamBindings();

    // Set the uniform variables for this GeometryBuffer in the Shader
    shaderParameterBindings->setParameter("modelViewProjMatrix", d_matrix);
    shaderParameterBindings->setParameter("alphaPercentage", d_alpha);

    // set our buffer as the vertex source.
    const UINT stride = getVertexAttributeElementCount() * sizeof(float);
    const UINT offset = 0;
    d_deviceContext->IASetVertexBuffers(0, 1, &d_vertexBuffer, &stride, &offset);
    //Update the input layout
    d_deviceContext->IASetInputLayout(d_inputLayout);

    d_owner.bindBlendMode(d_blendMode);
    d_owner.bindRasterizerState(d_clippingActive);

    const int pass_count = d_effect ? d_effect->getPassCount() : 1;
    for (int pass = 0; pass < pass_count; ++pass)
    {
        // set up RenderEffect
        if (d_effect)
            d_effect->performPreRenderFunctions(pass);

        //Prepare for the rendering process according to the used render material
        d_renderMaterial->prepareForRendering();

        // draw the geometry
        drawDependingOnFillRule();
    }

    // clean up RenderEffect
    if (d_effect)
        d_effect->performPostRenderFunctions();

    updateRenderTargetData(d_owner.getActiveRenderTarget());
}

//----------------------------------------------------------------------------//
void Direct3D11GeometryBuffer::appendGeometry(const float* vertex_data, std::size_t array_size)
{
    GeometryBuffer::appendGeometry(vertex_data, array_size);

    updateVertexBuffer();
}

//----------------------------------------------------------------------------//
void Direct3D11GeometryBuffer::updateMatrix() const
{
    if ( !d_matrixValid || !isRenderTargetDataValid(d_owner.getActiveRenderTarget()) )
    {
        // Apply the view projection matrix to the model matrix and save the result as cached matrix
        d_matrix = d_owner.getViewProjectionMatrix() * getModelMatrix();

        d_matrixValid = true;
    }
}

//----------------------------------------------------------------------------//
void Direct3D11GeometryBuffer::updateVertexBuffer() const
{
    bool needNewBuffer = false;
    size_t vertexCount = d_vertexData.size();
    UINT dataSize = vertexCount * sizeof(float);

    if(d_bufferSize < dataSize)
    {
        needNewBuffer = true;
        d_bufferSize = dataSize;
    }

    const float* vertexData;
    if(d_vertexData.empty())
        vertexData = 0;
    else
        vertexData = &d_vertexData[0];


    if(needNewBuffer)
    {
        cleanupVertexBuffer();
        allocateVertexBuffer(dataSize);
    }

    D3D11_BOX box;
    box.left  = 0; //We copy everything
    box.right = dataSize; //Number of bytes to copy
    box.top  = 0;
    box.bottom = 1;
    box.front = 0;
    box.back  = 1;

    d_deviceContext->UpdateSubresource( d_vertexBuffer, 0, &box, vertexData, 0, 0 );
}

//----------------------------------------------------------------------------//
void Direct3D11GeometryBuffer::allocateVertexBuffer(const UINT dataSize) const
{
    D3D11_BUFFER_DESC buffer_desc;
    buffer_desc.Usage          = D3D11_USAGE_DEFAULT;
    buffer_desc.ByteWidth      = dataSize;
    buffer_desc.BindFlags      = D3D11_BIND_VERTEX_BUFFER;
    buffer_desc.CPUAccessFlags = 0;
    buffer_desc.MiscFlags      = 0;

    if (FAILED(d_device->CreateBuffer(&buffer_desc, 0, &d_vertexBuffer)))
        throw RendererException("failed to allocate vertex buffer.");
}

//----------------------------------------------------------------------------//
void Direct3D11GeometryBuffer::cleanupVertexBuffer() const
{
    if (d_vertexBuffer)
    {
        d_vertexBuffer->Release();
        d_vertexBuffer = 0;
        d_bufferSize = 0;
    }
}

//----------------------------------------------------------------------------//
void Direct3D11GeometryBuffer::drawDependingOnFillRule() const
{
    //TODO IDENT
/*    if(d_polygonFillRule == PolygonFillRule::NoFilling)
    {
    */

        d_deviceContext->Draw(d_vertexCount, 0);
            /* 
    }
    else if(d_polygonFillRule == PolygonFillRule::EvenOdd)
//....
*/
}

//----------------------------------------------------------------------------//
void Direct3D11GeometryBuffer::finaliseVertexAttributes()
{
    // Create the input layout
    std::vector<D3D11_INPUT_ELEMENT_DESC> vertexLayoutVector;

    //Update the vertex attrib pointers of the vertex array object depending on the saved attributes
    UINT dataOffset = 0;
    const size_t attribute_count = d_vertexAttributes.size();
    for (size_t i = 0; i < attribute_count; ++i)
    {
        switch(d_vertexAttributes.at(i))
        {
        case VertexAttributeType::Position0:
            {
                D3D11_INPUT_ELEMENT_DESC inputEleDescription = {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, dataOffset, D3D11_INPUT_PER_VERTEX_DATA, 0};
                vertexLayoutVector.push_back( inputEleDescription );

                dataOffset += 12;
            }
            break;
        case VertexAttributeType::Colour0:
            {
                D3D11_INPUT_ELEMENT_DESC inputEleDescription = {"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, dataOffset, D3D11_INPUT_PER_VERTEX_DATA, 0};
                vertexLayoutVector.push_back(inputEleDescription);

                dataOffset += 16;
            }
            break;
        case VertexAttributeType::TexCoord0:
            {
                D3D11_INPUT_ELEMENT_DESC inputEleDescription = {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, dataOffset, D3D11_INPUT_PER_VERTEX_DATA, 0};
                vertexLayoutVector.push_back(inputEleDescription);

                dataOffset += 8;
            }
            break;
        default:
            break;
        }
    }

    if(vertexLayoutVector.size() == 0)
                throw RendererException(
            "The empty vertex layout is invalid because it is empty.");


    const CEGUI::Direct3D11ShaderWrapper* shaderWrapper = static_cast<const CEGUI::Direct3D11ShaderWrapper*>(d_renderMaterial->getShaderWrapper());

    if (d_inputLayout)
        d_inputLayout->Release();

    if (FAILED(d_device->CreateInputLayout(&vertexLayoutVector[0], vertexLayoutVector.size(),
                                            shaderWrapper->getVertShaderBufferPointer(),
                                            shaderWrapper->getVertShaderBufferSize(),
                                            &d_inputLayout)))
    {
        throw RendererException(
            "Failed to create D3D InputLayout.");
    }
}

//----------------------------------------------------------------------------//
void Direct3D11GeometryBuffer::setScissorRects() const
{
    D3D11_RECT clip;
    clip.left   = static_cast<LONG>(d_preparedClippingRegion.left());
    clip.top    = static_cast<LONG>(d_preparedClippingRegion.top());
    clip.right  = static_cast<LONG>(d_preparedClippingRegion.right());
    clip.bottom = static_cast<LONG>(d_preparedClippingRegion.bottom());
    d_deviceContext->RSSetScissorRects(1, &clip);
}

//----------------------------------------------------------------------------//

} // End of  CEGUI namespace section

