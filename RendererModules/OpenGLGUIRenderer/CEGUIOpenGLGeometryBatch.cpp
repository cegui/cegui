/***********************************************************************
    filename:   CEGUIOpenGLGeometryBatch.cpp
    created:    Thu Jan 8 2009
    author:     Paul D Turner
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2009 Paul D Turner & The CEGUI Development Team
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
#include "CEGUIOpenGLGeometryBatch.h"
#include "CEGUIExceptions.h"
#include <cstdlib>

// Start of CEGUI namespace section
namespace CEGUI
{
//----------------------------------------------------------------------------//
// internal Vertex structure used for GL based geometry.
struct OpenGLGeometryBatch::GLVertex
{
    float tex[2];
    float colour[4];
    float position[3];
};

//----------------------------------------------------------------------------//
OpenGLGeometryBatch::OpenGLGeometryBatch(GLuint texture) :
    d_tex(texture),
    d_buffer(0),
    d_buff_capacity(0),
    d_vertex_count(0)
{
}

//----------------------------------------------------------------------------//
OpenGLGeometryBatch::~OpenGLGeometryBatch()
{
    std::free(d_buffer);
}

//----------------------------------------------------------------------------//
OpenGLGeometryBatch::OpenGLGeometryBatch(const OpenGLGeometryBatch& ob) :
    d_tex(ob.d_tex),
    d_buffer(0),
    d_buff_capacity(0),
    d_vertex_count(0)
{
    performBufferManagement(ob.d_vertex_count);
    d_vertex_count = ob.d_vertex_count;
    // deep copy vertex data
    std::memcpy(d_buffer, ob.d_buffer, d_vertex_count * sizeof(GLVertex));
}

//----------------------------------------------------------------------------//
OpenGLGeometryBatch& OpenGLGeometryBatch::operator=(const OpenGLGeometryBatch& ob)
{
    std::free(d_buffer);
    d_tex = ob.d_tex;
    d_buffer = 0;
    d_buff_capacity = 0;
    d_vertex_count = 0;

    performBufferManagement(ob.d_vertex_count);
    d_vertex_count = ob.d_vertex_count;
    // deep copy vertex data
    std::memcpy(d_buffer, ob.d_buffer, d_vertex_count * sizeof(GLVertex));

    return *this;
}

//----------------------------------------------------------------------------//
void OpenGLGeometryBatch::appendGeometry(const Vertex* const vbuff,
    const uint vertex_count)
{
    performBufferManagement(d_vertex_count + vertex_count);

    for (uint i = 0; i < vertex_count; ++i)
    {
        const Vertex& vs = vbuff[i];
        GLVertex& vd = d_buffer[d_vertex_count + i];
        // copy vertex info the buffer, converting from CEGUI::Vertex to
        // something directly usable by OpenGL as needed.
        vd.tex[0]      = vs.tex_coords.d_x;
        vd.tex[1]      = vs.tex_coords.d_y;
        vd.colour[0]   = vs.colour_val.getRed();
        vd.colour[1]   = vs.colour_val.getGreen();
        vd.colour[2]   = vs.colour_val.getBlue();
        vd.colour[3]   = vs.colour_val.getAlpha();
        vd.position[0] = vs.position.d_x;
        vd.position[1] = vs.position.d_y;
        vd.position[2] = vs.position.d_z;
    }

    d_vertex_count += vertex_count;
}

//----------------------------------------------------------------------------//
void OpenGLGeometryBatch::draw() const
{
    glBindTexture(GL_TEXTURE_2D, d_tex);

    // set up pointers to the vertex element arrays
    glTexCoordPointer(2, GL_FLOAT, sizeof(GLVertex), d_buffer);
    glColorPointer(4, GL_FLOAT, sizeof(GLVertex), &d_buffer[0].colour[0]);
    glVertexPointer(3, GL_FLOAT, sizeof(GLVertex), &d_buffer[0].position[0]);
    // draw the geometry
    glDrawArrays(GL_TRIANGLES, 0, d_vertex_count);
}

//----------------------------------------------------------------------------//
GLuint OpenGLGeometryBatch::getTextureName() const
{
    return d_tex;
}

//----------------------------------------------------------------------------//
uint OpenGLGeometryBatch::getVertexCount() const
{
    return d_vertex_count;
}

//----------------------------------------------------------------------------//
void OpenGLGeometryBatch::performBufferManagement(const uint vertex_count)
{
    if (vertex_count > d_buff_capacity)
    {
        d_buff_capacity = vertex_count + vertex_count / 2;
        d_buffer = static_cast<GLVertex*>(
            std::realloc(d_buffer, d_buff_capacity * sizeof(GLVertex)));

        // for failure case, reset back to sane state and throw
        if (!d_buffer)
        {
            d_buff_capacity = 0;
            d_vertex_count = 0;
            throw MemoryException(
                "OpenGLGeometryBatch::performBufferManagement: failed to"
                "(re)allocate memory for vertex array.");
        }
    }
}

//----------------------------------------------------------------------------//

} // End of  CEGUI namespace section
