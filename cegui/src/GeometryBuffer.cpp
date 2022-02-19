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
#include "CEGUI/Renderer.h" // for BlendMode
#include "CEGUI/RenderMaterial.h"
#include <glm/gtc/matrix_transform.hpp>

namespace CEGUI
{
constexpr size_t COLORED_VERTEX_FLOAT_COUNT = sizeof(ColouredVertex) / sizeof(float);
constexpr size_t TEXTURED_VERTEX_FLOAT_COUNT = sizeof(TexturedColouredVertex) / sizeof(float);

//---------------------------------------------------------------------------//
GeometryBuffer::GeometryBuffer(RefCounted<RenderMaterial> renderMaterial):
    d_renderMaterial(std::move(renderMaterial)),
    d_translation(0.f, 0.f, 0.f),
    d_rotation(1.f, 0.f, 0.f, 0.f), // <-- IDENTITY
    d_scale(1.f, 1.f, 1.f),
    d_pivot(0.f, 0.f, 0.f),
    d_customTransform(1.f),
    d_blendMode(BlendMode::Normal)
{
}

//---------------------------------------------------------------------------//
GeometryBuffer::~GeometryBuffer() = default;

//---------------------------------------------------------------------------//
void GeometryBuffer::appendGeometry(const std::vector<ColouredVertex>& coloured_vertices)
{
    if (!coloured_vertices.empty())
        appendGeometry(coloured_vertices.data(), coloured_vertices.size());
}

//---------------------------------------------------------------------------//
void GeometryBuffer::appendGeometry(const ColouredVertex* vertexArray,
                                    std::size_t vertexCount)
{
    const size_t addedFloatCount = COLORED_VERTEX_FLOAT_COUNT * vertexCount;
    appendGeometry(reinterpret_cast<const float*>(vertexArray), addedFloatCount);
}

//---------------------------------------------------------------------------//
void GeometryBuffer::appendGeometry(const std::vector<TexturedColouredVertex>& textured_vertices)
{
    if (!textured_vertices.empty())
        appendGeometry(textured_vertices.data(), textured_vertices.size());
}

//---------------------------------------------------------------------------//
void GeometryBuffer::appendGeometry(const TexturedColouredVertex* vertexArray,
                                    std::size_t vertexCount)
{
    const size_t addedFloatCount = TEXTURED_VERTEX_FLOAT_COUNT * vertexCount;
    appendGeometry(reinterpret_cast<const float*>(vertexArray), addedFloatCount);
}

//---------------------------------------------------------------------------//
void GeometryBuffer::appendGeometry(const float* vertexArray, std::size_t arraySize)
{
    const size_t prevFloatCount = d_vertexData.size();
    d_vertexData.resize(prevFloatCount + arraySize);

    float* dest = d_vertexData.data() + prevFloatCount;
    std::memcpy(dest, vertexArray, arraySize * sizeof(float));

    d_vertexCount = d_vertexData.size() / getVertexAttributeElementCount();
}

//---------------------------------------------------------------------------//
void GeometryBuffer::appendVertex(const TexturedColouredVertex& vertex)
{
    // Add the vertex data in their default order into an array
    float vertexData[TEXTURED_VERTEX_FLOAT_COUNT];

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

    appendGeometry(vertexData, TEXTURED_VERTEX_FLOAT_COUNT);
}

//---------------------------------------------------------------------------//
void GeometryBuffer::appendVertex(const ColouredVertex& vertex)
{
    // Add the vertex data in their default order into an array
    float vertexData[COLORED_VERTEX_FLOAT_COUNT];

    // Copy the vertex attributes into the array
    vertexData[0] = vertex.d_position.x;
    vertexData[1] = vertex.d_position.y;
    vertexData[2] = vertex.d_position.z;
    vertexData[3] = vertex.d_colour.x;
    vertexData[4] = vertex.d_colour.y;
    vertexData[5] = vertex.d_colour.z;
    vertexData[6] = vertex.d_colour.w;

    appendGeometry(vertexData, COLORED_VERTEX_FLOAT_COUNT);
}

//---------------------------------------------------------------------------//
int GeometryBuffer::getVertexAttributeElementCount() const
{
    int count = 0;

    const unsigned int attribute_count = d_vertexAttributes.size();
    for (unsigned int i = 0; i < attribute_count; ++i)
    {
        switch (d_vertexAttributes.at(i))
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
RenderMaterial* GeometryBuffer::getRenderMaterial() const
{
    return d_renderMaterial.get();
}

//---------------------------------------------------------------------------//
void GeometryBuffer::setRenderMaterial(RefCounted<RenderMaterial> render_material)
{
    d_renderMaterial = render_material;
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
    setScale(glm::vec3(scale, 0.f));
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
    if (d_customTransform != transformation)
    {
        d_customTransform = transformation;
        d_matrixValid = false;
    }
}

//----------------------------------------------------------------------------//
void GeometryBuffer::setClippingRegion(const Rectf& region)
{
    d_clippingRegion = region;

    d_preparedClippingRegion.top(std::max(0.0f, region.top()));
    d_preparedClippingRegion.bottom(std::max(0.0f, region.bottom()));
    d_preparedClippingRegion.left(std::max(0.0f, region.left()));
    d_preparedClippingRegion.right(std::max(0.0f, region.right()));
}

//----------------------------------------------------------------------------//
void GeometryBuffer::reset()
{
    d_vertexData.clear();
    d_vertexCount = 0;
    d_postStencilVertexCount = 0;

    d_lastRenderTarget = nullptr;
    d_lastRenderTargetActivationCount = 0;

    d_translation = glm::vec3(0.f, 0.f, 0.f);
    d_rotation = glm::quat(1.f, 0.f, 0.f, 0.f);
    d_scale = glm::vec3(1.f, 1.f, 1.f);
    d_pivot = glm::vec3(0.f, 0.f, 0.f);
    d_customTransform = glm::mat4x4(1.f);
    d_clippingRegion = Rectf();
    d_preparedClippingRegion = Rectf();

    d_effect = nullptr;
    d_alpha = 1.f;
    d_blendMode = BlendMode::Normal;
    d_polygonFillRule = PolygonFillRule::NoFilling;
    d_clippingActive = false;
    d_matrixValid = false;
}

//----------------------------------------------------------------------------//
void GeometryBuffer::setTexture(const std::string& parameterName, const Texture* texture)
{
    d_renderMaterial->getShaderParamBindings()->setParameter(parameterName, texture);
}

//--------------------------------------------------------------------------//
const Texture* GeometryBuffer::getTexture(const std::string& parameterName) const
{
    auto shaderParam = d_renderMaterial->getShaderParamBindings()->getParameter(parameterName);
    return shaderParam ? static_cast<ShaderParameterTexture*>(shaderParam)->d_parameterValue : nullptr;
}

//--------------------------------------------------------------------------//
void GeometryBuffer::setMainTexture(const Texture* texture)
{
    d_renderMaterial->setMainTexture(texture);
}

//--------------------------------------------------------------------------//
const Texture* GeometryBuffer::getMainTexture() const
{
    return d_renderMaterial->getMainTexture();
}

//--------------------------------------------------------------------------//
void GeometryBuffer::updateTextureCoordinates(const Texture* texture, float scaleFactor)
{
    // Check that the buffer is textured and uses the given texture
    if (!texture || getMainTexture() != texture)
        return;

    for (size_t i = 0; i < d_vertexCount; ++i)
    {
        d_vertexData[i * TEXTURED_VERTEX_FLOAT_COUNT + 7] *= scaleFactor;
        d_vertexData[i * TEXTURED_VERTEX_FLOAT_COUNT + 8] *= scaleFactor;
    }
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

}

