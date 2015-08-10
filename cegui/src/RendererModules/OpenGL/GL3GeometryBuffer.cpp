/***********************************************************************
    created:    Wed, 8th Feb 2012
    author:     Lukas E Meindl (based on code by Paul D Turner)
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2012 Paul D Turner & The CEGUI Development Team
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
#include "CEGUI/RendererModules/OpenGL/GL3GeometryBuffer.h"
#include "CEGUI/RendererModules/OpenGL/GL3Renderer.h"
#include "CEGUI/RenderEffect.h"
#include "CEGUI/RendererModules/OpenGL/Texture.h"
#include "CEGUI/Vertex.h"
#include "CEGUI/RendererModules/OpenGL/ShaderManager.h"
#include "CEGUI/RendererModules/OpenGL/Shader.h"
#include "CEGUI/RendererModules/OpenGL/StateChangeWrapper.h"
#include "CEGUI/RendererModules/OpenGL/GlmPimpl.h"

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

// Start of CEGUI namespace section
namespace CEGUI
{
//----------------------------------------------------------------------------//
OpenGL3GeometryBuffer::OpenGL3GeometryBuffer(OpenGL3Renderer& owner) :
    OpenGLGeometryBufferBase(owner),
    d_shader(owner.getShaderStandard()),
    d_shaderPosLoc(owner.getShaderStandardPositionLoc()),
    d_shaderTexCoordLoc(owner.getShaderStandardTexCoordLoc()),
    d_shaderColourLoc(owner.getShaderStandardColourLoc()),
    d_shaderStandardMatrixLoc(owner.getShaderStandardMatrixUniformLoc()),
    d_glStateChanger(owner.getOpenGLStateChanger()),
    d_bufferSize(0)
{
    initialiseOpenGLBuffers();
}

//----------------------------------------------------------------------------//
OpenGL3GeometryBuffer::~OpenGL3GeometryBuffer()
{
    deinitialiseOpenGLBuffers();
}

//----------------------------------------------------------------------------//
void OpenGL3GeometryBuffer::draw() const
{
    CEGUI::Rectf viewPort = d_owner->getActiveViewPort();

    d_glStateChanger->scissor(static_cast<GLint>(d_clipRect.left()),
              static_cast<GLint>(viewPort.getHeight() - d_clipRect.bottom()),
              static_cast<GLint>(d_clipRect.getWidth()),
              static_cast<GLint>(d_clipRect.getHeight()));

    // apply the transformations we need to use.
    if (!d_matrixValid)
        updateMatrix();

    // Send ModelViewProjection matrix to shader
    glm::mat4 modelViewProjectionMatrix = d_owner->getViewProjectionMatrix()->d_matrix * d_matrix->d_matrix;
    glUniformMatrix4fv(d_shaderStandardMatrixLoc, 1, GL_FALSE, glm::value_ptr(modelViewProjectionMatrix));

    // activate desired blending mode
    d_owner->setupRenderingBlendMode(d_blendMode);

    if (OpenGLInfo::getSingleton().isVaoSupported())
    {
        // Bind our vao
        d_glStateChanger->bindVertexArray(d_verticesVAO);
    }
    else
    {
        // We need to emulate a VAO.
        // This binds and sets up a vbo for rendering
        configureVertexArray();
    }

    const int pass_count = d_effect ? d_effect->getPassCount() : 1;
     size_t pos = 0;
    for (int pass = 0; pass < pass_count; ++pass)
    {
        // set up RenderEffect
        if (d_effect)
            d_effect->performPreRenderFunctions(pass);

        // draw the batches
       
        BatchList::const_iterator i = d_batches.begin();
        for ( ; i != d_batches.end(); ++i)
        {
            const BatchInfo& currentBatch = *i;

            if (currentBatch.clip)
                glEnable(GL_SCISSOR_TEST);
            else
                glDisable(GL_SCISSOR_TEST);

            glBindTexture(GL_TEXTURE_2D, currentBatch.texture);

            // draw the geometry
            const unsigned int numVertices = currentBatch.vertexCount;
            glDrawArrays(GL_TRIANGLES, pos, numVertices);

            pos += numVertices;
        }
    }

    // clean up RenderEffect
    if (d_effect)
        d_effect->performPostRenderFunctions();
}

//----------------------------------------------------------------------------//
void OpenGL3GeometryBuffer::appendGeometry(const Vertex* const vbuff,
    uint vertex_count)
{
    OpenGLGeometryBufferBase::appendGeometry(vbuff, vertex_count);
    updateOpenGLBuffers();
}

//----------------------------------------------------------------------------//
void OpenGL3GeometryBuffer::reset()
{
    OpenGLGeometryBufferBase::reset();
    updateOpenGLBuffers();
}

//----------------------------------------------------------------------------//
void OpenGL3GeometryBuffer::initialiseOpenGLBuffers()
{
    if (OpenGLInfo::getSingleton().isVaoSupported())
    {
        glGenVertexArrays(1, &d_verticesVAO);
        glBindVertexArray(d_verticesVAO);
    }

    // Generate position vbo
    glGenBuffers(1, &d_verticesVBO);

    // This binds and sets up a vbo. The 
    configureVertexArray();
    glBufferData(GL_ARRAY_BUFFER, 0, 0, GL_DYNAMIC_DRAW);

    if (OpenGLInfo::getSingleton().isVaoSupported())
    {
        // Unbind Vertex Attribute Array (VAO)
        glBindVertexArray(0);
    }

    // Unbind array and element array buffers
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

//----------------------------------------------------------------------------//
void OpenGL3GeometryBuffer::configureVertexArray() const
{
    glBindBuffer(GL_ARRAY_BUFFER, d_verticesVBO);
    GLsizei stride = 9 * sizeof(GLfloat);

    glVertexAttribPointer(d_shaderTexCoordLoc, 2, GL_FLOAT, GL_FALSE, stride, 0);
    glEnableVertexAttribArray(d_shaderTexCoordLoc);

    glVertexAttribPointer(d_shaderColourLoc, 4, GL_FLOAT, GL_FALSE, stride, BUFFER_OFFSET(2 * sizeof(GLfloat)));
    glEnableVertexAttribArray(d_shaderColourLoc);

    glVertexAttribPointer(d_shaderPosLoc, 3, GL_FLOAT, GL_FALSE, stride, BUFFER_OFFSET(6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(d_shaderPosLoc);
}

//----------------------------------------------------------------------------//
void OpenGL3GeometryBuffer::deinitialiseOpenGLBuffers()
{
    if (OpenGLInfo::getSingleton().isVaoSupported())
        glDeleteVertexArrays(1, &d_verticesVAO);
    glDeleteBuffers(1, &d_verticesVBO);
}

//----------------------------------------------------------------------------//
void OpenGL3GeometryBuffer::updateOpenGLBuffers()
{
    bool needNewBuffer = false;
    unsigned int vertexCount = d_vertices.size();

    if(d_bufferSize < vertexCount)
    {
        needNewBuffer = true;
        d_bufferSize = vertexCount;
    }

    d_glStateChanger->bindBuffer(GL_ARRAY_BUFFER, d_verticesVBO);

    GLsizei dataSize = vertexCount * sizeof(GLVertex);

    GLVertex* data;
    if(d_vertices.empty())
        data = 0;
    else
        data = &d_vertices[0];

    if(needNewBuffer)
    {
        glBufferData(GL_ARRAY_BUFFER, dataSize, data, GL_DYNAMIC_DRAW);
    }
    else
    {
        glBufferSubData(GL_ARRAY_BUFFER, 0, dataSize, data);
    }
}

//----------------------------------------------------------------------------//

} // End of  CEGUI namespace section

