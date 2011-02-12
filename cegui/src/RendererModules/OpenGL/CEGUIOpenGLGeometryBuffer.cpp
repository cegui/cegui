/***********************************************************************
    filename:   CEGUIOpenGLGeometryBuffer.cpp
    created:    Thu Jan 8 2009
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
#include <GL/glew.h>
#include "CEGUIOpenGLGeometryBuffer.h"
#include "CEGUIRenderEffect.h"
#include "CEGUIOpenGLTexture.h"
#include "CEGUIVertex.h"

// Start of CEGUI namespace section
namespace CEGUI
{
//----------------------------------------------------------------------------//
OpenGLGeometryBuffer::OpenGLGeometryBuffer(OpenGLRenderer& owner) :
    d_owner(&owner),
    d_activeTexture(0),
    d_translation(0, 0, 0),
    d_rotation(Quaternion::IDENTITY),
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

    // activate desired blending mode
    d_owner->setupRenderingBlendMode(d_blendMode);

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
            glBindTexture(GL_TEXTURE_2D, (*i).first);
            // set up pointers to the vertex element arrays
            glTexCoordPointer(2, GL_FLOAT, sizeof(GLVertex),
                              &d_vertices[pos]);
            glColorPointer(4, GL_FLOAT, sizeof(GLVertex),
                           &d_vertices[pos].colour[0]);
            glVertexPointer(3, GL_FLOAT, sizeof(GLVertex),
                            &d_vertices[pos].position[0]);
            // draw the geometry
            glDrawArrays(GL_TRIANGLES, 0, (*i).second);
            pos += (*i).second;
        }
    }

    // clean up RenderEffect
    if (d_effect)
        d_effect->performPostRenderFunctions();
}

//----------------------------------------------------------------------------//
void OpenGLGeometryBuffer::setTranslation(const Vector3& v)
{
    d_translation = v;
    d_matrixValid = false;
}

//----------------------------------------------------------------------------//
void OpenGLGeometryBuffer::setRotation(const Quaternion& r)
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
    d_clipRect.d_top    = PixelAligned(region.d_top);
    d_clipRect.d_bottom = PixelAligned(region.d_bottom);
    d_clipRect.d_left   = PixelAligned(region.d_left);
    d_clipRect.d_right  = PixelAligned(region.d_right);
}

//----------------------------------------------------------------------------//
void OpenGLGeometryBuffer::appendGeometry(const Vertex* const vbuff,
    uint vertex_count)
{
    performBatchManagement();

    // update size of current batch
    d_batches.back().second += vertex_count;

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
void OpenGLGeometryBuffer::setActiveTexture(Texture* texture)
{
    d_activeTexture = static_cast<OpenGLTexture*>(texture);
}

//----------------------------------------------------------------------------//
void OpenGLGeometryBuffer::reset()
{
    d_batches.clear();
    d_vertices.clear();
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
    return d_vertices.size();
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
    if (d_batches.empty() || (gltex != d_batches.back().first))
        d_batches.push_back(BatchInfo(gltex, 0));
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

    const Vector3 final_trans(d_translation.d_x + d_pivot.d_x,
                              d_translation.d_y + d_pivot.d_y,
                              d_translation.d_z + d_pivot.d_z);

    glLoadIdentity();
    glTranslatef(final_trans.d_x, final_trans.d_y, final_trans.d_z);

    float rotation_matrix[16];
    rotation_matrix[ 0] = 1.0f - 2.0f * (d_rotation.d_y * d_rotation.d_y + d_rotation.d_z * d_rotation.d_z);
    rotation_matrix[ 1] = 2.0f * (d_rotation.d_x * d_rotation.d_y + d_rotation.d_z * d_rotation.d_w);
    rotation_matrix[ 2] = 2.0f * (d_rotation.d_x * d_rotation.d_z - d_rotation.d_y * d_rotation.d_w);
    rotation_matrix[ 3] = 0.0f;

    rotation_matrix[ 4] = 2.0f * (d_rotation.d_x * d_rotation.d_y - d_rotation.d_z * d_rotation.d_w);
    rotation_matrix[ 5] = 1.0f - 2.0f * (d_rotation.d_x * d_rotation.d_x + d_rotation.d_z * d_rotation.d_z);
    rotation_matrix[ 6] = 2.0f * (d_rotation.d_z * d_rotation.d_y + d_rotation.d_x * d_rotation.d_w);
    rotation_matrix[ 7] = 0.0f;

    rotation_matrix[ 8] = 2.0f * (d_rotation.d_x * d_rotation.d_z + d_rotation.d_y * d_rotation.d_w);
    rotation_matrix[ 9] = 2.0f * (d_rotation.d_y * d_rotation.d_z - d_rotation.d_x * d_rotation.d_w);
    rotation_matrix[10] = 1.0f - 2.0f * (d_rotation.d_x * d_rotation.d_x + d_rotation.d_y * d_rotation.d_y);
    rotation_matrix[11] = 0.0f;

    rotation_matrix[12] = 0.0f;
    rotation_matrix[13] = 0.0f;
    rotation_matrix[14] = 0.0f;
    rotation_matrix[15] = 1.0f;

    glMultMatrixf(rotation_matrix);

    glTranslatef(-d_pivot.d_x, -d_pivot.d_y, -d_pivot.d_z);

    glGetDoublev(GL_MODELVIEW_MATRIX, d_matrix);
    glPopMatrix();

    d_matrixValid = true;
}

//----------------------------------------------------------------------------//

} // End of  CEGUI namespace section

