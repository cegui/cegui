/***********************************************************************
    filename:   CEGUIGeometryBuffer.cpp
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

#include <vector>

// Start of CEGUI namespace section
namespace CEGUI
{
//---------------------------------------------------------------------------//
GeometryBuffer::GeometryBuffer(RefCounted<RenderMaterial> renderMaterial) :
    d_blendMode(BM_NORMAL)
    , d_renderMaterial(renderMaterial)
{
}

//---------------------------------------------------------------------------//
GeometryBuffer::~GeometryBuffer()
{
}

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
                                    uint vertex_count)
{
    // Add the vertex data in their default order into an array
    std::vector<float> vertexData;
    const ColouredVertex* vs = vertex_array;
    for (uint i = 0; i < vertex_count; ++i, ++vs)
    {
        // Add all the elements in the default order for textured and coloured
        // geometry into the vector
        vertexData.push_back(vs->d_position.x);
        vertexData.push_back(vs->d_position.y);
        vertexData.push_back(vs->d_position.z);
        vertexData.push_back(vs->d_colour.getRed());
        vertexData.push_back(vs->d_colour.getGreen());
        vertexData.push_back(vs->d_colour.getBlue());
        vertexData.push_back(vs->d_colour.getAlpha());
    }

    // Append the prepared geometry data
    appendGeometry(vertexData);
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
                                    uint vertex_count)
{
    // Add the vertex data in their default order into an array
    std::vector<float> vertexData;
    const TexturedColouredVertex* vs = vertex_array;
    for (uint i = 0; i < vertex_count; ++i, ++vs)
    {
        // Add all the elements in the default order for textured and coloured
        // geometry into the vector
        vertexData.push_back(vs->d_position.x);
        vertexData.push_back(vs->d_position.y);
        vertexData.push_back(vs->d_position.z);
        vertexData.push_back(vs->d_colour.getRed());
        vertexData.push_back(vs->d_colour.getGreen());
        vertexData.push_back(vs->d_colour.getBlue());
        vertexData.push_back(vs->d_colour.getAlpha());
    vertexData.push_back(vs->d_texCoords.x);
        vertexData.push_back(vs->d_texCoords.y);
    }

    // Append the prepared geometry data
    appendGeometry(vertexData);
}

//---------------------------------------------------------------------------//
void GeometryBuffer::appendGeometry(const float* const vertex_data,
                                    uint array_size)
{
    std::vector<float> vectorVertexData(vertex_data, vertex_data + array_size);
    appendGeometry(vectorVertexData);
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
    vertexData[3] = vertex.d_colour.getRed();
    vertexData[4] = vertex.d_colour.getGreen();
    vertexData[5] = vertex.d_colour.getBlue();
    vertexData[6] = vertex.d_colour.getAlpha();
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
    vertexData[3] = vertex.d_colour.getRed();
    vertexData[4] = vertex.d_colour.getGreen();
    vertexData[5] = vertex.d_colour.getBlue();
    vertexData[6] = vertex.d_colour.getAlpha();

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
            case VAT_POSITION0:
                count += 3;
                break;
            case VAT_COLOUR0:
                count += 4;
                break;
            case VAT_TEXCOORD0:
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

} // End of  CEGUI namespace section

