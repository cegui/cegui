/***********************************************************************
    created:    Tue Mar 3 2009
    author:     Paul D Turner (parts based on original code by Thomas Suter)
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2011 Paul D Turner & The CEGUI Development Team
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
#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "CEGUI/RendererModules/Irrlicht/GeometryBuffer.h"
#include "CEGUI/RenderEffect.h"
#include "CEGUI/RendererModules/Irrlicht/Texture.h"
#include "CEGUI/Vertex.h"
#include "CEGUI/Quaternion.h"

// Start of CEGUI namespace section
namespace CEGUI
{
//----------------------------------------------------------------------------//
IrrlichtGeometryBuffer::IrrlichtGeometryBuffer(irr::video::IVideoDriver& driver):
    d_driver(driver),
    d_activeTexture(0),
    d_clipRect(0, 0, 0, 0),
    d_clippingActive(true),
    d_translation(0, 0, 0),
    d_rotation(0, 0, 0),
    d_pivot(0, 0, 0),
    d_effect(0),
    d_matrixValid(false),
#if IRRLICHT_VERSION_MAJOR > 1 || (IRRLICHT_VERSION_MAJOR == 1 && IRRLICHT_VERSION_MINOR >= 8)
    d_xViewDir(1.0f),
#else
    d_xViewDir(driver.getDriverType() != irr::video::EDT_OPENGL ? 1.0f : -1.0f),
#endif
    d_texelOffset(driver.getDriverType() != irr::video::EDT_OPENGL ? -0.5f : 0.0f)
{
    d_material.BackfaceCulling = false;
    d_material.Lighting = false;
    d_material.ZBuffer = 0;
    d_material.ZWriteEnable = false;

    // force upate of blending options to suit the default 'normal' mode
    d_blendMode = BM_INVALID;
    setBlendMode(BM_NORMAL);
}

//----------------------------------------------------------------------------//
void IrrlichtGeometryBuffer::draw() const
{
    if (!d_matrixValid)
        updateMatrix();

    d_driver.setTransform(irr::video::ETS_WORLD, d_matrix);

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
            if (i->clip)
                setupClipping();

            d_material.setTexture(0, i->texture);
            d_driver.setMaterial(d_material);
            d_driver.drawIndexedTriangleList(&d_vertices[pos], i->vertexCount,
                                             &d_indices[pos], i->vertexCount / 3);
            pos += i->vertexCount;

            if (i->clip)
                cleanupClipping();
        }
    }

    // clean up RenderEffect
    if (d_effect)
        d_effect->performPostRenderFunctions();
}

//----------------------------------------------------------------------------//
void IrrlichtGeometryBuffer::setupClipping() const
{
    // NB: This is done via viewport & projection manipulation because Irrlicht
    // does not expose scissoring facilities of underlying APIs.  This has the
    // unfortunate side effect of being much more expensive to set up.
    d_savedViewport = d_driver.getViewPort();
    d_savedProjection = d_driver.getTransform(irr::video::ETS_PROJECTION);

    const Sizef csz(d_clipRect.getSize());
    const Sizef tsz(static_cast<float>(d_savedViewport.getWidth()),
                     static_cast<float>(d_savedViewport.getHeight()));

    // set modified projection 'scissor' matix that negates scale and
    // translation that would be done by setting the viewport to the clip area.
    irr::core::matrix4 scsr(irr::core::matrix4::EM4CONST_IDENTITY);
    scsr(0, 0) = tsz.d_width / csz.d_width;
    scsr(1, 1) = tsz.d_height / csz.d_height;
    scsr(3, 0) = d_xViewDir * (tsz.d_width + 2.0f *
                   (d_savedViewport.UpperLeftCorner.X -
                     (d_clipRect.left() + csz.d_width * 0.5f))) / csz.d_width;
    scsr(3, 1) = -(tsz.d_height + 2.0f *
                   (d_savedViewport.UpperLeftCorner.Y -
                     (d_clipRect.top() + csz.d_height * 0.5f))) / csz.d_height;

    scsr *= d_savedProjection;
    d_driver.setTransform(irr::video::ETS_PROJECTION, scsr);

    // set new viewport for the clipping area
    const irr::core::rect<irr::s32> vp(
            static_cast<irr::s32>(d_clipRect.left()),
            static_cast<irr::s32>(d_clipRect.top()),
            static_cast<irr::s32>(d_clipRect.right()),
            static_cast<irr::s32>(d_clipRect.bottom()));
    d_driver.setViewPort(vp);
}

//----------------------------------------------------------------------------//
void IrrlichtGeometryBuffer::cleanupClipping() const
{
    d_driver.setTransform(irr::video::ETS_PROJECTION, d_savedProjection);
    d_driver.setViewPort(d_savedViewport);
}

//----------------------------------------------------------------------------//
void IrrlichtGeometryBuffer::setTranslation(const Vector3f& v)
{
    d_translation.X = v.d_x;
    d_translation.Y = v.d_y;
    d_translation.Z = v.d_z;
    d_matrixValid = false;
}

//----------------------------------------------------------------------------//
void IrrlichtGeometryBuffer::setRotation(const Quaternion& r)
{
    d_rotation.W = -r.d_w;
    d_rotation.X = r.d_x;
    d_rotation.Y = r.d_y;
    d_rotation.Z = r.d_z;
    d_matrixValid = false;
}

//----------------------------------------------------------------------------//
void IrrlichtGeometryBuffer::setPivot(const Vector3f& p)
{
    d_pivot.X = p.d_x;
    d_pivot.Y = p.d_y;
    d_pivot.Z = p.d_z;
    d_matrixValid = false;
}

//----------------------------------------------------------------------------//
void IrrlichtGeometryBuffer::setClippingRegion(const Rectf& region)
{
    d_clipRect.top(ceguimax(0.0f, region.top()));
    d_clipRect.bottom(ceguimax(0.0f, region.bottom()));
    d_clipRect.left(ceguimax(0.0f, region.left()));
    d_clipRect.right(ceguimax(0.0f, region.right()));
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

    if (d_batches.empty() ||
        d_batches.back().texture != t ||
        d_batches.back().clip != d_clippingActive)
    {
        const BatchInfo batch = {t, 0, d_clippingActive};
        d_batches.push_back(batch);
    }

    // buffer these vertices
    const irr::u16 idx_start = d_batches.back().vertexCount;
    irr::video::S3DVertex v;
    for (uint i = 0; i < vertex_count; ++i)
    {
        const Vertex& vs = vbuff[i];
        v.Pos.X     = vs.position.d_x + d_texelOffset;
        v.Pos.Y     = vs.position.d_y + d_texelOffset;
        v.Pos.Z     = vs.position.d_z;
        v.TCoords.X = vs.tex_coords.d_x;
        v.TCoords.Y = vs.tex_coords.d_y;
        v.Color.set(vs.colour_val.getARGB());
        d_vertices.push_back(v);
        d_indices.push_back(idx_start + i);
    }

    // update size of current batch
    d_batches.back().vertexCount += vertex_count;
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
void IrrlichtGeometryBuffer::setBlendMode(const BlendMode mode)
{
    // if blend mode is already set to this, ignore.
    if (d_blendMode == mode)
        return;

    // call default to set mode field (in case we change how that's done)
    GeometryBuffer::setBlendMode(mode);

#if CEGUI_IRR_SDK_VERSION >= 16
    // FIXME: Here we just use the 'best of a bad situation' option
    // FIXME: which results in incorrect accumulation of alpha values
    // FIXME: in texture based targets.  There is no fix for this that is
    // FIXME: possible with the stock Irrlicht; while we could creata an
    // FIXME: appropriate custom material, it would involve directly linking
    // FIXME: with the various underlying rendering APIs - which is fine for
    // FIXME: an end user, but is definitely not what _we_ want to be doing.
    //
    // If anybody knows the above information to be incorrect, and has a fix
    // for this issue, please let us know! :)

/*    if (d_blendMode == BM_RTT_PREMULTIPLIED)
    {
        d_material.MaterialType = irr::video::EMT_ONETEXTURE_BLEND;
        d_material.MaterialTypeParam = irr::video::pack_texureBlendFunc(
                irr::video::EBF_ONE,
                irr::video::EBF_ONE_MINUS_SRC_ALPHA,
                irr::video::EMFN_MODULATE_1X,
                irr::video::EAS_NONE);
    }
    else */
    {
        d_material.MaterialType = irr::video::EMT_ONETEXTURE_BLEND;
        d_material.MaterialTypeParam =
#if IRRLICHT_VERSION_MAJOR > 1 || (IRRLICHT_VERSION_MAJOR == 1 && IRRLICHT_VERSION_MINOR >= 8)
            irr::video::pack_textureBlendFunc(
#else
            irr::video::pack_texureBlendFunc(
#endif
                irr::video::EBF_SRC_ALPHA,
                irr::video::EBF_ONE_MINUS_SRC_ALPHA,
                irr::video::EMFN_MODULATE_1X,
                irr::video::EAS_NONE);
    }
#else
    d_material.MaterialType = irr::video::EMT_TRANSPARENT_ALPHA_CHANNEL;
    d_material.MaterialTypeParam = 0;
#endif
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

    irr::core::matrix4 ptrans;
    ptrans.setTranslation(-d_pivot);

    d_matrix *= d_rotation.getMatrix();
    d_matrix *= ptrans;

    d_matrixValid = true;
}

//----------------------------------------------------------------------------//
irr::video::SMaterial& IrrlichtGeometryBuffer::getMaterial()
{
    return const_cast<irr::video::SMaterial&>(
        static_cast<const IrrlichtGeometryBuffer*>(this)->getMaterial());
}

//----------------------------------------------------------------------------//
const irr::video::SMaterial& IrrlichtGeometryBuffer::getMaterial() const
{
    return d_material;
}

//----------------------------------------------------------------------------//
void IrrlichtGeometryBuffer::setClippingActive(const bool active)
{
    d_clippingActive = active;
}

//----------------------------------------------------------------------------//
bool IrrlichtGeometryBuffer::isClippingActive() const
{
    return d_clippingActive;
}

//----------------------------------------------------------------------------//
//
} // End of  CEGUI namespace section
