/***********************************************************************
    filename:   CEGUIOpenGLGeometryBuffer.cpp
    created:    Thu Jan 8 2009
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
#include <GL/glew.h>
#include "CEGUIOpenGLGeometryBuffer.h"
#include "CEGUIRenderEffect.h"
#include "CEGUIOpenGLTexture.h"

// Start of CEGUI namespace section
namespace CEGUI
{
//----------------------------------------------------------------------------//
OpenGLGeometryBuffer::OpenGLGeometryBuffer() :
    d_activeTexture(0),
    d_hasCustomMatrix(false),
    d_translation(0, 0, 0),
    d_rotation(0, 0, 0),
    d_pivot(0, 0, 0),
    d_effect(0),
    d_matrixValid(false)
{
}

//----------------------------------------------------------------------------//
void OpenGLGeometryBuffer::appendVertex(const Vertex& vertex)
{
    appendGeometry(&vertex, 1);
}

//----------------------------------------------------------------------------//
void OpenGLGeometryBuffer::draw() const
{
    // setup clip region
    GLint vp[4];
    glGetIntegerv(GL_VIEWPORT, vp);
    glScissor(static_cast<GLint>(d_clipRect.d_left),
              static_cast<GLint>(vp[3] - d_clipRect.d_bottom),
              static_cast<GLint>(d_clipRect.getWidth()),
              static_cast<GLint>(d_clipRect.getHeight()));

    // apply the transformations we need to use.
    if (!d_matrixValid)
        updateMatrix();

    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixd(d_matrix);

    // set up RenderEffect
    if (d_effect)
        d_effect->performPreRenderFunctions();

    // draw the batches
    BatchList::const_iterator i = d_batches.begin();
    for ( ; i != d_batches.end(); ++i)
        (*i).draw();

    // clean up RenderEffect
    if (d_effect)
        d_effect->performPostRenderFunctions();
}

//----------------------------------------------------------------------------//
void OpenGLGeometryBuffer::setTransform(const float* matrix)
{
    for (int i = 0; i < 16; ++i)
        d_xform[i] = matrix[i];

    d_hasCustomMatrix = true;
    d_matrixValid = false;
}

//----------------------------------------------------------------------------//
void OpenGLGeometryBuffer::setTranslation(const Vector3& v)
{
    d_translation = v;
    d_matrixValid = false;
}

//----------------------------------------------------------------------------//
void OpenGLGeometryBuffer::setRotation(const Vector3& r)
{
    d_rotation = r;
    d_matrixValid = false;
}

//----------------------------------------------------------------------------//
void OpenGLGeometryBuffer::setPivot(const Vector3& p)
{
    d_pivot = Vector3(p.d_x, p.d_y, p.d_z);
    d_matrixValid = false;
}

//----------------------------------------------------------------------------//
void OpenGLGeometryBuffer::setClippingRegion(const Rect& region)
{
    d_clipRect = region;
}

//----------------------------------------------------------------------------//
void OpenGLGeometryBuffer::appendGeometry(const Vertex* const vbuff,
    uint vertex_count)
{
    performBatchManagement();
    d_batches.back().appendGeometry(vbuff, vertex_count);
}

//----------------------------------------------------------------------------//
void OpenGLGeometryBuffer::setActiveTexture(Texture* texture)
{
    d_activeTexture = static_cast<OpenGLTexture*>(texture);
}

//----------------------------------------------------------------------------//
void OpenGLGeometryBuffer::reset()
{
    d_batches.clear();
    d_activeTexture = 0;
}

//----------------------------------------------------------------------------//
Texture* OpenGLGeometryBuffer::getActiveTexture() const
{
    return d_activeTexture;
}

//----------------------------------------------------------------------------//
uint OpenGLGeometryBuffer::getVertexCount() const
{
    uint count = 0;
    BatchList::const_iterator i = d_batches.begin();

    for ( ; i != d_batches.end(); ++i)
        count += (*i).getVertexCount();

    return count;
}

//----------------------------------------------------------------------------//
uint OpenGLGeometryBuffer::getBatchCount() const
{
    return d_batches.size();
}

//----------------------------------------------------------------------------//
void OpenGLGeometryBuffer::performBatchManagement()
{
    const GLuint gltex = d_activeTexture ?
                            d_activeTexture->getOpenGLTexture() : 0;

    // create a new batch if there are no batches yet, or if the active texture
    // differs from that used by the current batch.
    if (d_batches.empty() || (gltex != d_batches.back().getTextureName()))
        d_batches.push_back(OpenGLGeometryBatch(gltex));
}

//----------------------------------------------------------------------------//
void OpenGLGeometryBuffer::setRenderEffect(RenderEffect* effect)
{
    d_effect = effect;
}

//----------------------------------------------------------------------------//
RenderEffect* OpenGLGeometryBuffer::getRenderEffect()
{
    return d_effect;
}

//----------------------------------------------------------------------------//
const double* OpenGLGeometryBuffer::getMatrix() const
{
    if (!d_matrixValid)
        updateMatrix();

    return d_matrix;
}

//----------------------------------------------------------------------------//
void OpenGLGeometryBuffer::updateMatrix() const
{
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();

    // apply the transformations we need to use.
    if (d_hasCustomMatrix)
    {
        glLoadMatrixf(d_xform);
    }
    else
    {
        const Vector3 final_trans(d_translation.d_x + d_pivot.d_x,
                                d_translation.d_y + d_pivot.d_y,
                                d_translation.d_z + d_pivot.d_z);

        glLoadIdentity();
        glTranslatef(final_trans.d_x, final_trans.d_y, final_trans.d_z);
        glRotatef(d_rotation.d_z, 0.0f, 0.0f, 1.0f);
        glRotatef(d_rotation.d_y, 0.0f, 1.0f, 0.0f);
        glRotatef(d_rotation.d_x, 1.0f, 0.0f, 0.0f);
        glTranslatef(-d_pivot.d_x, -d_pivot.d_y, -d_pivot.d_z);
    }

    glGetDoublev(GL_MODELVIEW_MATRIX, d_matrix);
    glPopMatrix();

    d_matrixValid = true;
}

//----------------------------------------------------------------------------//

} // End of  CEGUI namespace section
