/***********************************************************************
    filename:   CEGUIIrrlichtGeometryBuffer.h
    created:    Tue Mar 3 2009
    author:     Paul D Turner (parts based on original code by Thomas Suter)
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
#ifndef _CEGUIIrrlichtGeometryBuffer_h_
#define _CEGUIIrrlichtGeometryBuffer_h_

#include "CEGUIIrrlichtRendererDef.h"
#include "../../CEGUIGeometryBuffer.h"
#include "../../CEGUIRect.h"
#include "../../CEGUIVector.h"
#include <irrlicht.h>
#include <vector>

#if defined(_MSC_VER)
#   pragma warning(push)
#   pragma warning(disable : 4251)
#endif

// Start of CEGUI namespace section
namespace CEGUI
{
class IrrlichtTexture;

//! GeometryBuffer implementation for the Irrlicht engine.
class IRR_GUIRENDERER_API IrrlichtGeometryBuffer : public GeometryBuffer
{
public:
    //! constructor
    IrrlichtGeometryBuffer(irr::video::IVideoDriver& driver);

    //! return the transformation matrix used for this buffer.
    const irr::core::matrix4& getMatrix() const;

    //! return reference to the Irrlicht material used with this GeometryBuffer.
    irr::video::SMaterial& getMaterial();
    //! return reference to the Irrlicht material used with this GeometryBuffer.
    const irr::video::SMaterial& getMaterial() const;


    // implement GeometryBuffer interface
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

    //! Irrlicht video driver we're to use.
    irr::video::IVideoDriver& d_driver;
    //! Texture that is set as active
    IrrlichtTexture* d_activeTexture;
    //! material used with the texture for rendering.
    mutable irr::video::SMaterial d_material;
    //! rectangular clip region
    Rect d_clipRect;
    //! translation vector
    irr::core::vector3d<irr::f32> d_translation;
    //! rotation vector
    irr::core::vector3d<irr::f32> d_rotation;
    //! pivot point for rotation
    irr::core::vector3d<irr::f32> d_pivot;
    //! RenderEffect that will be used by the GeometryBuffer
    RenderEffect* d_effect;
    //! model matrix cache
    mutable irr::core::matrix4 d_matrix;
    //! true when d_matrix is valid and up to date
    mutable bool d_matrixValid;
    //! type to track info for per-texture sub batches of geometry
    typedef std::pair<irr::video::ITexture*, uint> BatchInfo;
    //! type of container that tracks BatchInfos.
    typedef std::vector<BatchInfo> BatchList;
    //! type of container used to queue the geometry
    typedef std::vector<irr::video::S3DVertex> VertexList;
    //! type of container used for indexes
    typedef std::vector<irr::u16> IndexList;
    //! list of texture batches added to the geometry buffer
    BatchList d_batches;
    //! container where added geometry vertices are stored.
    VertexList d_vertices;
    //! container where added geometry indices are stored.
    IndexList d_indices;
    //! a coefficient used to handle OpenGL / D3D projection variations.
    const float d_xViewDir;
    //! an offset applied to geometry to get correct texel to pixel mapping.
    const float d_texelOffset;
};


} // End of  CEGUI namespace section

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif

#endif  // end of guard _CEGUIIrrlichtGeometryBuffer_h_
