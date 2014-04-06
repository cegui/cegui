/***********************************************************************
    filename:   GeometryBuffer.cpp
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
#include "glm/gtc/matrix_transform.hpp"

// Start of CEGUI namespace section
namespace CEGUI
{
//----------------------------------------------------------------------------//
Direct3D11GeometryBuffer::Direct3D11GeometryBuffer(Direct3D11Renderer& owner, CEGUI::RefCounted<RenderMaterial> renderMaterial)
    : GeometryBuffer(renderMaterial)
    , d_owner(owner)
    , d_device(d_owner.getDirect3DDevice())
    , d_deviceContext(d_owner.getDirect3DDeviceContext())
    , d_bufferSize(0)
    , d_clipRect(0, 0, 0, 0)
    , d_inputLayout(0)
    , d_vertexBuffer(0)
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

    // Update the model matrix if necessary
    if (!d_matrixValid)
        updateMatrix();
 
    CEGUI::ShaderParameterBindings* shaderParameterBindings = (*d_renderMaterial).getShaderParamBindings();
    // Set the ModelViewProjection matrix in the bindings
    glm::mat4 modelViewProjectionMatrix = d_owner.getViewProjectionMatrix() * d_matrix;
    shaderParameterBindings->setParameter("modelViewPerspMatrix", modelViewProjectionMatrix);

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
}

//----------------------------------------------------------------------------//
void Direct3D11GeometryBuffer::setClippingRegion(const Rectf& region)
{
    d_clipRect.top(ceguimax(0.0f, region.top()));
    d_clipRect.bottom(ceguimax(0.0f, region.bottom()));
    d_clipRect.left(ceguimax(0.0f, region.left()));
    d_clipRect.right(ceguimax(0.0f, region.right()));
}


//----------------------------------------------------------------------------//
void Direct3D11GeometryBuffer::appendGeometry(const std::vector<float>& vertex_data)
{
    d_vertexData.insert(d_vertexData.end(), vertex_data.begin(), vertex_data.end());
    // Update size of geometry buffer
    d_vertexCount = d_vertexData.size() / getVertexAttributeElementCount();

    updateVertexBuffer();
}

//----------------------------------------------------------------------------//
void Direct3D11GeometryBuffer::updateMatrix() const
{
    const glm::vec3 final_trans(d_translation.d_x + d_pivot.d_x,
                                d_translation.d_y + d_pivot.d_y,
                                d_translation.d_z + d_pivot.d_z);

    d_matrix = glm::translate(glm::mat4(1.0f), final_trans);

    glm::quat rotationQuat = glm::quat(d_rotation.d_w, d_rotation.d_x, d_rotation.d_y, d_rotation.d_z);
    glm::mat4 rotation_matrix = glm::mat4_cast(rotationQuat);

    glm::mat4 scale_matrix(glm::scale(glm::mat4(1.0f), glm::vec3(d_scale.d_x, d_scale.d_y, d_scale.d_z)));

    d_matrix *= rotation_matrix * scale_matrix;

    glm::vec3 transl = glm::vec3(-d_pivot.d_x, -d_pivot.d_y, -d_pivot.d_z);
    glm::mat4 translMatrix = glm::translate(glm::mat4(1.0f), transl);
    d_matrix *=  translMatrix * d_customTransform;

    d_matrixValid = true;
}

//----------------------------------------------------------------------------//
const glm::mat4& Direct3D11GeometryBuffer::getMatrix() const
{
    if (!d_matrixValid)
        updateMatrix();

    return d_matrix;
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
        CEGUI_THROW(RendererException("failed to allocate vertex buffer."));
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
/*    if(d_polygonFillRule == PFR_NONE)
    {
    */

        d_deviceContext->Draw(d_vertexCount, 0);
            /* 
    }
    else if(d_polygonFillRule == PFR_EVEN_ODD)
//....
*/
}

//----------------------------------------------------------------------------//
void Direct3D11GeometryBuffer::finaliseVertexAttributes()
{
    // Create the input layout
    std::vector<D3D11_INPUT_ELEMENT_DESC> vertexLayoutVector;

    //Update the vertex attrib pointers of the vertex array object depending on the saved attributes
    int dataOffset = 0;
    const size_t attribute_count = d_vertexAttributes.size();
    for (size_t i = 0; i < attribute_count; ++i)
    {
        switch(d_vertexAttributes.at(i))
        {
        case VAT_POSITION0:
            {
                D3D11_INPUT_ELEMENT_DESC inputEleDescription = {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, dataOffset, D3D11_INPUT_PER_VERTEX_DATA, 0};
                vertexLayoutVector.push_back( inputEleDescription );

                dataOffset += 12;
            }
            break;
        case VAT_COLOUR0:
            {
                D3D11_INPUT_ELEMENT_DESC inputEleDescription = {"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, dataOffset, D3D11_INPUT_PER_VERTEX_DATA, 0};
                vertexLayoutVector.push_back(inputEleDescription);

                dataOffset += 16;
            }
            break;
        case VAT_TEXCOORD0:
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
                CEGUI_THROW(RendererException(
            "The empty vertex layout is invalid because it is empty."));


    CEGUI::Direct3D11ShaderWrapper* shaderWrapper = static_cast<CEGUI::Direct3D11ShaderWrapper*>(d_renderMaterial->getShaderWrapper());

    if (d_inputLayout)
        d_inputLayout->Release();

    if (FAILED(d_device->CreateInputLayout(&vertexLayoutVector[0], vertexLayoutVector.size(),
                                            shaderWrapper->getVertShaderBufferPointer(),
                                            shaderWrapper->getVertShaderBufferSize(),
                                            &d_inputLayout)))
    {
        CEGUI_THROW(RendererException(
            "Failed to create D3D InputLayout."));
    }
}

//----------------------------------------------------------------------------//
void Direct3D11GeometryBuffer::setScissorRects() const
{
    D3D11_RECT clip;
    clip.left   = static_cast<LONG>(d_clipRect.left());
    clip.top    = static_cast<LONG>(d_clipRect.top());
    clip.right  = static_cast<LONG>(d_clipRect.right());
    clip.bottom = static_cast<LONG>(d_clipRect.bottom());
    d_deviceContext->RSSetScissorRects(1, &clip);
}

//----------------------------------------------------------------------------//

} // End of  CEGUI namespace section

