/***********************************************************************
    filename:   CEGUIDirect3D9GeometryBuffer.h
    created:    Mon Feb 9 2009
    author:     Paul D Turner
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
#ifndef _CEGUIDirect3D9GeometryBuffer_h_
#define _CEGUIDirect3D9GeometryBuffer_h_

#include "../../CEGUIGeometryBuffer.h"
#include "CEGUIDirect3D9Renderer.h"
#include "../../CEGUIRect.h"
#include <d3dx9.h>
#include <utility>
#include <vector>

#if defined(_MSC_VER)
#   pragma warning(push)
#   pragma warning(disable : 4251)
#endif

// Start of CEGUI namespace section
namespace CEGUI
{
class Direct3D9Texture;

/*!
\brief
    Direct3D9 based implementation of the GeometryBuffer interface.
*/
class DIRECT3D9_GUIRENDERER_API Direct3D9GeometryBuffer : public GeometryBuffer
{
public:
    //! Constructor
    Direct3D9GeometryBuffer(Direct3D9Renderer& owner, LPDIRECT3DDEVICE9 device);

    //! return pointer to D3DXMATRIX used as world transform.
    const D3DXMATRIX* getMatrix() const;

    // implementation of abstract members from GeometryBuffer
    void draw() const;
    void setTranslation(const Vector3& t);
    void setRotation(const Vector3& r);
    void setPivot(const Vector3& p);
    void setClippingRegion(const Rect& region);
    void appendVertex(const Vertex& vertex);
    void appendGeometry(const Vertex* const vbuff, uint vertex_count);
    void setActiveTexture(Texture* texture);
    void reset();
    Texture* getActiveTexture() const;
    uint getVertexCount() const;
    uint getBatchCount() const;
    void setRenderEffect(RenderEffect* effect);
    RenderEffect* getRenderEffect();

protected:
    //! perform batch management operations prior to adding new geometry.
    void performBatchManagement();
    //! update cached matrix
    void updateMatrix() const;

    //! internal Vertex structure used for Direct3D based geometry.
    struct D3DVertex
    {
        //! The transformed position for the vertex.
        FLOAT x, y, z;
        //! colour of the vertex.
        DWORD diffuse;
        //! texture coordinates.
        float tu, tv;
    };

    //! Owning Direct3D9Renderer object
    Direct3D9Renderer& d_owner;
    //! last texture that was set as active
    Direct3D9Texture* d_activeTexture;
    //! type to track info for per-texture sub batches of geometry
    typedef std::pair<LPDIRECT3DTEXTURE9, uint> BatchInfo;
    //! type of container that tracks BatchInfos.
    typedef std::vector<BatchInfo> BatchList;
    //! list of texture batches added to the geometry buffer
    BatchList d_batches;
    //! type of container used to queue the geometry
    typedef std::vector<D3DVertex> VertexList;
    //! container where added geometry is stored.
    VertexList d_vertices;
    //! rectangular clip region
    Rect d_clipRect;
    //! translation vector
    Vector3 d_translation;
    //! rotation vector
    Vector3 d_rotation;
    //! pivot point for rotation
    Vector3 d_pivot;
    //! RenderEffect that will be used by the GeometryBuffer
    RenderEffect* d_effect;
    //! The D3D Device
    LPDIRECT3DDEVICE9 d_device;
    //! model matrix cache
    mutable D3DXMATRIX d_matrix;
    //! true when d_matrix is valid and up to date
    mutable bool d_matrixValid;
};

} // End of  CEGUI namespace section

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif

#endif  // end of guard _CEGUIDirect3D9GeometryBuffer_h_
