/***********************************************************************
    created:    Thu Jan 8 2009
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
#include "CEGUI/RendererModules/OpenGL/GLGeometryBuffer.h"
#include "CEGUI/RendererModules/OpenGL/GLRenderer.h"
#include "CEGUI/RenderEffect.h"
#include "CEGUI/RendererModules/OpenGL/Texture.h"
#include "CEGUI/Vertex.h"
#include "CEGUI/RendererModules/OpenGL/GlmPimpl.h"
#include "glm/gtc/type_ptr.hpp"

// Start of CEGUI namespace section
namespace CEGUI
{
//----------------------------------------------------------------------------//
OpenGLGeometryBuffer::OpenGLGeometryBuffer(OpenGLRenderer& owner) :
    OpenGLGeometryBufferBase(owner)
{
}

//----------------------------------------------------------------------------//
void OpenGLGeometryBuffer::draw() const
{
    CEGUI::Rectf viewPort = d_owner->getActiveViewPort();

    // setup clip region
    glScissor(static_cast<GLint>(d_clipRect.left()),
              static_cast<GLint>(viewPort.getHeight() - d_clipRect.bottom()),
              static_cast<GLint>(d_clipRect.getWidth()),
              static_cast<GLint>(d_clipRect.getHeight()));

    // apply the transformations we need to use.
    if (!d_matrixValid)
        updateMatrix();

    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(glm::value_ptr(d_matrix->d_matrix));

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
            if (i->clip)
                glEnable(GL_SCISSOR_TEST);
            else
                glDisable(GL_SCISSOR_TEST);

            glBindTexture(GL_TEXTURE_2D, i->texture);
            // set up pointers to the vertex element arrays
            glTexCoordPointer(2, GL_FLOAT, sizeof(GLVertex),
                              &d_vertices[pos]);
            glColorPointer(4, GL_FLOAT, sizeof(GLVertex),
                           &d_vertices[pos].colour[0]);
            glVertexPointer(3, GL_FLOAT, sizeof(GLVertex),
                            &d_vertices[pos].position[0]);
            // draw the geometry
            glDrawArrays(GL_TRIANGLES, 0, i->vertexCount);
            pos += i->vertexCount;
        }
    }

    // clean up RenderEffect
    if (d_effect)
        d_effect->performPostRenderFunctions();
}

//----------------------------------------------------------------------------//

} // End of  CEGUI namespace section

