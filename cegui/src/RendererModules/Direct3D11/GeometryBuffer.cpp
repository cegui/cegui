/***********************************************************************
    created:    Wed May 5 2010
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
#include "CEGUI/RendererModules/Direct3D11/GeometryBuffer.h"
#include "CEGUI/RendererModules/Direct3D11/Texture.h"
#include "CEGUI/RenderEffect.h"
#include "CEGUI/Vertex.h"
#include "CEGUI/Exceptions.h"

// Start of CEGUI namespace section
namespace CEGUI
{
//----------------------------------------------------------------------------//
Direct3D11GeometryBuffer::Direct3D11GeometryBuffer(Direct3D11Renderer& owner) :
    d_owner(owner),
    d_device(d_owner.getDirect3DDevice()),
    d_activeTexture(0),
    d_vertexBuffer(0),
    d_bufferSize(0),
    d_bufferSynched(false),
    d_clipRect(0, 0, 0, 0),
    d_clippingActive(true),
    d_translation(0, 0, 0),
    d_rotation(0, 0, 0),
    d_pivot(0, 0, 0),
    d_effect(0),
    d_matrixValid(false)
{
}

//----------------------------------------------------------------------------//
Direct3D11GeometryBuffer::~Direct3D11GeometryBuffer()
{
    cleanupVertexBuffer();
}

//----------------------------------------------------------------------------//
void Direct3D11GeometryBuffer::draw(uint32 drawModeMask) const
{
    CEGUI_UNUSED(drawModeMask):
    
    // setup clip region
    D3D11_RECT clip;
    clip.left   = static_cast<LONG>(d_clipRect.left());
    clip.top    = static_cast<LONG>(d_clipRect.top());
    clip.right  = static_cast<LONG>(d_clipRect.right());
    clip.bottom = static_cast<LONG>(d_clipRect.bottom());
    d_device.d_context->RSSetScissorRects(1, &clip);

    if (!d_bufferSynched)
        syncHardwareBuffer();

    // apply the transformations we need to use.
    if (!d_matrixValid)
        updateMatrix();

    d_owner.setWorldMatrix(d_matrix);

    // set our buffer as the vertex source.
    const UINT stride = sizeof(D3DVertex);
    const UINT offset = 0;
    d_device.d_context->IASetVertexBuffers(0, 1, &d_vertexBuffer, &stride, &offset);

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
            // Set Texture
            d_owner.setCurrentTextureShaderResource(
                    const_cast<ID3D11ShaderResourceView*>(i->texture));
            // Draw this batch
            d_owner.bindTechniquePass(d_blendMode, i->clip);
            d_device.d_context->Draw(i->vertexCount, pos);
            pos += i->vertexCount;
        }
    }

    // clean up RenderEffect
    if (d_effect)
        d_effect->performPostRenderFunctions();
}

//----------------------------------------------------------------------------//
void Direct3D11GeometryBuffer::setTranslation(const Vector3f& v)
{
    d_translation = v;
    d_matrixValid = false;
}

//----------------------------------------------------------------------------//
void Direct3D11GeometryBuffer::setRotation(const Quaternion& r)
{
    d_rotation = r;
    d_matrixValid = false;
}

//----------------------------------------------------------------------------//
void Direct3D11GeometryBuffer::setPivot(const Vector3f& p)
{
    d_pivot = p;
    d_matrixValid = false;
}

//----------------------------------------------------------------------------//
void Direct3D11GeometryBuffer::setClippingRegion(const Rectf& region)
{
    d_clipRect.top(ceguimax(0.0f, region.top()));
    d_clipRect.bottom(ceguimax(0.0f, region.bottom()));
    d_clipRect.left(ceguimax(0.0f, region.left()));
    d_clipRect.right(ceguimax(0.0f, region.right()));
}

//----------------------------------------------------------------------------//
void Direct3D11GeometryBuffer::appendVertex(const Vertex& vertex)
{
    appendGeometry(&vertex, 1);
}

//----------------------------------------------------------------------------//
void Direct3D11GeometryBuffer::appendGeometry(const Vertex* const vbuff,
                                              uint vertex_count)
{
    const ID3D11ShaderResourceView* srv =
        d_activeTexture ? d_activeTexture->getDirect3DShaderResourceView() : 0;

    // create a new batch if there are no batches yet, or if the active texture
    // differs from that used by the current batch.
    if (d_batches.empty() ||
        srv != d_batches.back().texture ||
        d_clippingActive != d_batches.back().clip)
    {
        BatchInfo batch = {srv, 0, d_clippingActive};
        d_batches.push_back(batch);
    }

    // update size of current batch
    d_batches.back().vertexCount += vertex_count;

    // buffer these vertices
    D3DVertex vd;
    const Vertex* vs = vbuff;
    for (uint i = 0; i < vertex_count; ++i, ++vs)
    {
        // copy vertex info the buffer, converting from CEGUI::Vertex to
        // something directly usable by D3D as needed.
        vd.x       = vs->position.d_x;
        vd.y       = vs->position.d_y;
        vd.z       = vs->position.d_z;
        vd.diffuse = vs->colour_val.getARGB();
        vd.tu      = vs->tex_coords.d_x;
        vd.tv      = vs->tex_coords.d_y;
        d_vertices.push_back(vd);
    }

    d_bufferSynched = false;
}

//----------------------------------------------------------------------------//
void Direct3D11GeometryBuffer::setActiveTexture(Texture* texture)
{
    d_activeTexture = static_cast<Direct3D11Texture*>(texture);
}

//----------------------------------------------------------------------------//
void Direct3D11GeometryBuffer::reset()
{
    d_batches.clear();
    d_vertices.clear();
    d_activeTexture = 0;
}

//----------------------------------------------------------------------------//
Texture* Direct3D11GeometryBuffer::getActiveTexture() const
{
    return d_activeTexture;
}

//----------------------------------------------------------------------------//
uint Direct3D11GeometryBuffer::getVertexCount() const
{
    return d_vertices.size();
}

//----------------------------------------------------------------------------//
uint Direct3D11GeometryBuffer::getBatchCount() const
{
    return d_batches.size();
}

//----------------------------------------------------------------------------//
void Direct3D11GeometryBuffer::setRenderEffect(RenderEffect* effect)
{
    d_effect = effect;
}

//----------------------------------------------------------------------------//
RenderEffect* Direct3D11GeometryBuffer::getRenderEffect()
{
    return d_effect;
}

//----------------------------------------------------------------------------//
void Direct3D11GeometryBuffer::updateMatrix() const
{
    const D3DXVECTOR3 p(d_pivot.d_x, d_pivot.d_y, d_pivot.d_z);
    const D3DXVECTOR3 t(d_translation.d_x,
                        d_translation.d_y,
                        d_translation.d_z);

    D3DXQUATERNION r;
    r.x = d_rotation.d_x;
    r.y = d_rotation.d_y;
    r.z = d_rotation.d_z;
    r.w = d_rotation.d_w;

    D3DXMatrixTransformation(&d_matrix, 0, 0, 0, &p, &r, &t);

    d_matrixValid = true;
}

//----------------------------------------------------------------------------//
const D3DXMATRIX* Direct3D11GeometryBuffer::getMatrix() const
{
    if (!d_matrixValid)
        updateMatrix();

    return &d_matrix;
}

//----------------------------------------------------------------------------//
void Direct3D11GeometryBuffer::syncHardwareBuffer() const
{
    const size_t vertex_count = d_vertices.size();

    if (vertex_count > d_bufferSize)
    {
        cleanupVertexBuffer();
        allocateVertexBuffer(vertex_count);
    }

    if (vertex_count > 0)
    {
        void* buff;
		D3D11_MAPPED_SUBRESOURCE SubRes;
        if (FAILED(d_device.d_context->Map(d_vertexBuffer,0,D3D11_MAP_WRITE_DISCARD, 0, &SubRes)))
            CEGUI_THROW(RendererException("failed to map buffer."));
		buff=SubRes.pData;

        std::memcpy(buff, &d_vertices[0], sizeof(D3DVertex) * vertex_count);
        d_device.d_context->Unmap(d_vertexBuffer,0);
    }

    d_bufferSynched = true;
}

//----------------------------------------------------------------------------//
void Direct3D11GeometryBuffer::allocateVertexBuffer(const size_t count) const
{
    D3D11_BUFFER_DESC buffer_desc;
    buffer_desc.Usage          = D3D11_USAGE_DYNAMIC;
    buffer_desc.ByteWidth      = count * sizeof(D3DVertex);
    buffer_desc.BindFlags      = D3D11_BIND_VERTEX_BUFFER;
    buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    buffer_desc.MiscFlags      = 0;

    if (FAILED(d_device.d_device->CreateBuffer(&buffer_desc, 0, &d_vertexBuffer)))
        CEGUI_THROW(RendererException("failed to allocate vertex buffer."));

    d_bufferSize = count;
}

//----------------------------------------------------------------------------//
void Direct3D11GeometryBuffer::cleanupVertexBuffer() const
{
    if (d_vertexBuffer)
    {
        d_vertexBuffer->Release();
        d_vertexBuffer = 0;
        d_bufferSize = 0;
    }
}

//----------------------------------------------------------------------------//
void Direct3D11GeometryBuffer::setClippingActive(const bool active)
{
    d_clippingActive = active;
}

//----------------------------------------------------------------------------//
bool Direct3D11GeometryBuffer::isClippingActive() const
{
    return d_clippingActive;
}

//----------------------------------------------------------------------------//

} // End of  CEGUI namespace section

