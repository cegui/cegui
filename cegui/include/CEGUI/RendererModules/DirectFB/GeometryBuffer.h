/***********************************************************************
    filename:   CEGUIDirectFBGeometryBuffer.h
    created:    Tue Mar 10 2009
    author:     Paul D Turner (parts based on code by Keith Mok)
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
#ifndef _CEGUIDirectFBGeometryBuffer_h_
#define _CEGUIDirectFBGeometryBuffer_h_

#include "../../CEGUIGeometryBuffer.h"
#include "../../CEGUIRect.h"
#include <directfb.h>
#include <vector>


// Start of CEGUI namespace section
namespace CEGUI
{
class DirectFBRenderer;
class DirectFBTexture;

//! Implemetation of CEGUI::GeometryBuffer for DirectFB.
class DirectFBGeometryBuffer : public GeometryBuffer
{
public:
    //! Constructor.
    DirectFBGeometryBuffer(DirectFBRenderer& owner);
    //! Destructor
    ~DirectFBGeometryBuffer();

    // Implement GeometryBuffer interface.
    void draw() const;
    void setTranslation(const Vector3& v);
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
    //! update cached matrix
    void updateMatrix() const;

    //! DirectFBRenderer object that created and owns this GeometryBuffer.
    DirectFBRenderer& d_owner;
    //! last texture that was set as active
    DirectFBTexture* d_activeTexture;
    //! type to track info for per-texture sub batches of geometry
    typedef std::pair<IDirectFBSurface*, uint> BatchInfo;
    //! type of container that tracks BatchInfos.
    typedef std::vector<BatchInfo> BatchList;
    //! list of texture batches added to the geometry buffer
    BatchList d_batches;
    //! type of container used to queue the geometry
    typedef std::vector<DFBVertex> VertexList;
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
    //! model matrix cache
    mutable s32 d_matrix[9];
    //! true when d_matrix is valid and up to date
    mutable bool d_matrixValid;
};

} // End of  CEGUI namespace section

#endif  // end of guard _CEGUIDirectFBGeometryBuffer_h_
