/***********************************************************************
    filename:   CEGUIDirect3D9GeometryBatch.cpp
    created:    Tue Feb 10 2009
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
#include "CEGUIDirect3D9GeometryBatch.h"
#include "CEGUIExceptions.h"
#include <cstdlib>

// Start of CEGUI namespace section
namespace CEGUI
{
//----------------------------------------------------------------------------//
// internal Vertex structure used for Direct3D based geometry.
struct Direct3D9GeometryBatch::D3DVertex
{
    //! The transformed position for the vertex.
	FLOAT x, y, z;
    //! colour of the vertex.
	DWORD diffuse;
    //! texture coordinates.
	float tu, tv;
};

//----------------------------------------------------------------------------//
Direct3D9GeometryBatch::Direct3D9GeometryBatch(LPDIRECT3DDEVICE9 device,
                                               LPDIRECT3DTEXTURE9 texture) :
    d_device(device),
    d_tex(texture),
    d_buffer(0),
    d_buff_capacity(0),
    d_vertex_count(0)
{
}

//----------------------------------------------------------------------------//
Direct3D9GeometryBatch::~Direct3D9GeometryBatch()
{
    std::free(d_buffer);
}

//----------------------------------------------------------------------------//
Direct3D9GeometryBatch::Direct3D9GeometryBatch(const Direct3D9GeometryBatch& ob):
    d_device(ob.d_device),
    d_tex(ob.d_tex),
    d_buffer(0),
    d_buff_capacity(0),
    d_vertex_count(0)
{
    performBufferManagement(ob.d_vertex_count);
    d_vertex_count = ob.d_vertex_count;
    // deep copy vertex data
    std::memcpy(d_buffer, ob.d_buffer, d_vertex_count * sizeof(D3DVertex));
}

//----------------------------------------------------------------------------//
Direct3D9GeometryBatch& Direct3D9GeometryBatch::operator=(
    const Direct3D9GeometryBatch& ob)
{
    std::free(d_buffer);
    d_device = ob.d_device;
    d_tex = ob.d_tex;
    d_buffer = 0;
    d_buff_capacity = 0;
    d_vertex_count = 0;

    performBufferManagement(ob.d_vertex_count);
    d_vertex_count = ob.d_vertex_count;
    // deep copy vertex data
    std::memcpy(d_buffer, ob.d_buffer, d_vertex_count * sizeof(D3DVertex));

    return *this;
}

//----------------------------------------------------------------------------//
void Direct3D9GeometryBatch::appendGeometry(const Vertex* const vbuff,
                                            const uint vertex_count)
{
    performBufferManagement(d_vertex_count + vertex_count);

    for (uint i = 0; i < vertex_count; ++i)
    {
        const Vertex& vs = vbuff[i];
        D3DVertex& vd = d_buffer[d_vertex_count + i];
        // copy vertex info the buffer, converting from CEGUI::Vertex to
        // something directly usable by D3D as needed.
        vd.x       = vs.position.d_x - 0.5f;
        vd.y       = vs.position.d_y - 0.5f;
        vd.z       = vs.position.d_z;
        vd.diffuse = vs.colour_val.getARGB();
        vd.tu      = vs.tex_coords.d_x;
        vd.tv      = vs.tex_coords.d_y;
    }

    d_vertex_count += vertex_count;
}

//----------------------------------------------------------------------------//
void Direct3D9GeometryBatch::draw() const
{
    d_device->SetTexture(0, d_tex);
    d_device->DrawPrimitiveUP(D3DPT_TRIANGLELIST, d_vertex_count / 3,
        d_buffer, sizeof(D3DVertex));
}

//----------------------------------------------------------------------------//
LPDIRECT3DTEXTURE9 Direct3D9GeometryBatch::getTexture() const
{
    return d_tex;
}

//----------------------------------------------------------------------------//
uint Direct3D9GeometryBatch::getVertexCount() const
{
    return d_vertex_count;
}

//----------------------------------------------------------------------------//
void Direct3D9GeometryBatch::performBufferManagement(const uint vertex_count)
{
    if (vertex_count > d_buff_capacity)
    {
        d_buff_capacity = vertex_count + vertex_count / 2;
        d_buffer = static_cast<D3DVertex*>(
            std::realloc(d_buffer, d_buff_capacity * sizeof(D3DVertex)));

        // for failure case, reset back to sane state and throw
        if (!d_buffer)
        {
            d_buff_capacity = 0;
            d_vertex_count = 0;
            throw MemoryException(
                "Direct3D9GeometryBatch::performBufferManagement: failed to"
                "(re)allocate memory for vertex array.");
        }
    }
}

//----------------------------------------------------------------------------//

} // End of  CEGUI namespace section
