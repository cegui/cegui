/***********************************************************************
    filename:   StateChangeWrapper.h
    created:    Wed, 8th Feb 2012
    author:     Lukas E Meindl
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

#ifndef _CEGUIOpenGL3StateChangeWrapper_h_
#define _CEGUIOpenGL3StateChangeWrapper_h_

#include "CEGUI/RendererModules/OpenGL/GL.h"
#include "CEGUI/RendererModules/OpenGL/GL3Renderer.h"

#if defined(_MSC_VER)
#   pragma warning(push)
#   pragma warning(disable : 4251)
#endif

// Start of CEGUI namespace section
namespace CEGUI
{

/*!
\brief
OpenGL3StateChangeWrapper - wraps OpenGL calls and checks for redundant calls beforehand
*/
class OPENGL_GUIRENDERER_API OpenGL3StateChangeWrapper :
    public AllocatedObject<OpenGL3StateChangeWrapper>
{
protected:
    struct BlendFuncParams
    {
        BlendFuncParams();
        void reset();
        bool equal(GLenum sFactor, GLenum dFactor);
        GLenum d_sFactor, d_dFactor;
    };
    struct BlendFuncSeperateParams
    {
        BlendFuncSeperateParams();
        void reset();
        bool equal(GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha, GLenum dfactorAlpha);
        GLenum d_sfactorRGB, d_dfactorRGB, d_sfactorAlpha, d_dfactorAlpha;
    };
    struct PortParams
    {
        PortParams();
        void reset();
        bool equal(GLint x, GLint y, GLsizei width, GLsizei height);
        GLint d_x, d_y;
        GLsizei d_width, d_height;
    };
    struct BindBufferParams
    {
        BindBufferParams();
        void reset();
        bool equal(GLenum target, GLuint buffer);
        GLenum d_target;
        GLuint d_buffer;
    };
    struct BoundTexture
    {
        BoundTexture();
        void bindTexture(GLenum target, GLuint texture);
        GLenum d_target;
        GLuint d_texture;
    };

public:
    //! constructor.
    OpenGL3StateChangeWrapper(OpenGL3Renderer& owner);
    //! destructor
    virtual ~OpenGL3StateChangeWrapper();

    
    //! Due to unknown changes of states between each time CEGUI gets rendered, we will invalidate
    //! all states on CPU-side so that the following calls will definitely change the states on GPU
    void reset();

    void bindVertexArray(GLuint vertexArray);
    void useProgram(GLuint program);
    void blendFunc(GLenum sfactor, GLenum dfactor);
    void blendFuncSeparate(GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha, GLenum dfactorAlpha);
    void viewport(GLint x, GLint y, GLsizei width, GLsizei height);
    void scissor(GLint x, GLint y, GLsizei width, GLsizei height);
    void bindBuffer(GLenum target, GLuint buffer);
    /*
    \brief
        This function takes the number representing the texture position as integer, not the actual OpenGL value
        for the position, such as GL_TEXTURE0, GL_TEXTURE1 etc. 
        
    \param texture_position
        Variable representing the texture position as integer, it will be used in the following way to get the
        OpenGL Texture position: (GL_TEXTURE0 + texture_position)
    */
    void activeTexture(unsigned int texture_position);
    void bindTexture(GLenum target, GLuint texture);

protected:
    GLuint                      d_vertexArrayObject;
    GLuint                      d_shaderProgram;
    BlendFuncParams             d_blendFuncParams;
    BlendFuncSeperateParams     d_blendFuncSeperateParams;
    PortParams                  d_viewPortParams;
    PortParams                  d_scissorParams;
    BindBufferParams            d_bindBufferParams;
    //! The active texture saved as integer and not as OpenGL enum
    unsigned int                 d_activeTexturePosition;
    //! List of bound textures, the position in the vector defines the active texture it is bound to
    std::vector<BoundTexture>   d_boundTextures;
};

} // End of  CEGUI namespace section

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif

#endif
