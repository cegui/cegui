/***********************************************************************
    filename:   CEGUIDirect3D9GeometryBatch.h
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
#ifndef _CEGUIDirect3D9GeometryBatch_h_
#define _CEGUIDirect3D9GeometryBatch_h_

#include "CEGUIDirect3D9Renderer.h"
#include "CEGUIVertex.h"


// Start of CEGUI namespace section
namespace CEGUI
{
/*!
\brief
    Class representing a batch of geometry that can be rendered.
*/
class DIRECT3D9_GUIRENDERER_API Direct3D9GeometryBatch
{
public:
    //! constructor
    Direct3D9GeometryBatch(LPDIRECT3DDEVICE9 device, 
                           LPDIRECT3DTEXTURE9 texture);

    //! destructor
    ~Direct3D9GeometryBatch();

    //! Copy constructor
    Direct3D9GeometryBatch(const Direct3D9GeometryBatch& ob);

    //! assignment
    Direct3D9GeometryBatch& operator=(const Direct3D9GeometryBatch& ob);

    //! append additional geometry to the batch.
    void appendGeometry(const Vertex* const vbuff, const uint vertex_count);

    //! send this batch of geometry to GL.
    void draw() const;

    //! return the texture used for this geometry batch.
    LPDIRECT3DTEXTURE9 getTexture() const;

    //! return the number of vertices in this batch.
    uint getVertexCount() const;

private:
    //! handle re-allocation of the buffer as needed.
    void performBufferManagement(const uint vertex_count);

    // forward ref for implementation structure used to hold vertex data.
    struct D3DVertex;
    //! D3D9 device we're using
    LPDIRECT3DDEVICE9 d_device;
    //! D3D9 texture this batch uses.
    LPDIRECT3DTEXTURE9 d_tex;
    //! pointer to memory buffer holding the vertices.
    D3DVertex* d_buffer;
    //! Number of vertices buffer is allocated to hold.
    uint d_buff_capacity;
    //! Current number of vertices in the buffer.
    uint d_vertex_count;
};

} // End of  CEGUI namespace section

#endif  // end of guard _CEGUIDirect3D9GeometryBatch_h_
