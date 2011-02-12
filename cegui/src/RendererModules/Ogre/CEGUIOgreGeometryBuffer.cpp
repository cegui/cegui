/***********************************************************************
    filename:   CEGUIOgreGeometryBuffer.cpp
    created:    Tue Feb 17 2009
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
#include "CEGUIOgreGeometryBuffer.h"
#include "CEGUIOgreTexture.h"
#include "CEGUIVertex.h"
#include "CEGUIRenderEffect.h"

#include <OgreRenderSystem.h>
#include <OgreQuaternion.h>
#include <OgreHardwareBufferManager.h>

// Start of CEGUI namespace section
namespace CEGUI
{
//----------------------------------------------------------------------------//
static const Ogre::LayerBlendModeEx S_colourBlendMode =
{
    Ogre::LBT_COLOUR,
    Ogre::LBX_MODULATE,
    Ogre::LBS_TEXTURE,
    Ogre::LBS_DIFFUSE
};

//----------------------------------------------------------------------------//
static const Ogre::LayerBlendModeEx S_alphaBlendMode =
{
    Ogre::LBT_ALPHA,
    Ogre::LBX_MODULATE,
    Ogre::LBS_TEXTURE,
    Ogre::LBS_DIFFUSE
};

//----------------------------------------------------------------------------//
static const Ogre::TextureUnitState::UVWAddressingMode S_textureAddressMode =
{
    Ogre::TextureUnitState::TAM_CLAMP,
    Ogre::TextureUnitState::TAM_CLAMP,
    Ogre::TextureUnitState::TAM_CLAMP
};

//----------------------------------------------------------------------------//
// Helper to allocate a vertex buffer and initialse a Ogre::RenderOperation
static void initialiseRenderOp(Ogre::RenderOperation& rop,
                               Ogre::HardwareVertexBufferSharedPtr& vb,
                               size_t count)
{
    using namespace Ogre;

    // basic initialisation of render op
    rop.vertexData = new VertexData;
    rop.operationType = RenderOperation::OT_TRIANGLE_LIST;
    rop.useIndexes = false;

    // setup vertex declaration for format we will use
    VertexDeclaration* vd = rop.vertexData->vertexDeclaration;
    size_t vd_offset = 0;
    vd->addElement(0, vd_offset, VET_FLOAT3, VES_POSITION);
    vd_offset += VertexElement::getTypeSize(VET_FLOAT3);
    vd->addElement(0, vd_offset, VET_COLOUR, VES_DIFFUSE);
    vd_offset += VertexElement::getTypeSize(VET_COLOUR);
    vd->addElement(0, vd_offset, VET_FLOAT2, VES_TEXTURE_COORDINATES);

    // create hardware vertex buffer
    vb = HardwareBufferManager::getSingleton().createVertexBuffer(
            vd->getVertexSize(0), count,
            HardwareBuffer::HBU_DYNAMIC_WRITE_ONLY_DISCARDABLE,
            false);

    // bind vertex buffer
    rop.vertexData->vertexBufferBinding->setBinding(0, vb);
}

//----------------------------------------------------------------------------//
// Helper to cleanup what initialiseRenderOp did.
static void cleanupRenderOp(Ogre::RenderOperation& rop,
                            Ogre::HardwareVertexBufferSharedPtr& vb)
{
    delete rop.vertexData;
    rop.vertexData = 0;
    vb.setNull();
}

//----------------------------------------------------------------------------//
OgreGeometryBuffer::OgreGeometryBuffer(OgreRenderer& owner,
                                       Ogre::RenderSystem& rs) :
    d_owner(owner),
    d_renderSystem(rs),
    d_activeTexture(0),
    d_translation(0, 0, 0),
    d_rotation(Quaternion::IDENTITY),
    d_pivot(0, 0, 0),
    d_effect(0),
    d_texelOffset(rs.getHorizontalTexelOffset(), rs.getVerticalTexelOffset()),
    d_matrixValid(false),
    d_sync(false)
{
    initialiseRenderOp(d_renderOp, d_hwBuffer, 64);
}

//----------------------------------------------------------------------------//
OgreGeometryBuffer::~OgreGeometryBuffer()
{
    cleanupRenderOp(d_renderOp, d_hwBuffer);
}

//----------------------------------------------------------------------------//
void OgreGeometryBuffer::draw() const
{
    // Set up clipping for this buffer
    d_renderSystem.setScissorTest(true, d_clipRect.d_left, d_clipRect.d_top,
                                  d_clipRect.d_right, d_clipRect.d_bottom);

    if (!d_sync)
        syncHardwareBuffer();

    if (!d_matrixValid)
        updateMatrix();

    d_renderSystem._setWorldMatrix(d_matrix);

    d_owner.setupRenderingBlendMode(d_blendMode);

    const int pass_count = d_effect ? d_effect->getPassCount() : 1;
    for (int pass = 0; pass < pass_count; ++pass)
    {
        // set up RenderEffect
        if (d_effect)
            d_effect->performPreRenderFunctions(pass);

        // draw the batches
        size_t pos = 0;
        BatchList::const_iterator i = d_batches.begin();
        for ( ; i != d_batches.end(); ++i)
        {
            d_renderOp.vertexData->vertexStart = pos;
            d_renderOp.vertexData->vertexCount = (*i).second;
            d_renderSystem._setTexture(0, true, (*i).first);
            initialiseTextureStates();
            d_renderSystem._render(d_renderOp);
            pos += (*i).second;
        }
    }

    // clean up RenderEffect
    if (d_effect)
        d_effect->performPostRenderFunctions();
}

//----------------------------------------------------------------------------//
void OgreGeometryBuffer::setTranslation(const Vector3& v)
{
    d_translation = v;
    d_matrixValid = false;
}

//----------------------------------------------------------------------------//
void OgreGeometryBuffer::setRotation(const Quaternion& r)
{
    d_rotation = r;
    d_matrixValid = false;
}

//----------------------------------------------------------------------------//
void OgreGeometryBuffer::setPivot(const Vector3& p)
{
    d_pivot = p;
    d_matrixValid = false;
}

//----------------------------------------------------------------------------//
void OgreGeometryBuffer::setClippingRegion(const Rect& region)
{
    d_clipRect.d_top    = PixelAligned(region.d_top);
    d_clipRect.d_bottom = PixelAligned(region.d_bottom);
    d_clipRect.d_left   = PixelAligned(region.d_left);
    d_clipRect.d_right  = PixelAligned(region.d_right);
}

//----------------------------------------------------------------------------//
void OgreGeometryBuffer::appendVertex(const Vertex& vertex)
{
    appendGeometry(&vertex, 1);
}

//----------------------------------------------------------------------------//
void OgreGeometryBuffer::appendGeometry(const Vertex* const vbuff,
                                        uint vertex_count)
{
    // see if we should start a new batch
    Ogre::TexturePtr t;
    if (d_activeTexture)
        t = d_activeTexture->getOgreTexture();

    if (d_batches.empty() || d_batches.back().first != t)
        d_batches.push_back(BatchInfo(t, 0));

    // update size of current batch
    d_batches.back().second += vertex_count;

    // buffer these vertices
    OgreVertex v;
    for (uint i = 0; i < vertex_count; ++i)
    {
        const Vertex& vs = vbuff[i];
        // convert from CEGUI::Vertex to something directly usable by Ogre.
        v.x       = vs.position.d_x + d_texelOffset.d_x;
        v.y       = vs.position.d_y + d_texelOffset.d_y;
        v.z       = vs.position.d_z;
        v.diffuse = colourToOgre(vs.colour_val);
        v.u       = vs.tex_coords.d_x;
        v.v       = vs.tex_coords.d_y;

        d_vertices.push_back(v);
    }

    d_sync = false;
}

//----------------------------------------------------------------------------//
void OgreGeometryBuffer::setActiveTexture(Texture* texture)
{
    d_activeTexture = static_cast<OgreTexture*>(texture);
}

//----------------------------------------------------------------------------//
void OgreGeometryBuffer::reset()
{
    d_vertices.clear();
    d_batches.clear();
    d_activeTexture = 0;
    d_sync = false;
}

//----------------------------------------------------------------------------//
Texture* OgreGeometryBuffer::getActiveTexture() const
{
    return d_activeTexture;
}

//----------------------------------------------------------------------------//
uint OgreGeometryBuffer::getVertexCount() const
{
    return d_vertices.size();
}

//----------------------------------------------------------------------------//
uint OgreGeometryBuffer::getBatchCount() const
{
    return d_batches.size();
}

//----------------------------------------------------------------------------//
void OgreGeometryBuffer::setRenderEffect(RenderEffect* effect)
{
    d_effect = effect;
}

//----------------------------------------------------------------------------//
RenderEffect* OgreGeometryBuffer::getRenderEffect()
{
    return d_effect;
}

//----------------------------------------------------------------------------//
Ogre::RGBA OgreGeometryBuffer::colourToOgre(const Colour& col) const
{
    Ogre::ColourValue ocv(col.getRed(),
                          col.getGreen(),
                          col.getBlue(),
                          col.getAlpha());

    uint32 final;
    d_renderSystem.convertColourValue(ocv, &final);

    return final;
}

//----------------------------------------------------------------------------//
void OgreGeometryBuffer::updateMatrix() const
{
    // translation to position geometry and offset to pivot point
    Ogre::Matrix4 trans;
    trans.makeTrans(d_translation.d_x + d_pivot.d_x,
                    d_translation.d_y + d_pivot.d_y,
                    d_translation.d_z + d_pivot.d_z);

    // rotation
    Ogre::Matrix4 rot(Ogre::Quaternion(
        d_rotation.d_w, d_rotation.d_x, d_rotation.d_y, d_rotation.d_z));

    // translation to remove rotation pivot offset
    Ogre::Matrix4 inv_pivot_trans;
    inv_pivot_trans.makeTrans(-d_pivot.d_x, -d_pivot.d_y, -d_pivot.d_z);

    // calculate final matrix
    d_matrix = trans * rot * inv_pivot_trans;

    d_matrixValid = true;
}

//----------------------------------------------------------------------------//
void OgreGeometryBuffer::syncHardwareBuffer() const
{
    // Reallocate h/w buffer as requied
    size_t size = d_hwBuffer->getNumVertices();
    const size_t required_size = d_vertices.size();
    if(size < required_size)
    {
        // calculate new size to use
        while(size < required_size)
            size *= 2;

        // Reallocate the buffer
        cleanupRenderOp(d_renderOp, d_hwBuffer);
        initialiseRenderOp(d_renderOp, d_hwBuffer, size);
    }

    // copy vertex data into hw buffer
    if (required_size > 0)
    {
        std::memcpy(d_hwBuffer->lock(Ogre::HardwareVertexBuffer::HBL_DISCARD),
                    &d_vertices[0], sizeof(OgreVertex) * d_vertices.size());

        d_hwBuffer->unlock();
    }

    d_sync = true;
}

//----------------------------------------------------------------------------//
const Ogre::Matrix4& OgreGeometryBuffer::getMatrix() const
{
    if (!d_matrixValid)
        updateMatrix();

    return d_matrix;
}

//----------------------------------------------------------------------------//
void OgreGeometryBuffer::initialiseTextureStates() const
{
    using namespace Ogre;
    d_renderSystem._setTextureCoordCalculation(0, TEXCALC_NONE);
    d_renderSystem._setTextureCoordSet(0, 0);
    d_renderSystem._setTextureUnitFiltering(0, FO_LINEAR, FO_LINEAR, FO_POINT);
    d_renderSystem._setTextureAddressingMode(0, S_textureAddressMode);
    d_renderSystem._setTextureMatrix(0, Matrix4::IDENTITY);
    d_renderSystem._setAlphaRejectSettings(CMPF_ALWAYS_PASS, 0, false);
    d_renderSystem._setTextureBlendMode(0, S_colourBlendMode);
    d_renderSystem._setTextureBlendMode(0, S_alphaBlendMode);
    d_renderSystem._disableTextureUnitsFrom(1);
}

//----------------------------------------------------------------------------//

} // End of  CEGUI namespace section
