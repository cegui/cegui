/***********************************************************************
    filename:   CEGUIIrrlichtGeometryBuffer.cpp
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
#include "CEGUIIrrlichtGeometryBuffer.h"
#include "CEGUIRenderEffect.h"
#include "CEGUIIrrlichtTexture.h"
#include "CEGUIVertex.h"

// Start of CEGUI namespace section
namespace CEGUI
{
//----------------------------------------------------------------------------//
IrrlichtGeometryBuffer::IrrlichtGeometryBuffer(irr::video::IVideoDriver& driver):
    d_driver(driver),
    d_activeTexture(0),
    d_translation(0, 0, 0),
    d_rotation(0, 0, 0),
    d_pivot(0, 0, 0),
    d_effect(0),
    d_matrixValid(false)
{
    d_material.BackfaceCulling = false;
    d_material.Lighting = false;
    d_material.ZBuffer = 0;
    d_material.ZWriteEnable = false;
    d_material.MaterialType = irr::video::EMT_TRANSPARENT_ALPHA_CHANNEL;
    d_material.MaterialTypeParam = 0;
}

//----------------------------------------------------------------------------//
void IrrlichtGeometryBuffer::draw() const
{
    // TODO: Set up clipping for this buffer
    // TODO: No scissor test, so it's funtime with viewports and projections ;)

    if (!d_matrixValid)
        updateMatrix();

    d_driver.setTransform(irr::video::ETS_WORLD, d_matrix);

    // set up RenderEffect
    if (d_effect)
        d_effect->performPreRenderFunctions();

    // draw the batches
    size_t pos = 0;
    BatchList::const_iterator i = d_batches.begin();
    for ( ; i != d_batches.end(); ++i)
    {
        d_material.setTexture(0, (*i).first);
        d_driver.setMaterial(d_material);
        d_driver.drawIndexedTriangleList(&d_vertices[pos], (*i).second,
                                         &d_indices[pos], (*i).second / 3);
        pos += (*i).second;
    }

    // clean up RenderEffect
    if (d_effect)
        d_effect->performPostRenderFunctions();
}

//----------------------------------------------------------------------------//
void IrrlichtGeometryBuffer::setTransform(const float* matrix)
{
    // This is going to be removed!
}

//----------------------------------------------------------------------------//
void IrrlichtGeometryBuffer::setTranslation(const Vector3& v)
{
    d_translation.X = v.d_x;
    d_translation.Y = v.d_y;
    d_translation.Z = v.d_z;
    d_matrixValid = false;
}

//----------------------------------------------------------------------------//
void IrrlichtGeometryBuffer::setRotation(const Vector3& r)
{
    d_rotation.X = r.d_x;
    d_rotation.Y = r.d_y;
    d_rotation.Z = r.d_z;
    d_matrixValid = false;
}

//----------------------------------------------------------------------------//
void IrrlichtGeometryBuffer::setPivot(const Vector3& p)
{
    d_pivot.X = p.d_x;
    d_pivot.Y = p.d_y;
    d_pivot.Z = p.d_z;
    d_matrixValid = false;
}

//----------------------------------------------------------------------------//
void IrrlichtGeometryBuffer::setClippingRegion(const Rect& region)
{
    d_clipRect = region;
}

//----------------------------------------------------------------------------//
void IrrlichtGeometryBuffer::appendVertex(const Vertex& vertex)
{
    appendGeometry(&vertex, 1);
}

//----------------------------------------------------------------------------//
void IrrlichtGeometryBuffer::appendGeometry(const Vertex* const vbuff,
                                            uint vertex_count)
{
    // see if we should start a new batch
    irr::video::ITexture* t =
        d_activeTexture ? d_activeTexture->getIrrlichtTexture() : 0;

    if (d_batches.empty() || d_batches.back().first != t)
        d_batches.push_back(BatchInfo(t, 0));

    // buffer these vertices
    const irr::u16 idx_start = d_batches.back().second;
    irr::video::S3DVertex v;
    for (uint i = 0; i < vertex_count; ++i)
    {
        const Vertex& vs = vbuff[i];
        v.Pos.X     = vs.position.d_x;
        v.Pos.Y     = vs.position.d_y;
        v.Pos.Z     = vs.position.d_z;
        v.TCoords.X = vs.tex_coords.d_x;
        v.TCoords.Y = vs.tex_coords.d_y;
        v.Color.set(vs.colour_val.getARGB());
        d_vertices.push_back(v);
        d_indices.push_back(idx_start + i);
    }

    // update size of current batch
    d_batches.back().second += vertex_count;
}

//----------------------------------------------------------------------------//
void IrrlichtGeometryBuffer::setActiveTexture(Texture* texture)
{
    d_activeTexture = static_cast<IrrlichtTexture*>(texture);
}

//----------------------------------------------------------------------------//
void IrrlichtGeometryBuffer::reset()
{
    d_vertices.clear();
    d_indices.clear();
    d_batches.clear();
    d_activeTexture = 0;
}

//----------------------------------------------------------------------------//
Texture* IrrlichtGeometryBuffer::getActiveTexture() const
{
    return d_activeTexture;
}

//----------------------------------------------------------------------------//
uint IrrlichtGeometryBuffer::getVertexCount() const
{
    return d_vertices.size();
}

//----------------------------------------------------------------------------//
uint IrrlichtGeometryBuffer::getBatchCount() const
{
    return d_batches.size();
}

//----------------------------------------------------------------------------//
void IrrlichtGeometryBuffer::setRenderEffect(RenderEffect* effect)
{
    d_effect = effect;
}

//----------------------------------------------------------------------------//
RenderEffect* IrrlichtGeometryBuffer::getRenderEffect()
{
    return d_effect;
}

//----------------------------------------------------------------------------//
const irr::core::matrix4& IrrlichtGeometryBuffer::getMatrix() const
{
    return d_matrix;
}

//----------------------------------------------------------------------------//
void IrrlichtGeometryBuffer::updateMatrix() const
{
    d_matrix.makeIdentity();
    d_matrix.setTranslation(d_translation + d_pivot);

    irr::core::matrix4 rot;
    rot.setRotationDegrees(d_rotation);
    irr::core::matrix4 ptrans;
    ptrans.setTranslation(-d_pivot);

    d_matrix *= rot;
    d_matrix *= ptrans;

    d_matrixValid = true;
}

//----------------------------------------------------------------------------//

} // End of  CEGUI namespace section
