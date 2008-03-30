/***********************************************************************
    filename:   CEGUIOpenGLRenderTarget.cpp
    created:    Tue Feb 12 2008
    author:     Paul D Turner

    This file contains parts of the original CEGUI OpenGL renderer
    code that was written by Mark Strom <mwstrom@gmail.com>
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2008 Paul D Turner & The CEGUI Development Team
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
#include "CEGUIOpenGLRenderTarget.h"
#include "opengltexture.h"
#include "CEGUIColourRect.h"

// Start of CEGUI namespace section
namespace CEGUI
{
//----------------------------------------------------------------------------//
OpenGLRenderTarget::OpenGLRenderTarget() :
    d_area(0, 0, 0, 0),
    d_boundTexture(0),
    d_bufferIdx(0),
    d_queueEnabled(true),
    d_directRendering(false)
{
}

//----------------------------------------------------------------------------//
OpenGLRenderTarget::~OpenGLRenderTarget()
{
}

//----------------------------------------------------------------------------//
void OpenGLRenderTarget::queueQuad(const Rect& dest, float z,
    const Texture* tex, const Rect& tex_rect, const ColourRect& cols,
    const QuadSplitMode quad_split_mode)
{
    if (!d_queueEnabled)
    {
        renderQuad(dest, z, tex, tex_rect, cols, quad_split_mode);
        return;
    }

    Quad quad;
    // set up the quad struct with the input data
    _initialise_quad(quad, dest, z,
                     static_cast<const OpenGLTexture*>(tex)->getOGLTexid(),
                     tex_rect, quad_split_mode, cols);

    // add quad to list (insertion sorted)
    _queue_quad(quad);
}

//----------------------------------------------------------------------------//
void OpenGLRenderTarget::renderQuad(const Rect& dest, float z,
    const Texture* tex, const Rect& tex_rect, const ColourRect& cols,
    const QuadSplitMode quad_split_mode)
{
    Quad quad;
    // set up the quad struct with the input data
    _initialise_quad(quad, dest, z,
                     static_cast<const OpenGLTexture*>(tex)->getOGLTexid(),
                     tex_rect, quad_split_mode, cols);

    _render_quad(quad);
}

//----------------------------------------------------------------------------//
void OpenGLRenderTarget::clearRenderList(void)
{
    d_quadlist.clear();
}

//----------------------------------------------------------------------------//
void OpenGLRenderTarget::execute(void)
{
    beginRenderState();

    renderQuadList(d_quadlist.begin(), d_quadlist.end());

    endRenderState();
}

//----------------------------------------------------------------------------//
void OpenGLRenderTarget::beginDirectRendering()
{
    if (!d_directRendering)
    {
        d_directRendering = true;
        beginRenderState();
    }
}

//----------------------------------------------------------------------------//
void OpenGLRenderTarget::endDirectRendering()
{
    if (d_directRendering)
    {
        // render quads added to the 'immediate' queue.
        renderQuadList(d_immediateQueue.begin(), d_immediateQueue.end());
        d_immediateQueue.clear();

        d_directRendering = false;
        endRenderState();
    }
}

//----------------------------------------------------------------------------//
void OpenGLRenderTarget::enableQueue(bool enable)
{
    d_queueEnabled = enable;
    d_queueEnabled ? endDirectRendering() : beginDirectRendering();
}

//----------------------------------------------------------------------------//
const Rect& OpenGLRenderTarget::getArea() const
{
    return d_area;
}

//----------------------------------------------------------------------------//
void OpenGLRenderTarget::renderVertexBuffer()
{
    // exit if there is no data in the buffer
    if (!d_bufferIdx)
        return;

    // render (d_bufferIdx == number of vertices in the buffer)
    glDrawArrays(GL_TRIANGLES, 0, d_bufferIdx);

    // reset buffer
    d_bufferIdx = 0;
}

//----------------------------------------------------------------------------//
void OpenGLRenderTarget::beginRenderState()
{
    // save current attributes
    glPushClientAttrib(GL_CLIENT_ALL_ATTRIB_BITS);
    glPushAttrib(GL_ALL_ATTRIB_BITS);

    glViewport(
        static_cast<GLsizei>(d_area.d_left),
        static_cast<GLsizei>(d_area.d_top),
        static_cast<GLsizei>(d_area.d_right),
        static_cast<GLsizei>(d_area.d_bottom));

    glPolygonMode(GL_FRONT, GL_FILL);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(d_area.d_left, d_area.d_right, d_area.d_top, d_area.d_bottom);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_FOG);
    glDisable(GL_TEXTURE_GEN_S);
    glDisable(GL_TEXTURE_GEN_T);
    glDisable(GL_TEXTURE_GEN_R);

    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glEnable(GL_TEXTURE_2D);

    d_boundTexture = 0;
}

//----------------------------------------------------------------------------//
void OpenGLRenderTarget::endRenderState()
{
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);

    // restore former attributes
    glPopAttrib();
    glPopClientAttrib();
}

//----------------------------------------------------------------------------//
void OpenGLRenderTarget::stuffQuad(const Quad& quad, Vertex* vbuffer)
{
    // vertex 0
    vbuffer[0].vertex[0] = quad.position.d_left;
    vbuffer[0].vertex[1] = quad.position.d_top;
    vbuffer[0].vertex[2] = quad.z;
    vbuffer[0].color     = quad.topLeftCol;
    vbuffer[0].tex[0]    = quad.texPosition.d_left;
    vbuffer[0].tex[1]    = quad.texPosition.d_top;

    // vertex 1
    vbuffer[1].vertex[0] = quad.position.d_left;
    vbuffer[1].vertex[1] = quad.position.d_bottom;
    vbuffer[1].vertex[2] = quad.z;
    vbuffer[1].color     = quad.bottomLeftCol;
    vbuffer[1].tex[0]    = quad.texPosition.d_left;
    vbuffer[1].tex[1]    = quad.texPosition.d_bottom;

    // vertex 2
    vbuffer[2].vertex[0]  = quad.position.d_right;
    vbuffer[2].vertex[2]  = quad.z;
    vbuffer[2].color      = quad.bottomRightCol;
    vbuffer[2].tex[0]     = quad.texPosition.d_right;

    // top-left to bottom-right diagonal
    if (quad.quadSplitMode == TopLeftToBottomRight)
    {
        vbuffer[2].vertex[1]  = quad.position.d_bottom;
        vbuffer[2].tex[1]     = quad.texPosition.d_bottom;
    }
    // bottom-left to top-right diagonal
    else
    {
        vbuffer[2].vertex[1]  = quad.position.d_top;
        vbuffer[2].tex[1]     = quad.texPosition.d_top;
    }

    // vertex 3
    vbuffer[3].vertex[0] = quad.position.d_right;
    vbuffer[3].vertex[1] = quad.position.d_top;
    vbuffer[3].vertex[2] = quad.z;
    vbuffer[3].color     = quad.topRightCol;
    vbuffer[3].tex[0]    = quad.texPosition.d_right;
    vbuffer[3].tex[1]    = quad.texPosition.d_top;

    // vertex 4
    vbuffer[4].vertex[0] = quad.position.d_left;
    vbuffer[4].vertex[2] = quad.z;
    vbuffer[4].color     = quad.topLeftCol;
    vbuffer[4].tex[0]    = quad.texPosition.d_left;

    // top-left to bottom-right diagonal
    if (quad.quadSplitMode == TopLeftToBottomRight)
    {
        vbuffer[4].vertex[1] = quad.position.d_top;
        vbuffer[4].tex[1]    = quad.texPosition.d_top;
    }
    // bottom-left to top-right diagonal
    else
    {
        vbuffer[4].vertex[1] = quad.position.d_bottom;
        vbuffer[4].tex[1]    = quad.texPosition.d_bottom;
    }

    // vertex 5
    vbuffer[5].vertex[0] = quad.position.d_right;
    vbuffer[5].vertex[1] = quad.position.d_bottom;
    vbuffer[5].vertex[2] = quad.z;
    vbuffer[5].color     = quad.bottomRightCol;
    vbuffer[5].tex[0]    = quad.texPosition.d_right;
    vbuffer[5].tex[1]    = quad.texPosition.d_bottom;
}

//----------------------------------------------------------------------------//
void OpenGLRenderTarget::_initialise_quad(Quad& quad, const Rect& dest, float z,
    const GLuint tex, const Rect& tex_rect, const QuadSplitMode split,
    const ColourRect& cols)
{
    // store position. Note that we need to flip over the Y axis.
    quad.position.d_left    = dest.d_left;
    quad.position.d_right   = dest.d_right;
    quad.position.d_bottom  = d_area.d_bottom - dest.d_bottom;
    quad.position.d_top     = d_area.d_bottom - dest.d_top;
    quad.z                  = z;
    quad.quadSplitMode      = split;
    // store texture details
    quad.texid       = tex;
    quad.texPosition = tex_rect;
    // convert and store colours applied to the quad.
    quad.topLeftCol     = OpenGLRenderer::colourToOGL(cols.d_top_left);
    quad.topRightCol    = OpenGLRenderer::colourToOGL(cols.d_top_right);
    quad.bottomLeftCol  = OpenGLRenderer::colourToOGL(cols.d_bottom_left);
    quad.bottomRightCol = OpenGLRenderer::colourToOGL(cols.d_bottom_right);
}

//----------------------------------------------------------------------------//
void OpenGLRenderTarget::_queue_quad(const Quad& quad)
{
    d_quadlist.insert(quad);
}

//----------------------------------------------------------------------------//
void OpenGLRenderTarget::_render_quad(const Quad& quad)
{
    d_immediateQueue.push_back(quad);
}

//----------------------------------------------------------------------------//

} // End of  CEGUI namespace section
