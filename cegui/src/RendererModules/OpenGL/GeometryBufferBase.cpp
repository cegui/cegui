/***********************************************************************
    created:    Tue Apr 30 2013
    authors:    Paul D Turner <paul@cegui.org.uk>
                Lukas E Meindl
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2013 Paul D Turner & The CEGUI Development Team
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
#include "CEGUI/RendererModules/OpenGL/GL.h"
#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "CEGUI/RendererModules/OpenGL/GeometryBufferBase.h"
#include "CEGUI/RenderEffect.h"
#include "CEGUI/RendererModules/OpenGL/Texture.h"
#include "CEGUI/Vertex.h"
#include "CEGUI/RendererModules/OpenGL/GlmPimpl.h"

namespace CEGUI
{
//----------------------------------------------------------------------------//
OpenGLGeometryBufferBase::OpenGLGeometryBufferBase(OpenGLRendererBase& owner) :
    d_owner(&owner),
    d_activeTexture(0),
    d_clipRect(0, 0, 0, 0),
    d_clippingActive(true),
    d_translation(0, 0, 0),
    d_rotation(Quaternion::IDENTITY),
    d_pivot(0, 0, 0),
    d_effect(0),
    d_matrix(new mat4Pimpl()),
    d_matrixValid(false)
{
}

//----------------------------------------------------------------------------//
OpenGLGeometryBufferBase::~OpenGLGeometryBufferBase()
{
    delete d_matrix;
}

//----------------------------------------------------------------------------//
void OpenGLGeometryBufferBase::appendVertex(const Vertex& vertex)
{
    appendGeometry(&vertex, 1);
}

//----------------------------------------------------------------------------//
void OpenGLGeometryBufferBase::setTranslation(const Vector3f& v)
{
    d_translation = v;
    d_matrixValid = false;
}

//----------------------------------------------------------------------------//
void OpenGLGeometryBufferBase::setRotation(const Quaternion& r)
{
    d_rotation = r;
    d_matrixValid = false;
}

//----------------------------------------------------------------------------//
void OpenGLGeometryBufferBase::setPivot(const Vector3f& p)
{
    d_pivot = Vector3f(p.d_x, p.d_y, p.d_z);
    d_matrixValid = false;
}

//----------------------------------------------------------------------------//
void OpenGLGeometryBufferBase::setClippingRegion(const Rectf& region)
{
    d_clipRect.top(ceguimax(0.0f, region.top()));
    d_clipRect.left(ceguimax(0.0f, region.left()));
    d_clipRect.bottom(ceguimax(0.0f, region.bottom()));
    d_clipRect.right(ceguimax(0.0f, region.right()));
}

//----------------------------------------------------------------------------//
void OpenGLGeometryBufferBase::appendGeometry(const Vertex* const vbuff,
    uint vertex_count)
{
    performBatchManagement();

    // update size of current batch
    d_batches.back().vertexCount += vertex_count;

    // buffer these vertices
    GLVertex vd;
    const Vertex* vs = vbuff;
    for (uint i = 0; i < vertex_count; ++i, ++vs)
    {
        // copy vertex info the buffer, converting from CEGUI::Vertex to
        // something directly usable by OpenGL as needed.
        vd.tex[0]      = vs->tex_coords.d_x;
        vd.tex[1]      = vs->tex_coords.d_y;
        vd.colour[0]   = vs->colour_val.getRed();
        vd.colour[1]   = vs->colour_val.getGreen();
        vd.colour[2]   = vs->colour_val.getBlue();
        vd.colour[3]   = vs->colour_val.getAlpha();
        vd.position[0] = vs->position.d_x;
        vd.position[1] = vs->position.d_y;
        vd.position[2] = vs->position.d_z;
        d_vertices.push_back(vd);
    }
}

//----------------------------------------------------------------------------//
void OpenGLGeometryBufferBase::setActiveTexture(Texture* texture)
{
    d_activeTexture = static_cast<OpenGLTexture*>(texture);
}

//----------------------------------------------------------------------------//
void OpenGLGeometryBufferBase::reset()
{
    d_batches.clear();
    d_vertices.clear();
    d_activeTexture = 0;
}

//----------------------------------------------------------------------------//
Texture* OpenGLGeometryBufferBase::getActiveTexture() const
{
    return d_activeTexture;
}

//----------------------------------------------------------------------------//
uint OpenGLGeometryBufferBase::getVertexCount() const
{
    return d_vertices.size();
}

//----------------------------------------------------------------------------//
uint OpenGLGeometryBufferBase::getBatchCount() const
{
    return d_batches.size();
}

//----------------------------------------------------------------------------//
void OpenGLGeometryBufferBase::performBatchManagement()
{
    const GLuint gltex = d_activeTexture ?
                            d_activeTexture->getOpenGLTexture() : 0;

    // create a new batch if there are no batches yet, or if the active texture
    // differs from that used by the current batch.
    if (d_batches.empty() ||
        gltex != d_batches.back().texture ||
        d_clippingActive != d_batches.back().clip)
    {
        const BatchInfo batch = {gltex, 0, d_clippingActive};
        d_batches.push_back(batch);
    }
}

//----------------------------------------------------------------------------//
void OpenGLGeometryBufferBase::setRenderEffect(RenderEffect* effect)
{
    d_effect = effect;
}

//----------------------------------------------------------------------------//
RenderEffect* OpenGLGeometryBufferBase::getRenderEffect()
{
    return d_effect;
}

//----------------------------------------------------------------------------//
const mat4Pimpl* OpenGLGeometryBufferBase::getMatrix() const
{
    if (!d_matrixValid)
        updateMatrix();

    return d_matrix;
}

//----------------------------------------------------------------------------//
void OpenGLGeometryBufferBase::updateMatrix() const
{
    glm::mat4& modelMatrix = d_matrix->d_matrix;
    modelMatrix = glm::mat4(1.f);

    const glm::vec3 final_trans(d_translation.d_x + d_pivot.d_x,
                                d_translation.d_y + d_pivot.d_y,
                                d_translation.d_z + d_pivot.d_z);

    modelMatrix = glm::translate(modelMatrix, final_trans);

    glm::quat rotationQuat = glm::quat(d_rotation.d_w, d_rotation.d_x, d_rotation.d_y, d_rotation.d_z);
    glm::mat4 rotation_matrix = glm::mat4_cast(rotationQuat);

    modelMatrix = modelMatrix * rotation_matrix;

    glm::vec3 transl = glm::vec3(-d_pivot.d_x, -d_pivot.d_y, -d_pivot.d_z);
    glm::mat4 translMatrix = glm::translate(glm::mat4(1.f), transl);
    modelMatrix =  modelMatrix * translMatrix;

    d_matrixValid = true;
}

//----------------------------------------------------------------------------//
void OpenGLGeometryBufferBase::setClippingActive(const bool active)
{
    d_clippingActive = active;
}

//----------------------------------------------------------------------------//
bool OpenGLGeometryBufferBase::isClippingActive() const
{
    return d_clippingActive;
}

//----------------------------------------------------------------------------//

}

