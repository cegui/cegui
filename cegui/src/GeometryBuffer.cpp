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
void GeometryBuffer::appendGeometry(const TexturedColouredVertex* const vbuff,
    uint vertex_count)
{
    // Add the vertex data in their default order into an array
    std::vector<float> vertexData;
    const TexturedColouredVertex* vs = vbuff;
    for (uint i = 0; i < vertex_count; ++i, ++vs)
    {
        // Add all the elements in the default order for textured and coloured
        // geometry into the vector
        vertexData.push_back(vs->position.d_x);
        vertexData.push_back(vs->position.d_y);
        vertexData.push_back(vs->position.d_z);
        vertexData.push_back(vs->colour_val.getRed());
        vertexData.push_back(vs->colour_val.getGreen());
        vertexData.push_back(vs->colour_val.getBlue());
        vertexData.push_back(vs->colour_val.getAlpha());
        vertexData.push_back(vs->tex_coords.d_x);
        vertexData.push_back(vs->tex_coords.d_y);
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
    vertexData[0] = vertex.position.d_x;
    vertexData[1] = vertex.position.d_y;
    vertexData[2] = vertex.position.d_z;
    vertexData[3] = vertex.colour_val.getRed();
    vertexData[4] = vertex.colour_val.getGreen();
    vertexData[5] = vertex.colour_val.getBlue();
    vertexData[6] = vertex.colour_val.getAlpha();
    vertexData[7] = vertex.tex_coords.d_x;
    vertexData[8] = vertex.tex_coords.d_y;

    appendGeometry(vertexData, 9);
}

//---------------------------------------------------------------------------//
void GeometryBuffer::appendVertex(const ColouredVertex& vertex)
{
    // Add the vertex data in their default order into an array
    float vertexData[7];

    // Copy the vertex attributes into the array
    vertexData[0] = vertex.position.d_x;
    vertexData[1] = vertex.position.d_y;
    vertexData[2] = vertex.position.d_z;
    vertexData[3] = vertex.colour_val.getRed();
    vertexData[4] = vertex.colour_val.getGreen();
    vertexData[5] = vertex.colour_val.getBlue();
    vertexData[6] = vertex.colour_val.getAlpha();

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
void GeometryBuffer::appendVertexAttribute(VertexAttributeType attribute)
{
    d_vertexAttributes.push_back(attribute);
}


//---------------------------------------------------------------------------//

} // End of  CEGUI namespace section

