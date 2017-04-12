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
#include "CEGUI/RendererModules/OpenGL/GL3GeometryBuffer.h"
#include "CEGUI/RendererModules/OpenGL/GL3Renderer.h"
#include "CEGUI/RenderEffect.h"
#include "CEGUI/Vertex.h"
#include "CEGUI/ShaderParameterBindings.h"
#include "CEGUI/RendererModules/OpenGL/ShaderManager.h"
#include "CEGUI/RendererModules/OpenGL/Shader.h"
#include "CEGUI/RendererModules/OpenGL/StateChangeWrapper.h"
#include "CEGUI/RendererModules/OpenGL/GLBaseShaderWrapper.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

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
    initialiseVertexBuffers();
}

//----------------------------------------------------------------------------//
OpenGL3GeometryBuffer::~OpenGL3GeometryBuffer()
{
    deinitialiseOpenGLBuffers();
}

//----------------------------------------------------------------------------//
void OpenGL3GeometryBuffer::draw() const
{
    if(d_vertexData.empty())
        return;

    CEGUI::Rectf viewPort = d_owner.getActiveViewPort();

    if (d_clippingActive)
    {
        d_glStateChanger->scissor(static_cast<GLint>(d_preparedClippingRegion.left()),
            static_cast<GLint>(viewPort.getHeight() - d_preparedClippingRegion.bottom()),
            static_cast<GLint>(d_preparedClippingRegion.getWidth()),
            static_cast<GLint>(d_preparedClippingRegion.getHeight()));

        d_glStateChanger->enable(GL_SCISSOR_TEST);
    }
    else
        d_glStateChanger->disable(GL_SCISSOR_TEST);

    // Update the model view projection matrix
    updateMatrix();

    CEGUI::ShaderParameterBindings* shaderParameterBindings = (*d_renderMaterial).getShaderParamBindings();

    // Set the uniform variables for this GeometryBuffer in the Shader
    shaderParameterBindings->setParameter("modelViewProjMatrix", d_matrix);
    shaderParameterBindings->setParameter("alphaFactor", d_alpha);

    // activate desired blending mode
    d_owner.setupRenderingBlendMode(d_blendMode);

    if (OpenGLInfo::getSingleton().isVaoSupported())
    {
        // Bind our vao
        d_glStateChanger->bindVertexArray(d_verticesVAO);
    }
    else
    {
        // This binds and sets up a vbo for rendering
        finaliseVertexAttributes();
    }

    const int pass_count = d_effect ? d_effect->getPassCount() : 1;
    for (int pass = 0; pass < pass_count; ++pass)
    {
        // set up RenderEffect
        if (d_effect)
            d_effect->performPreRenderFunctions(pass);

        d_renderMaterial->prepareForRendering();

        // draw the geometry
        drawDependingOnFillRule();
    }

    // clean up RenderEffect
    if (d_effect)
        d_effect->performPostRenderFunctions();

    updateRenderTargetData(d_owner.getActiveRenderTarget());
}

//----------------------------------------------------------------------------//
void OpenGL3GeometryBuffer::reset()
{
    OpenGLGeometryBufferBase::reset();
    updateOpenGLBuffers();
}

//----------------------------------------------------------------------------//
void OpenGL3GeometryBuffer::initialiseVertexBuffers()
{
    if (OpenGLInfo::getSingleton().isVaoSupported())
    {
        glGenVertexArrays(1, &d_verticesVAO);
        d_glStateChanger->bindVertexArray(d_verticesVAO);
    }

    // Generate position vbo
    glGenBuffers(1, &d_verticesVBO);
    d_glStateChanger->bindBuffer(GL_ARRAY_BUFFER, d_verticesVBO);

    glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_DYNAMIC_DRAW);

    if (OpenGLInfo::getSingleton().isVaoSupported())
    {
        // Unbind Vertex Attribute Array (VAO)
        d_glStateChanger->bindVertexArray(0);
    }

    // Unbind array and element array buffers
    d_glStateChanger->bindBuffer(GL_ARRAY_BUFFER, 0);    
}


//----------------------------------------------------------------------------//
void OpenGL3GeometryBuffer::finaliseVertexAttributes() const
{
    //On OpenGL desktop versions we want to bind both of the following calls, otherwise vbos are enough as the following calls
    //only affect the vbo (which may be tied to a vao)
    if (OpenGLInfo::getSingleton().isVaoSupported())
    {
        d_glStateChanger->bindVertexArray(d_verticesVAO);
    }

    d_glStateChanger->bindBuffer(GL_ARRAY_BUFFER, d_verticesVBO);

    GLsizei stride = getVertexAttributeElementCount() * sizeof(GL_FLOAT);
    const CEGUI::OpenGLBaseShaderWrapper* gl3_shader_wrapper = static_cast<const CEGUI::OpenGLBaseShaderWrapper*>(d_renderMaterial->getShaderWrapper());
    //Update the vertex attrib pointers of the vertex array object depending on the saved attributes
    int dataOffset = 0;
    const size_t attribute_count = d_vertexAttributes.size();
    for (size_t i = 0; i < attribute_count; ++i)
    {
        switch(d_vertexAttributes.at(i))
        {
        case VertexAttributeType::Position0:
            {
                GLint shader_pos_loc = gl3_shader_wrapper->getAttributeLocation("inPosition");
                glVertexAttribPointer(shader_pos_loc, 3, GL_FLOAT, GL_FALSE, stride, BUFFER_OFFSET(dataOffset * sizeof(GLfloat)));
                glEnableVertexAttribArray(shader_pos_loc);
                dataOffset += 3;
            }
            break;
        case VertexAttributeType::Colour0:
            {
                GLint shader_colour_loc = gl3_shader_wrapper->getAttributeLocation("inColour");
                glVertexAttribPointer(shader_colour_loc, 4, GL_FLOAT, GL_FALSE, stride, BUFFER_OFFSET(dataOffset * sizeof(GLfloat)));
                glEnableVertexAttribArray(shader_colour_loc);
                dataOffset += 4;
            }
            break;
        case VertexAttributeType::TexCoord0:
            {
                GLint texture_coord_loc = gl3_shader_wrapper->getAttributeLocation("inTexCoord");
                glVertexAttribPointer(texture_coord_loc, 2, GL_FLOAT, GL_FALSE, stride, BUFFER_OFFSET(dataOffset * sizeof(GLfloat)));
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
    if (OpenGLInfo::getSingleton().isVaoSupported())
        glDeleteVertexArrays(1, &d_verticesVAO);
    glDeleteBuffers(1, &d_verticesVBO);
}

//----------------------------------------------------------------------------//
void OpenGL3GeometryBuffer::updateOpenGLBuffers()
{
    bool needNewBuffer = false;
    size_t vertexCount = d_vertexData.size();

    if(d_bufferSize < vertexCount)
    {
        needNewBuffer = true;
        d_bufferSize = vertexCount;
    }

    d_glStateChanger->bindBuffer(GL_ARRAY_BUFFER, d_verticesVBO);

    float* vertexData;
    if(d_vertexData.empty())
        vertexData = nullptr;
    else
        vertexData = &d_vertexData[0];

    GLsizei dataSize = vertexCount * sizeof(float);

    if(needNewBuffer)
    {
        glBufferData(GL_ARRAY_BUFFER, dataSize, vertexData, GL_STATIC_DRAW);
    }
    else
    {
        glBufferSubData(GL_ARRAY_BUFFER, 0, dataSize, vertexData);
    }
}

//----------------------------------------------------------------------------//
void OpenGL3GeometryBuffer::appendGeometry(const float* vertex_data, std::size_t array_size)
{
    OpenGLGeometryBufferBase::appendGeometry(vertex_data, array_size);

    updateOpenGLBuffers();
}

//----------------------------------------------------------------------------//
void OpenGL3GeometryBuffer::drawDependingOnFillRule() const
{
    if(d_polygonFillRule == PolygonFillRule::NoFilling)
    {
        d_glStateChanger->disable(GL_CULL_FACE);
        d_glStateChanger->disable(GL_STENCIL_TEST);

        glDrawArrays(GL_TRIANGLES, 0, d_vertexCount);
    }
    else if(d_polygonFillRule == PolygonFillRule::EvenOdd)
    {
        //We use a stencil buffer to determine the insideness
        //of a fragment. Every draw inverts the precious value
        //according to the even-odd rule.
        glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

        d_glStateChanger->disable(GL_CULL_FACE);
        d_glStateChanger->enable(GL_STENCIL_TEST);
        glStencilMask(0xFF);
        glClear(GL_STENCIL_BUFFER_BIT);
        glStencilFunc(GL_ALWAYS, 0x00, 0xFF);
        glStencilOp(GL_INVERT, GL_KEEP, GL_INVERT);
        glDrawArrays(GL_TRIANGLES, 0, d_vertexCount - d_postStencilVertexCount);

        unsigned int postStencilStart = d_vertexCount - d_postStencilVertexCount;
        glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
        glStencilMask(0x00);
        glStencilFunc(GL_EQUAL, 0xFF, 0xFF);
        glDrawArrays(GL_TRIANGLES, postStencilStart, d_postStencilVertexCount);
    }
    else if(d_polygonFillRule == PolygonFillRule::NonZero)
    {
        //We use a stencil buffer to determine the insideness
        //of a fragment. We draw the front sides while increasing
        //stencil values and then draw backside while decreasing them.
        //A resulting 0 value means we are outside.
        glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

        unsigned int solid_fill_count = d_vertexCount - d_postStencilVertexCount;
        unsigned int vertex_pos = 0;

        //Performing the back/front faces stencil incr and decr stencil op
        d_glStateChanger->enable(GL_CULL_FACE);
        d_glStateChanger->enable(GL_STENCIL_TEST);
        glStencilMask(0xFF);
        glClear(GL_STENCIL_BUFFER_BIT);
        glStencilFunc(GL_ALWAYS, 0x00, 0xFF);

        glCullFace(GL_FRONT);
        glStencilOp(GL_KEEP, GL_KEEP, GL_INCR_WRAP);
        glDrawArrays(GL_TRIANGLES, vertex_pos, solid_fill_count);

        glCullFace(GL_BACK);
        glStencilOp(GL_KEEP, GL_KEEP, GL_DECR_WRAP);
        glDrawArrays(GL_TRIANGLES, vertex_pos, solid_fill_count);

        vertex_pos += solid_fill_count;

        //Only needing culling for the back/front face stencil calculations
        d_glStateChanger->disable(GL_CULL_FACE);

        glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
        glStencilMask(0x00);

        if(d_postStencilVertexCount != 0)
        {
            glStencilFunc(GL_NOTEQUAL, 0x00, 0xFF);
            glDrawArrays(GL_TRIANGLES, d_vertexCount - d_postStencilVertexCount, d_postStencilVertexCount);
        }
    }
}


//----------------------------------------------------------------------------//

} // End of  CEGUI namespace section

