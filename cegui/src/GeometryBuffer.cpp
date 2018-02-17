/***********************************************************************
    created:    Wed Jan 13 2010
    author:     Paul D Turner <paul@cegui.org.uk>
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
#include "CEGUI/GeometryBuffer.h"
#include "CEGUI/Vertex.h"
#include "CEGUI/ShaderParameterBindings.h"
#include "CEGUI/RenderTarget.h"

#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include <algorithm>
#include <iterator>
#include <stddef.h>

namespace CEGUI
{
//---------------------------------------------------------------------------//
GeometryBuffer::GeometryBuffer(RefCounted<RenderMaterial> renderMaterial):
    d_renderMaterial(renderMaterial),
    d_vertexCount(0),
    d_translation(0, 0, 0),
    d_rotation(1, 0, 0, 0), // <-- IDENTITY
    d_scale(1.0f, 1.0f, 1.0f),
    d_pivot(0, 0, 0),
    d_customTransform(1.0f),
    d_matrixValid(false),
    d_lastRenderTarget(nullptr),
    d_lastRenderTargetActivationCount(0),
    d_blendMode(BlendMode::Normal),
    d_polygonFillRule(PolygonFillRule::NoFilling),
    d_postStencilVertexCount(0),
    d_effect(nullptr),
    d_clippingRegion(0, 0, 0, 0),
    d_preparedClippingRegion(0, 0, 0, 0),
    d_clippingActive(false),
    d_alpha(1.0f)
{}

//---------------------------------------------------------------------------//
GeometryBuffer::~GeometryBuffer()
{}

//---------------------------------------------------------------------------//
void GeometryBuffer::setBlendMode(const BlendMode mode)
{
    d_blendMode = mode;
}

//---------------------------------------------------------------------------//
BlendMode GeometryBuffer::getBlendMode() const
{
    return d_blendMode;
}

//---------------------------------------------------------------------------//
void GeometryBuffer::appendGeometry(const std::vector<ColouredVertex>& coloured_vertices)
{
    if(coloured_vertices.empty())
        return;

    appendGeometry(&coloured_vertices[0], coloured_vertices.size());
}

//---------------------------------------------------------------------------//
void GeometryBuffer::appendGeometry(const ColouredVertex* vertex_array,
                                    std::size_t vertex_count)
{
    // Create a temporary array to contain our data
    static const std::size_t vertexDataSize = 7;
    std::size_t fullArraySize = vertexDataSize * vertex_count;
    float* vertexData = new float[fullArraySize];

    // Add the vertex data in their default order into an array
    const ColouredVertex* vs = vertex_array;
    for (std::size_t i = 0; i < vertex_count; ++i, ++vs)
    {
        // Add all the elements in the default order for textured and coloured
        // geometry into the vector
        std::size_t currentIndex = i * vertexDataSize;

        vertexData[currentIndex + 0] = vs->d_position.x;
        vertexData[currentIndex + 1] = vs->d_position.y;
        vertexData[currentIndex + 2] = vs->d_position.z;
        vertexData[currentIndex + 3] = vs->d_colour.x;
        vertexData[currentIndex + 4] = vs->d_colour.y;
        vertexData[currentIndex + 5] = vs->d_colour.z;
        vertexData[currentIndex + 6] = vs->d_colour.w;
    }

    // Append the prepared geometry data
    appendGeometry(vertexData, fullArraySize);

    delete[] vertexData;
}

//---------------------------------------------------------------------------//
void GeometryBuffer::appendGeometry(const std::vector<TexturedColouredVertex>& textured_vertices)
{
    if(textured_vertices.empty())
        return;

    appendGeometry(&textured_vertices[0], textured_vertices.size());
}

//---------------------------------------------------------------------------//
void GeometryBuffer::appendGeometry(const TexturedColouredVertex* vertex_array,
                                    std::size_t vertex_count)
{
    // Create a temporary array to contain our data
    static const std::size_t vertexDataSize = 9;
    std::size_t fullArraySize = vertexDataSize * vertex_count;
    float* vertexData = new float[fullArraySize];

    // Add the vertex data in their default order into an array
    const TexturedColouredVertex* vs = vertex_array;
    for (std::size_t i = 0; i < vertex_count; ++i, ++vs)
    {
        // Add all the elements in the default order for textured and coloured
        // geometry into the vector
        std::size_t currentIndex = i * vertexDataSize;
        
        vertexData[currentIndex + 0] = vs->d_position.x;
        vertexData[currentIndex + 1] = vs->d_position.y;
        vertexData[currentIndex + 2] = vs->d_position.z;
        vertexData[currentIndex + 3] = vs->d_colour.x;
        vertexData[currentIndex + 4] = vs->d_colour.y;
        vertexData[currentIndex + 5] = vs->d_colour.z;
        vertexData[currentIndex + 6] = vs->d_colour.w;
        vertexData[currentIndex + 7] = vs->d_texCoords.x;
        vertexData[currentIndex + 8] = vs->d_texCoords.y;
    }

    // Append the prepared geometry data
    appendGeometry(vertexData, fullArraySize);

    delete[] vertexData;
}

//---------------------------------------------------------------------------//
void GeometryBuffer::appendGeometry(const float* vertex_data,
                                    std::size_t array_size)
{
    d_vertexData.reserve(d_vertexData.size() + array_size);
    std::copy(vertex_data, vertex_data + array_size, std::back_inserter(d_vertexData));

    // Update size of geometry buffer
    d_vertexCount = d_vertexData.size() / getVertexAttributeElementCount();
}

//---------------------------------------------------------------------------//
void GeometryBuffer::appendVertex(const TexturedColouredVertex& vertex)
{
    // Add the vertex data in their default order into an array
    float vertexData[9];

    // Copy the vertex attributes into the array
    vertexData[0] = vertex.d_position.x;
    vertexData[1] = vertex.d_position.y;
    vertexData[2] = vertex.d_position.z;
    vertexData[3] = vertex.d_colour.x;
    vertexData[4] = vertex.d_colour.y;
    vertexData[5] = vertex.d_colour.z;
    vertexData[6] = vertex.d_colour.w;
    vertexData[7] = vertex.d_texCoords.x;
    vertexData[8] = vertex.d_texCoords.y;

    appendGeometry(vertexData, 9);
}

//---------------------------------------------------------------------------//
void GeometryBuffer::appendVertex(const ColouredVertex& vertex)
{
    // Add the vertex data in their default order into an array
    float vertexData[7];

    // Copy the vertex attributes into the array
    vertexData[0] = vertex.d_position.x;
    vertexData[1] = vertex.d_position.y;
    vertexData[2] = vertex.d_position.z;
    vertexData[3] = vertex.d_colour.x;
    vertexData[4] = vertex.d_colour.y;
    vertexData[5] = vertex.d_colour.z;
    vertexData[6] = vertex.d_colour.w;

    appendGeometry(vertexData, 7);
}

//---------------------------------------------------------------------------//
int GeometryBuffer::getVertexAttributeElementCount() const
{
    int count = 0;

    const unsigned int attribute_count = d_vertexAttributes.size();
    for (unsigned int i = 0; i < attribute_count; ++i)
    {
        switch(d_vertexAttributes.at(i))
        {
            case VertexAttributeType::Position0:
                count += 3;
                break;
            case VertexAttributeType::Colour0:
                count += 4;
                break;
            case VertexAttributeType::TexCoord0:
                count += 2;
                break;
            default:
                break;
        }
    }

    return count;
}

//---------------------------------------------------------------------------//
void GeometryBuffer::resetVertexAttributes()
{
    d_vertexAttributes.clear();
}

//---------------------------------------------------------------------------//
void GeometryBuffer::addVertexAttribute(VertexAttributeType attribute)
{
    d_vertexAttributes.push_back(attribute);
}

//---------------------------------------------------------------------------//
RefCounted<RenderMaterial> GeometryBuffer::getRenderMaterial() const
{
    return d_renderMaterial;
}

//---------------------------------------------------------------------------//
void GeometryBuffer::setRenderMaterial(RefCounted<RenderMaterial> render_material)
{
    d_renderMaterial = render_material;
}

//---------------------------------------------------------------------------//
void GeometryBuffer::setStencilRenderingActive(PolygonFillRule fill_rule)
{
    d_polygonFillRule = fill_rule;
}

//---------------------------------------------------------------------------//
void GeometryBuffer::setStencilPostRenderingVertexCount(unsigned int vertex_count)
{
    d_postStencilVertexCount = vertex_count;
}

//----------------------------------------------------------------------------//
void GeometryBuffer::setRenderEffect(RenderEffect* effect)
{
    d_effect = effect;
}

//----------------------------------------------------------------------------//
RenderEffect* GeometryBuffer::getRenderEffect()
{
    return d_effect;
}

//----------------------------------------------------------------------------//
void GeometryBuffer::setTranslation(const glm::vec3& translation)
{
    if (d_translation != translation)
    {
        d_translation = translation;
        d_matrixValid = false;
    }
}

//----------------------------------------------------------------------------//
void GeometryBuffer::setRotation(const glm::quat& rotationQuat)
{
    if (d_rotation != rotationQuat)
    {
        d_rotation = rotationQuat;
        d_matrixValid = false;
    }
}

//----------------------------------------------------------------------------//
void GeometryBuffer::setScale(const glm::vec3& scale)
{
    if (d_scale != scale)
    {
        d_scale = scale;
        d_matrixValid = false;
    }
}

//----------------------------------------------------------------------------//
void GeometryBuffer::setScale(const glm::vec2& scale)
{
    setScale(glm::vec3(scale, 0));
}

//----------------------------------------------------------------------------//
void GeometryBuffer::setPivot(const glm::vec3& p)
{
    if (d_pivot != p)
    {
        d_pivot = p;
        d_matrixValid = false;
    }
}

//----------------------------------------------------------------------------//
void GeometryBuffer::setCustomTransform(const glm::mat4x4& transformation)
{
    if(d_customTransform != transformation)
    {
        d_customTransform = transformation;
        d_matrixValid = false;
    }
}

void GeometryBuffer::setClippingRegion(const Rectf& region)
{
    d_clippingRegion = region;

    d_preparedClippingRegion.top(std::max(0.0f, region.top()));
    d_preparedClippingRegion.bottom(std::max(0.0f, region.bottom()));
    d_preparedClippingRegion.left(std::max(0.0f, region.left()));
    d_preparedClippingRegion.right(std::max(0.0f, region.right()));
}

const Rectf& GeometryBuffer::getClippingRegion() const
{
    return d_clippingRegion;
}

const Rectf& GeometryBuffer::getPreparedClippingRegion() const
{
    return d_preparedClippingRegion;
}

//----------------------------------------------------------------------------//
void GeometryBuffer::setClippingActive(const bool active)
{
    d_clippingActive = active;
}

//----------------------------------------------------------------------------//
bool GeometryBuffer::isClippingActive() const
{
    return d_clippingActive;
}


//----------------------------------------------------------------------------//
std::size_t GeometryBuffer::getVertexCount() const
{
    return d_vertexCount;
}

//----------------------------------------------------------------------------//
void GeometryBuffer::reset()
{
    d_vertexData.clear();
    d_clippingActive = true;
}

//----------------------------------------------------------------------------//
void GeometryBuffer::setTexture(const std::string& parameterName, const Texture* texture)
{
    CEGUI::ShaderParameterBindings* shaderParameterBindings = (*d_renderMaterial).getShaderParamBindings();
    shaderParameterBindings->setParameter(parameterName, texture);
}

//---------------------------------------------------------------------------//
void GeometryBuffer::setAlpha(float alpha)
{
    d_alpha = alpha;
}

//---------------------------------------------------------------------------//
float GeometryBuffer::getAlpha() const
{
    return d_alpha;
}

//---------------------------------------------------------------------------//
void GeometryBuffer::invalidateMatrix()
{
    d_matrixValid = false;
}

//---------------------------------------------------------------------------//
const RenderTarget* GeometryBuffer::getLastRenderTarget() const
{
    return d_lastRenderTarget;
}

//---------------------------------------------------------------------------//
bool GeometryBuffer::isRenderTargetDataValid(const RenderTarget* activeRenderTarget) const
{
    //! The data received from the RenderTarget is only valid if:
    //! 1. The RenderTarget is the same as the one used the last time
    //! 2. The GeometryBuffer never skipped a RenderTarget activation (Checked via counter)
    return (d_lastRenderTarget == activeRenderTarget) && (d_lastRenderTargetActivationCount + 1 == activeRenderTarget->getActivationCounter());
}

//--------------------------------------------------------------------------//
void GeometryBuffer::updateRenderTargetData(const RenderTarget* activeRenderTarget) const
{
    d_lastRenderTarget = activeRenderTarget;
    d_lastRenderTargetActivationCount = activeRenderTarget->getActivationCounter();
}


//----------------------------------------------------------------------------//
glm::mat4 GeometryBuffer::getModelMatrix() const
{
    glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), d_translation + d_pivot);

    const glm::mat4 scale_matrix(glm::scale(glm::mat4(1.0f), d_scale));
    modelMatrix *= glm::mat4_cast(d_rotation) * scale_matrix;

    const glm::mat4 translMatrix = glm::translate(glm::mat4(1.0f), -d_pivot);
    modelMatrix *=  translMatrix * d_customTransform;

    return modelMatrix;
}

const Texture* GeometryBuffer::getTexture(const std::string& parameterName) const
{
    auto renderMaterial = getRenderMaterial();
    ShaderParameterBindings* shaderParamBindings = (*renderMaterial).getShaderParamBindings();
    ShaderParameter* shaderParam = shaderParamBindings->getParameter(parameterName);
    ShaderParameterTexture* texture0ShaderParam = static_cast<ShaderParameterTexture*>(shaderParam);
    if (texture0ShaderParam == nullptr)
    {
        return nullptr;
    }

    return texture0ShaderParam->d_parameterValue;
}

void GeometryBuffer::updateTextureCoordinates(const Texture* texture, const float scaleFactor)
{
    const Texture* geomBuffTex0 = getTexture("texture0");
 
    if (geomBuffTex0 == nullptr || geomBuffTex0 != texture)
    {
        return;
    }


    size_t vertexCount = d_vertexData.size() / 9;
    for(size_t i = 0; i < vertexCount; ++i)
    {
        d_vertexData[i * 9 + 7] *= scaleFactor;
        d_vertexData[i * 9 + 8] *= scaleFactor;
    }

    VertexData tempVertexData = d_vertexData;
    d_vertexData.clear();
    appendGeometry(tempVertexData.data(), tempVertexData.size());
}

}

