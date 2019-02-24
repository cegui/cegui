/***********************************************************************
    created:    Wed, 8th Feb 2012
    author:     Lukas E Meindl (based on code by Paul D Turner)
                David Reepmeyer (added GLES2/GLES3)
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
#ifndef _CEGUIGLES2GeometryBuffer_h_
#define _CEGUIGLES2GeometryBuffer_h_

#include "CEGUI/RendererModules/OpenGL/GeometryBufferBase.h"
#include "CEGUI/RefCounted.h"

namespace CEGUI
{
class OpenGLBaseShader;
class OpenGLBaseStateChangeWrapper;
class GLES2Renderer;
class RenderMaterial;

//! GLES2 based implementation of the GeometryBuffer interface.
class OPENGL_GUIRENDERER_API GLES2GeometryBuffer : public OpenGLGeometryBufferBase
{
public:
    //! Constructor
    GLES2GeometryBuffer(GLES2Renderer& owner, CEGUI::RefCounted<RenderMaterial> renderMaterial);
    virtual ~GLES2GeometryBuffer();

    // Overrides of virtual and abstract methods from GeometryBuffer
    void draw() const override;
    void appendGeometry(const float* vertex_data, std::size_t array_size) override;
    void reset() override;

    // Implementation/overrides of member functions inherited from OpenGLGeometryBufferBase
    void finaliseVertexAttributes();

protected:
    void initialiseVertexBuffers();
    void deinitialiseOpenGLBuffers();
    //! Update the OpenGL buffer objects containing the vertex data.
    void updateOpenGLBuffers();
    //! Draws the vertex data depending on the fill rule that was set for this object.
    void drawDependingOnFillRule() const;
    //! called each time before rendering if VAO's not used (GLES2)
    //! called once upon setup if VAO's are used (GLES3)
    void bindVertexAttributes() const;

    //! OpenGL vao used for the vertices
    GLuint d_verticesVAO;
    GLint d_posAttrib;
    GLint d_texAttrib;
    GLint d_colAttrib;

    //! OpenGL vbo containing all vertex data
    GLuint d_verticesVBO;
    //! Pointer to the OpenGL state changer wrapper that was created inside the Renderer
    OpenGLBaseStateChangeWrapper* d_glStateChanger;
    //! Size of the buffer that is currently in use
    GLuint d_bufferSize;
};

}

#endif

