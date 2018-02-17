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
#include "CEGUI/Vertex.h"

#include <glm/gtc/type_ptr.hpp>

// Start of CEGUI namespace section
namespace CEGUI
{
//----------------------------------------------------------------------------//
OpenGLGeometryBuffer::OpenGLGeometryBuffer(OpenGLRenderer& owner, CEGUI::RefCounted<RenderMaterial> renderMaterial) :
    OpenGLGeometryBufferBase(owner, renderMaterial)
{
}

//----------------------------------------------------------------------------//
void OpenGLGeometryBuffer::draw() const
{
    if(d_vertexData.empty())
        return;

    CEGUI::Rectf viewPort = d_owner.getActiveViewPort();

    if (d_clippingActive)
    {
        glScissor(static_cast<GLint>(d_preparedClippingRegion.left()),
            static_cast<GLint>(viewPort.getHeight() - d_preparedClippingRegion.bottom()),
            static_cast<GLint>(d_preparedClippingRegion.getWidth()),
            static_cast<GLint>(d_preparedClippingRegion.getHeight()));

        glEnable(GL_SCISSOR_TEST);
    }
    else
        glDisable(GL_SCISSOR_TEST);

    // Update the model view projection matrix
    updateMatrix();

    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(glm::value_ptr(d_matrix));

    // activate desired blending mode
    d_owner.setupRenderingBlendMode(d_blendMode);

    const int pass_count = d_effect ? d_effect->getPassCount() : 1;
    for (int pass = 0; pass < pass_count; ++pass)
    {
        // set up RenderEffect
        if (d_effect)
            d_effect->performPreRenderFunctions(pass);

        d_renderMaterial->prepareForRendering();

        setupVertexDataPointers();

        // draw the geometry
        glDrawArrays(GL_TRIANGLES, 0, d_vertexCount);
    }

    // clean up RenderEffect
    if (d_effect)
        d_effect->performPostRenderFunctions();

    updateRenderTargetData(d_owner.getActiveRenderTarget());
}

//----------------------------------------------------------------------------//
void OpenGLGeometryBuffer::setupVertexDataPointers() const
{
    int dataOffset = 0;
    GLsizei stride = getVertexAttributeElementCount() * sizeof(GL_FLOAT);

    const size_t attribute_count = d_vertexAttributes.size();
    for (size_t i = 0; i < attribute_count; ++i)
    {
        switch(d_vertexAttributes.at(i))
        {
        case VertexAttributeType::Position0:
            {
                glVertexPointer(3, GL_FLOAT, stride,
                    &d_vertexData[dataOffset]);
                dataOffset += 3;
            }
            break;
        case VertexAttributeType::Colour0:
            {
                glColorPointer(4, GL_FLOAT, stride,
                    &d_vertexData[dataOffset]);
                dataOffset += 4;
            }
            break;
        case VertexAttributeType::TexCoord0:
            {
                glTexCoordPointer(2, GL_FLOAT, stride,
                    &d_vertexData[dataOffset]);
                dataOffset += 2;
            }
            break;
        default:
            break;
        }
    }
}

//----------------------------------------------------------------------------//

} // End of  CEGUI namespace section

