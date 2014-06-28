/***********************************************************************
    created:    Fri Jan 15 20109
    author:     Eugene Marcotte
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
#include "CEGUI/RendererModules/Null/GeometryBuffer.h"
#include "CEGUI/RendererModules/Null/Texture.h"
#include "CEGUI/Vertex.h"
#include "CEGUI/RenderEffect.h"

// Start of CEGUI namespace section
namespace CEGUI
{
//----------------------------------------------------------------------------//
NullGeometryBuffer::NullGeometryBuffer() :
    d_activeTexture(0),
    d_clipRect(0, 0, 0, 0),
    d_clippingActive(true),
    d_translation(0, 0, 0),
    d_rotation(),
    d_pivot(0, 0, 0),
    d_effect(0)
{
}

//----------------------------------------------------------------------------//
NullGeometryBuffer::~NullGeometryBuffer()
{
}

//----------------------------------------------------------------------------//
void NullGeometryBuffer::draw() const
{
    const int pass_count = d_effect ? d_effect->getPassCount() : 1;
    for (int pass = 0; pass < pass_count; ++pass)
    {
        // set up RenderEffect
        if (d_effect)
            d_effect->performPreRenderFunctions(pass);
    }

    // clean up RenderEffect
    if (d_effect)
        d_effect->performPostRenderFunctions();
}

//----------------------------------------------------------------------------//
void NullGeometryBuffer::setTranslation(const Vector3f& v)
{
    d_translation = v;
}

//----------------------------------------------------------------------------//
void NullGeometryBuffer::setRotation(const Quaternion& r)
{
    d_rotation = r;
}

//----------------------------------------------------------------------------//
void NullGeometryBuffer::setPivot(const Vector3f& p)
{
    d_pivot = p;
}

//----------------------------------------------------------------------------//
void NullGeometryBuffer::setClippingRegion(const Rectf& region)
{
    d_clipRect.top(ceguimax(0.0f, region.top()));
    d_clipRect.bottom(ceguimax(0.0f, region.bottom()));
    d_clipRect.left(ceguimax(0.0f, region.left()));
    d_clipRect.right(ceguimax(0.0f, region.right()));
}

//----------------------------------------------------------------------------//
void NullGeometryBuffer::appendVertex(const Vertex& vertex)
{
    appendGeometry(&vertex, 1);
}

//----------------------------------------------------------------------------//
void NullGeometryBuffer::appendGeometry(const Vertex* const vbuff,
                                        uint vertex_count)
{
    // buffer these vertices
    for (uint i = 0; i < vertex_count; ++i)
    {
        d_vertices.push_back(vbuff[i]);
    }
}

//----------------------------------------------------------------------------//
void NullGeometryBuffer::setActiveTexture(Texture* texture)
{
    d_activeTexture = static_cast<NullTexture*>(texture);
}

//----------------------------------------------------------------------------//
void NullGeometryBuffer::reset()
{
    d_vertices.clear();
}

//----------------------------------------------------------------------------//
Texture* NullGeometryBuffer::getActiveTexture() const
{
    return d_activeTexture;
}

//----------------------------------------------------------------------------//
uint NullGeometryBuffer::getVertexCount() const
{
    return d_vertices.size();
}

//----------------------------------------------------------------------------//
uint NullGeometryBuffer::getBatchCount() const
{
    return 1;
}

//----------------------------------------------------------------------------//
void NullGeometryBuffer::setRenderEffect(RenderEffect* effect)
{
    d_effect = effect;
}

//----------------------------------------------------------------------------//
RenderEffect* NullGeometryBuffer::getRenderEffect()
{
    return d_effect;
}

//----------------------------------------------------------------------------//
void NullGeometryBuffer::setClippingActive(const bool active)
{
    d_clippingActive = active;
}

//----------------------------------------------------------------------------//
bool NullGeometryBuffer::isClippingActive() const
{
    return d_clippingActive;
}

//----------------------------------------------------------------------------//

} // End of  CEGUI namespace section
