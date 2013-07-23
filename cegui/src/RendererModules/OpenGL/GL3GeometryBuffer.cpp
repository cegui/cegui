/***********************************************************************
    filename:   CEGUIOpenGLGeometryBuffer.cpp
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
#include <GL/glew.h>

#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "CEGUI/RendererModules/OpenGL/GL3GeometryBuffer.h"
#include "CEGUI/RendererModules/OpenGL/GL3Renderer.h"
#include "CEGUI/RenderEffect.h"
#include "CEGUI/RendererModules/OpenGL/Texture.h"
#include "CEGUI/Vertex.h"
#include "CEGUI/ShaderParameterBindings.h"
#include "CEGUI/RendererModules/OpenGL/ShaderManager.h"
#include "CEGUI/RendererModules/OpenGL/Shader.h"
#include "CEGUI/RendererModules/OpenGL/StateChangeWrapper.h"
#include "CEGUI/RendererModules/OpenGL/GlmPimpl.h"
#include "CEGUI/RendererModules/OpenGL/GL3ShaderWrapper.h"

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

// Start of CEGUI namespace section
namespace CEGUI
{
//----------------------------------------------------------------------------//
OpenGL3GeometryBuffer::OpenGL3GeometryBuffer(OpenGL3Renderer& owner, CEGUI::RefCounted<RenderMaterial> renderMaterial) :
    OpenGLGeometryBufferBase(owner, renderMaterial),
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

    CEGUI::ShaderParameterBindings* shaderParameterBindings = (*d_renderMaterial).getShaderParamBindings();
    // Set the ModelViewProjection matrix in the bindings
    glm::mat4 modelViewProjectionMatrix = d_owner->getViewProjectionMatrix()->d_matrix * d_matrix->d_matrix;
    shaderParameterBindings->setParameter("modelViewPerspMatrix", modelViewProjectionMatrix);

    // activate desired blending mode
    d_owner->setupRenderingBlendMode(d_blendMode);

    // Bind our vao
    d_glStateChanger->bindVertexArray(d_verticesVAO);

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

            if(currentBatch.texture != 0)
                shaderParameterBindings->setParameter("texture0", currentBatch.texture);

            d_renderMaterial->prepareForRendering();

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
void OpenGL3GeometryBuffer::reset()
{
    OpenGLGeometryBufferBase::reset();
    updateOpenGLBuffers();
}

//----------------------------------------------------------------------------//
void OpenGL3GeometryBuffer::initialiseOpenGLBuffers()
{
    glGenVertexArrays(1, &d_verticesVAO);
    d_glStateChanger->bindVertexArray(d_verticesVAO);

    // Generate and bind position vbo
    glGenBuffers(1, &d_verticesVBO);
    d_glStateChanger->bindBuffer(GL_ARRAY_BUFFER, d_verticesVBO);

    glBufferData(GL_ARRAY_BUFFER, 0, 0, GL_DYNAMIC_DRAW);

    // Unbind Vertex Attribute Array (VAO)
    d_glStateChanger->bindVertexArray(0);

    // Unbind array and element array buffers
    d_glStateChanger->bindBuffer(GL_ARRAY_BUFFER, 0);
}


//----------------------------------------------------------------------------//
void OpenGL3GeometryBuffer::finaliseVertexAttributes()
{  
    d_glStateChanger->bindVertexArray(d_verticesVAO);
    d_glStateChanger->bindBuffer(GL_ARRAY_BUFFER, d_verticesVBO);

    GLsizei stride = getVertexAttributeElementCount() * sizeof(GL_FLOAT);

    CEGUI::OpenGL3ShaderWrapper* gl3_shader_wrapper = static_cast<CEGUI::OpenGL3ShaderWrapper*>(d_renderMaterial->getShaderWrapper());

    //Update the vertex attrib pointers of the vertex array object depending on the saved attributes
    int dataOffset = 0;
    const unsigned int attribute_count = d_vertexAttributes.size();
    for (unsigned int i = 0; i < attribute_count; ++i)
    {
        switch(d_vertexAttributes.at(i))
        {
        case VAT_POSITION0:
            {
                GLint shader_pos_loc = gl3_shader_wrapper->getAttributeLocation("inPosition");
                glVertexAttribPointer(shader_pos_loc, 3, GL_FLOAT, GL_FALSE, stride, BUFFER_OFFSET(dataOffset * sizeof(GL_FLOAT)));
                glEnableVertexAttribArray(shader_pos_loc);
                dataOffset += 3;
            }
            break;
        case VAT_COLOUR0:
            {
                GLint shader_colour_loc = gl3_shader_wrapper->getAttributeLocation("inColour");
                glVertexAttribPointer(shader_colour_loc, 4, GL_FLOAT, GL_FALSE, stride, BUFFER_OFFSET(dataOffset * sizeof(GL_FLOAT)));
                glEnableVertexAttribArray(shader_colour_loc);
                dataOffset += 4;
            }
            break;
        case VAT_TEXCOORD0:
            {
                GLint texture_coord_loc = gl3_shader_wrapper->getAttributeLocation("inTexCoord");
                glVertexAttribPointer(texture_coord_loc, 2, GL_FLOAT, GL_FALSE, stride, BUFFER_OFFSET(dataOffset * sizeof(GL_FLOAT)));
                glEnableVertexAttribArray(texture_coord_loc);
                dataOffset += 2;
            }
            break;
        default:
            break;
        }
    }
}


//----------------------------------------------------------------------------//
void OpenGL3GeometryBuffer::deinitialiseOpenGLBuffers()
{
    glDeleteVertexArrays(1, &d_verticesVAO);
    glDeleteBuffers(1, &d_verticesVBO);
}

//----------------------------------------------------------------------------//
void OpenGL3GeometryBuffer::updateOpenGLBuffers()
{
    bool needNewBuffer = false;
    unsigned int vertexCount = d_vertexData.size();

    if(d_bufferSize < vertexCount)
    {
        needNewBuffer = true;
        d_bufferSize = vertexCount;
    }

    d_glStateChanger->bindBuffer(GL_ARRAY_BUFFER, d_verticesVBO);

    float* vertexData;
    if(d_vertexData.empty())
        vertexData = 0;
    else
        vertexData = &d_vertexData[0];

    GLsizei dataSize = d_bufferSize * sizeof(float);

    if(needNewBuffer)
    {
        glBufferData(GL_ARRAY_BUFFER, dataSize, vertexData, GL_DYNAMIC_DRAW);
    }
    else
    {
        glBufferSubData(GL_ARRAY_BUFFER, 0, dataSize, vertexData);
    }
}

//----------------------------------------------------------------------------//
void OpenGL3GeometryBuffer::appendGeometry(const std::vector<float>& vertex_data)
{
    OpenGLGeometryBufferBase::appendGeometry(vertex_data);
    updateOpenGLBuffers();
}


//----------------------------------------------------------------------------//

} // End of  CEGUI namespace section

